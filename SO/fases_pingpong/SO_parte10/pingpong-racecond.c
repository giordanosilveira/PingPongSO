// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022

// Teste de semáforos (pesado)

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "queue.h"

#define NUMTASKS 30
#define NUMSTEPS 100000

task_t task[NUMTASKS] ;
semaphore_t  s ;
long int soma = 0 ;

// corpo das tarefas
void taskBody(void *id)
{
  int i ;

  for (i=0; i< NUMSTEPS; i++)
  {
    // incrementa contador (seção crítica)
    sem_down (&s) ;
    soma += 1 ;
    //printf("soma %ld contador semaforo %d\n", soma, s.count);
    fprintf (stderr, "%d\n", (long) id) ;
    sem_up (&s) ;
  }
  //printf("saí\n");
  task_exit (0) ;
}
int main (int argc, char *argv[])
{
  int i ;

  printf ("main: inicio\n") ;

  ppos_init () ;

  // inicializa semáforo em 0 (bloqueado)
  sem_create (&s, 0) ;

  printf ("%d tarefas somando %d vezes cada, aguarde...\n",
          NUMTASKS, NUMSTEPS) ;

  // cria as tarefas
  for (i=0; i<NUMTASKS; i++)
    task_create (&task[i], taskBody, (void*) i) ;

  // espera um pouco e libera o semáforo
  task_sleep (20) ;
  sem_up (&s) ;
  printf("UP main\n");

  // aguarda as tarefas encerrarem
  for (i=0; i<NUMTASKS; i++)
    task_join (&task[i]) ;

  // destroi o semáforo
  sem_destroy (&s) ;

  // verifica se a soma está correta
  if (soma == (NUMTASKS*NUMSTEPS))
    printf ("Soma deu %ld, valor correto!\n", soma) ;
  else
    printf ("Soma deu %ld, mas deveria ser %d!\n",
            soma, NUMTASKS*NUMSTEPS) ;

  task_exit (0) ;

  exit (0) ;
}
