//GRR20197154 Giordano Henrique Silveira

// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022

// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#include <ucontext.h>		// biblioteca POSIX de trocas de contexto

#define STACKSIZE 64*1024

#define TASK_AGING -1
#define TEMPORIZADOR 1000
#define TASK_TIMER 20

#define VIVO 1
#define MORTO 0

#define YES 1
#define NO 0

#define READY 1
#define STOP 2
#define FINISHED 3

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
    struct task_t *prev, *next ;		// ponteiros para usar em filas
    int id ;				            // identificador da tarefa
    ucontext_t context ;			    // contexto armazenado da tarefa
    short status ;			            // pronta, rodando, suspensa, ...
    int priority;                       // Prioridade estática (inicial)
    int dinamic_prio;                   // Prioridade dinâmica
    short preemptable ;			        // pode ser preemptada?
  
    int task_timer;
  
    unsigned int init_time;             // Tempo do início da tarefa
    unsigned int activations;           // Números de ativações que a tarefa teve
    unsigned int exec_time;             // Tempo de total de execução
    unsigned int processor_time;        // Tempo de processamento total
    unsigned int sleep_systime;         // Guarda a hora que a tarefa dormiu
    unsigned int sleep_time;            // Tempo do sleep

    struct task_t *suspended_task;      // Fila usada para suspender uma tarefa que depende da corrente
  

  // ... (outros campos serão adicionados mais tarde)
} task_t ;

// estrutura que define um semáforo
typedef struct
{
    int count;
    int alive;
    int lock;
    task_t *semaphore_queue;

} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif

