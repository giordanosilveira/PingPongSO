#include "ppos.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

task_t *current_task;
task_t *main_task;
task_t *old_task;
task_t *dispacher;
task_t *ready_tasks;

int user_tasks = 0;
int current_id = 0;

void ppos_init () {

    #ifdef DEBUG
    printf("ppos_init: inicializando estruturas\n");
    #endif

    char *stack;

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
    
    main_task->id = current_id;
    current_task = main_task;
}

int task_create(task_t *task, void (*start_func)(void *), void *arg) {

    #ifdef DEBUG
    printf("task_create: criando a tarefa %d\n", current_id + 1);
    #endif

    ucontext_t context;
    char *stack;

    //Testa para ver se o contexto foi criado
    if (getcontext(&context) == -1){
        fprintf(stderr, "Erro ao criar a tarefa %d", current_id + 1);
        exit(0);
    }

    //Inicializa a pilha para o contexto atual
    stack = malloc(STACKSIZE);
    if (! stack) {
        fprintf(stderr, "Erro ao alocar a stack para o contexto da tarefa: %d\n", current_id + 1);
        return -1;
    }

    //Colocar as tarefas dentro da fila
    //Incrementar o user_tasks
    //task->status = READY

    //Inicializa as variáveis usadas para a troca de contexto
    context.uc_stack.ss_sp = stack;
    context.uc_stack.ss_size = STACKSIZE;
    context.uc_stack.ss_flags = 0;
    context.uc_flags = 0;

    //Cria o contexto para a função específicada
    makecontext(&context, (void *)(*start_func), 1, arg);

    //O contexto da tarefa atual recebe o contexto específicado
    task->context = context;
    task->id = current_id + 1;

    //Incrementa o ID
    current_id = current_id + 1;
    return current_id;
}

int task_switch (task_t *task) {

    #ifdef DEBUG
    printf("task_switch: trocando contexto %d -> %d\n", current_task->id, task->id);
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

void task_exit(int exit_code){

    #ifdef DEBUG
    printf("tast_exit: tarefa %d sendo encerrada\n", current_task->id);
    #endif

    old_task = current_task;
    task_switch(main_task);
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

}