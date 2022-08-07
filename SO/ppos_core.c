#include "ppos.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

task_t *current_task;
task_t *main_task;
task_t *old_task;
task_t *dispatcher_task;
task_t *ready_tasks;
unsigned int global_clock;

struct sigaction action;
struct itimerval timer;


int user_tasks = 0;
int current_id = 0;

// imprime na tela um elemento da fila (chamada pela função queue_print)
void print_elem (void *ptr) {
   task_t *elem = ptr ;

   if (!elem)
      return ;

   printf ("%d ", elem->id) ;
   //elem->next ? printf ("%d", elem->next->dinamic_prio) : printf ("*") ;
}

/**
 * @brief trata das interrupções do timer
 *
 */
void tratador () {

    global_clock++;

    //Aumenta o tempo da tarefa em um ms
    current_task->processor_time++;

    //Se a tarefa for preemptavel
    if (current_task->preemptable) {

        //Diminui o tick dele
        current_task->task_timer--;

        //Quando chegar a 0, retorna o timer para o valor inicial e chama o task_yield()
        if (! current_task->task_timer) {
            current_task->task_timer = TASK_TIMER;
            task_yield();
        }
    }

}

/**
 * @brief Inicializa a estrutura que vai tratar da ação
 * do temporizador.
 *
 */
void init_signal_handler () {
    
    // registra a ação para o sinal de timer SIGALRM
    action.sa_handler = tratador ;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;
    if (sigaction (SIGALRM, &action, 0) < 0)
    {
      perror ("Erro em sigaction: ") ;
      exit (1) ;
    }
    
}

/**
 * @brief Inicializa o temporizador em 1 ms 
 *
 */
void init_timer(){
    
    //Ajusta o relogío global do sistema
    global_clock = 0;

    // ajusta valores do temporizador
    timer.it_value.tv_usec = TEMPORIZADOR ;         // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec  = 0 ;                    // primeiro disparo, em segundos
    timer.it_interval.tv_usec = TEMPORIZADOR ;      // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec  = 0 ;                 // disparos subsequentes, em segundos
    
    // arma o temporizador ITIMER_REAL (vide man setitimer)
    if (setitimer (ITIMER_REAL, &timer, 0) < 0)
    {
      perror ("Erro em setitimer: ") ;
      exit (1) ;
    }

}

/**
 * @brief Inicializa as estruturas internas da
 * estutura task_t.
 * 
 * @param task (task_t*) : Uma tarefa para ser inicializada
 */
void init_struct_task(task_t *task){

    //Inicializa algumas estruturas internas
    task->id = current_id;
    task->next = task->prev = NULL;
    task->status = READY;
    task_setprio(task, 0);
    task->dinamic_prio = task->priority;
    task->task_timer = TASK_TIMER;

    task->activations = 0;
    task->exec_time = systime();
    task->processor_time = 0;

    if (task != dispatcher_task)
        task->preemptable = YES;
    else
        task->preemptable = NO;

    //Incrementa o current_id
    current_id++;
    #ifdef DEBUG
    printf("init_struct_task(): current_id %d\n", current_id);
    #endif

}

/**
 * @brief Escolhe a próxima tarefa a ser executada.
 * NESTE CASO, a tarefa mais prioritaria da fila (i.e menor valor).
 * 
 * @return task_t* : A próxima tarefa a ser executada.
 */
task_t* escalonador () {

    task_t *aux = ready_tasks;
    task_t *task_min_priority = aux;

    

    //Percorre a fila de tarefas e procura a menos prioritária
    aux = ready_tasks;

    //Sempre verifica a próxima tarefa
    while (aux->next != ready_tasks){
        if (aux->next->dinamic_prio < task_min_priority->dinamic_prio){
            task_min_priority = aux->next;
        }   
        aux = aux->next;
    }

    //Percorre a fila e diminui as prioridade dinâmicas das tarefas
    aux = ready_tasks;
    while (aux->next != ready_tasks) {
        if (aux->next != task_min_priority)
            aux->next->dinamic_prio += TASK_AGING; 
        aux = aux->next;
    }
    //Diminuição da prioridade da primeira tarefa
    if (aux->next != task_min_priority) {
        aux->next->dinamic_prio += TASK_AGING;
    }


    //task_min_priority->dinamic_prio = task_min_priority->priority;
    #ifdef DEBUGESCALONADOR
    printf("escalonador() tarefa->%d\n", task_min_priority->id);
    #endif
    return task_min_priority;;

}

/**
 * @brief Coloca uma tarefa da lista para executar.
 * 
 */
void dispatcher () {

    task_t *next_task;

    //Enquanto tem tarefa dos usuário para executar
    while (user_tasks) {
    
        //seleciona a próxima tarefa
        next_task = escalonador();

        //verifica se ela existe
        if (next_task) {
            
            //Se não conseguiu trocar a tarefa da uma mensagem de erro
            //e aborta o programa
            next_task->activations++;
            if (task_switch(next_task) < 0){
                fprintf(stderr, "Erro ao troca para a proxima tarefa\n");
                exit(1);
            }

            //Verifica o estado da tarefa após executar
            switch (next_task->status){

                //Caso ela esteja finalizada
                case FINISHED:

                    //Retira ela da fila
                    if(! queue_remove((queue_t **)(&ready_tasks), (queue_t*)(next_task)))
                        exit(1);
                    break;
                default:
                    next_task->dinamic_prio = next_task->priority;
                    break;
            }
        }
    }
    task_exit(0);
}

/**
 * @brief Define uma nova prioridade para a
 * tarefa passada como argumento.
 * 
 * @param task (task_t*) : Uma tarefa.
 * @param prio (int) : Nova prioridade da tarefa.
 */
void task_setprio(task_t *task, int prio){
    //Se task for nula, muda a prioridade da tarefa corrente
    if (! task)
        current_task->priority = prio;

    //Senão, muda a tarefa passada no argumento
    task->priority = task->dinamic_prio = prio;
}

/**
 * @brief Retorna a prioridade da tarefa passada
 * como argumento.
 * 
 * @param task (task_t*) : Uma tarefa.
 * @return int : Prioridade da tarefa.
 */
int task_getprio(task_t * task){

    //Se task for nula, retorna a prioridade da tarefa corrente
    if (! task)
        return current_task->priority;
    
    //Senão, retorna a prioridade da tarefa passada
    return task->priority;

}


void task_suspend(task_t **queue){

    if (queue_remove((queue_t**)(&ready_tasks), (queue_t *)(current_task)) < 0) {
        fprintf(stderr, "task_suspend(): "); 
        fprintf(stderr, "A tarefa %d não se evncontra na fila de tarefa de prontas\n", current_task->id);
        exit(1);
    }

    current_task->status = STOP;

    if (queue_append((queue_t **)(queue), (queue_t*)(current_task)) < 0) {
        fprintf(stderr, "task_suspend(): "); 
        fprintf(stderr, "A tarefa %d não pode ser colocada na fila\n", current_task->id);
        exit(1);
    }

    task_yield();
}


void task_resume(task_t *task, task_t **queue){

    if (! queue) {
        fprintf(stderr, "task_resume(): "); 
        fprintf(stderr, "A fila de tarefas não existe\n");
        exit(1);
    }

    if (queue_remove((queue_t **)(queue), (queue_t*)(task)) < 0){
        fprintf(stderr, "task_resume(): "); 
        fprintf(stderr, "A tarefa %d não pode ser retirada da fila\n", task->id);
        exit(1);
    }

    task->status = READY;

    if (queue_append((queue_t **)(&ready_tasks), (queue_t*)(task)) < 0) {
        fprintf(stderr, "task_resume(): "); 
        fprintf(stderr, "A tarefa %d não pode ser colocada na fila\n", task->id);
        exit(1);
    }

}


int task_join(task_t *task){

    if (! task){
        fprintf(stderr, "A tarefa é nula\n");
        return -1;
    }

    task_suspend(&task->suspended_task);

    return task->id;

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

    //inicializa tarefa dispatcher
    dispatcher_task = malloc(sizeof(task_t)*1);
    if (! dispatcher_task){
        fprintf(stderr, "O ponteiro do dispatcher não pode ser inicializado\n");
        exit(1);
    }
    
    //inicializa estrutas internas da task main e dispatcher
    init_struct_task(main_task);

    ready_tasks = NULL;

    user_tasks++;
    queue_append((queue_t**)(&ready_tasks), (queue_t*)(main_task));
    //queue_print("ahg ", (queue_t*)(ready_tasks), print_elem);
    
    task_create(dispatcher_task, dispatcher, NULL);
    
    //tarefa atual é a main
    current_task = main_task;

    init_signal_handler();
    init_timer();

    task_yield();
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

    //Inicializa estruturas internas da tarefa criada
    init_struct_task(task);

    //Se ela não for o próprio dispatcher, coloca ela na lista
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

    //Salva a tarefa antiga
    //A tarefa atual é a que será executada no momento
    old_task = current_task;
    current_task = task;

    //Troca os contextos
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

    //Se foi finalizada com um zero a tarefa finalizou
    if (exit_code >= 0) {
        current_task->status = FINISHED;
        user_tasks--;
    }

    //Salva a tarefa que foi finalizada
    old_task = current_task;
    
    //Caso a tarefa finalizada seja o dispatcher, retorna para a main
    if (current_task == dispatcher_task) {

        // O tempo total do dispatcher. tempo da criação - tempo da finalização
        dispatcher_task->exec_time += (systime() - dispatcher_task->exec_time);
        printf("Task %d exit: execution time %d ms, processor time     %d ms, %d activations\n", task_id(), dispatcher_task->exec_time, dispatcher_task->processor_time, dispatcher_task->activations);
        
        //Uso o dispatcher_task para ficar mais claro o que estou mudando
        if (task_switch(main_task) < 0){
            fprintf(stderr, "Não foi possível troca a tarefa\n");
            exit(1);
        }
    }
    else {

        task_t *aux = current_task->suspended_task;
        for (int i = 0; i < queue_size((queue_t *)(current_task->suspended_task)); i++){
            task_resume(aux, &current_task->suspended_task);
            aux = aux->next;
        }

        // O tempo total de uma tarefa qualquer. Tcriação - Texit
        current_task->exec_time += (systime() - current_task->exec_time);
        printf("Task %d exit: execution time %d ms, processor time  %d ms, %d activations\n", task_id(), current_task->exec_time, current_task->processor_time, current_task->activations);

        //Caso não, retorna para a task dispatcher
        if (task_switch(dispatcher_task) < 0){
                fprintf(stderr, "Não foi possível troca a tarefa\n");
                exit(1);
        }

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

    //Testa para ver se a current_task é válida
    if (! current_task){
        fprintf(stderr, "Tarefa atual nula : task_id()\n");
        return -1;
    }
    return current_task->id;
}

/**
 * @brief O Controle é passado para a task dispatcher
 * 
 */
void task_yield() {

    #ifdef DEBUGESCALONADOR1
    queue_print("fila escalonador ->", (queue_t*)ready_tasks, print_elem);
    #endif


    //indica que a task atual esta parada e que a task dispatcher
    //está pronta para executar 
    current_task->status = STOP;
    dispatcher_task->status = READY;

    //Troca o contexto

    dispatcher_task->activations++;
    task_switch(dispatcher_task);

}

/**
 * @brief Retorna o relogio globa do sistema
 * 
 * @return (unsigned int) : relogío do sistema 
 */
unsigned int systime () {
    return global_clock;
}