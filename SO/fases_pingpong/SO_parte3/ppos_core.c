#include "ppos.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

task_t *current_task;
task_t *main_task;
task_t *old_task;
task_t *dispatcher_task;
task_t *ready_tasks;

int user_tasks = 0;
int current_id = 0;

// imprime na tela um elemento da fila (chamada pela função queue_print)
void print_elem (void *ptr)
{
   task_t *elem = ptr ;

   if (!elem)
      return ;

   printf (" %d>", elem->id) ;
   elem->next ? printf ("%d", elem->next->id) : printf ("*") ;
}

/**
 * @brief Inicializa as estruturas internas da
 * estutura task_t.
 * 
 * @param task (task_t*) : Uma tarefa para ser inicializada
 */
void init_struct_task(task_t *task){

    task->id = current_id;
    task->next = task->prev = NULL;
    task->status = READY;

    current_id++;
    #ifdef DEBUG
    printf("init_struct_task(): current_id %d\n", current_id);
    #endif

}

task_t* escalonador () {

    task_t *aux;

    aux = ready_tasks;
    ready_tasks = ready_tasks->next;
    
    #ifdef DEBUG
    printf("escalonador(): proxima tarefa %d = %d\n", aux->id, ready_tasks->prev->id);
    queue_print("[",(queue_t *)(ready_tasks->prev),print_elem);
    #endif

    return aux;

}

/**
 * @brief Coloca uma tarefa da lista para executar.
 * 
 */
void dispatcher () {

    task_t *next_task;
    while (user_tasks) {
        
        next_task = escalonador();
        if (next_task) {
        
            #ifdef DEBUG
            printf("dispatcher() : indo para a tarefa -> %d\n", next_task->id);
            #endif

            if (task_switch(next_task) < 0){
                fprintf(stderr, "Erro ao troca para a proxima tarefa\n");
                exit(1);
            }

            switch (next_task->status){
                case FINISHED:
                    if(! queue_remove((queue_t **)(&ready_tasks), (queue_t*)(next_task)))
                        exit(1);
                    break;
                default:
                    break;
            }
        }
    }
    task_exit(0);
}

/**
 * @brief Inicializa as variáveis 
 * do Sistema Operacional.
 */
void ppos_init () {

    #ifdef DEBUG
    printf("ppos_init: inicializando estruturas\n");
    printf("Criando tarefa main %d\n", current_id);
    #endif

    setvbuf(stdout, 0, _IONBF, 0);

    //Inicializa função main
    main_task = malloc(sizeof(task_t)*1);
    if (! main_task) {
        fprintf(stderr, "Erro ao alocar a função main\n");
        exit(1);
    }

    //Pega o contexto da função main
    if (getcontext(&(main_task->context)) == -1) {
        fprintf(stderr, "Erro ao criar o contexto para a função main\n");
        exit(1);
    }

    dispatcher_task = malloc(sizeof(task_t)*1);
    if (! dispatcher_task){
        fprintf(stderr, "O ponteiro do dispatcher não pode ser inicializado\n");
        exit(1);
    }
    
    init_struct_task(main_task);
    task_create(dispatcher_task, dispatcher, NULL);
    
    ready_tasks = NULL;

    
    current_task = main_task;
}

/**
 * @brief Cria uma tarefa.
 * 
 * @param task (taks_t*) : Ponteiro que será criada a tarefa.
 * @param start_func (function *) : Função da tarefa.
 * @param arg (void *) : Argumento da função passada.
 * 
 * @return int : -1, se der algum erro; id da tarefa, se der certo.
 */
int task_create(task_t *task, void (*start_func)(void *), void *arg) {

    #ifdef DEBUG
    printf("task_create(): criando a tarefa %d\n", current_id);
    #endif

    ucontext_t context;
    char *stack;

    //Testa para ver se o contexto foi criado
    if (getcontext(&context) == -1){
        fprintf(stderr, "Erro ao criar a tarefa %d", current_id + 1);
        return -1;
    }

    //Inicializa a pilha para o contexto atual
    stack = malloc(STACKSIZE);
    if (! stack) {
        fprintf(stderr, "Erro ao alocar a stack para o contexto da tarefa: %d\n", current_id + 1);
        return -1;
    }

    //Inicializa as variáveis usadas para a troca de contexto
    context.uc_stack.ss_sp = stack;
    context.uc_stack.ss_size = STACKSIZE;
    context.uc_stack.ss_flags = 0;
    context.uc_flags = 0;

    //Cria o contexto para a função específicada
    makecontext(&context, (void *)(*start_func), 1, arg);

    //O contexto da tarefa atual recebe o contexto específicado
    task->context = context;
    init_struct_task(task);

    if (task != dispatcher_task){
        queue_append((queue_t**)(&ready_tasks), (queue_t*)(task));
        user_tasks++;
    }
    return current_id;
}

/**
 * @brief Troca a tarefa atual com a passada no argumento
 *
 * @param task (_task_t*_) | Tarefa a ser executada
 *  
 * @return int : -1 se der algum erro, 0 se a tarefa foi trocada
 */
int task_switch (task_t *task) {

    #ifdef DEBUG
    printf("INICIO: task_switch(): trocando contexto %d -> %d\n", current_task->id, task->id);
    #endif
    
    if (! task) {
        fprintf(stderr, "O ponteiro da tarefa é nulo: task_switch() \n");
        return -1;
    }

    old_task = current_task;
    current_task = task;

    if (swapcontext(&(old_task->context), &(task->context)) == -1) {
        fprintf(stderr, "Erro ao trocar contexto\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Termina uma tarefa
 * 
 * @param exit_code (_int_) : Código para terminar
 *  
 */
void task_exit(int exit_code){

    #ifdef DEBUG
    printf("tast_exit: tarefa %d sendo encerrada\n", current_task->id);
    #endif

    if (exit_code == 0) {
        current_task->status = FINISHED;
        user_tasks--;
    }

    old_task = current_task;
    
    if (current_task == dispatcher_task) {
        if (task_switch(main_task) < 0){
            fprintf(stderr, "Não foi possível troca a tarefa\n");
            exit(1);
        }
    }

    if (task_switch(dispatcher_task) < 0){
            fprintf(stderr, "Não foi possível troca a tarefa\n");
            exit(1);
    }
}

/**
 * @brief Retorna o id da tarefa.
 * 
 * @return int : id da tarefa. 
 */
int task_id(){

    #ifdef DEBUG
    printf("task_id()");
    #endif

    if (! current_task){
        fprintf(stderr, "Tarefa atual nula : task_id()\n");
        return -1;
    }
    return current_task->id;
}

void task_yield() {

    #ifdef DEBUG
    printf("task_yield(): tarefa atual: %d\n", current_task->id);
    #endif

    current_task->status = STOP;
    dispatcher_task->status = READY;
    task_switch(dispatcher_task);
}