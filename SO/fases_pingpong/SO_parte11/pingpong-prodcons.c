#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"

#define TAM_BUFFER 4

int ultima_insersao = -1;
int ultimo_consumido = -1;

int buffer[TAM_BUFFER];

task_t prod1, prod2, prod3, cons1, cons2;
semaphore_t s_buffer, s_item, s_vaga;

void produtor(void * arg) {

    int item; 
    while(1) {
        task_sleep(1000);

        item = rand() % 100;
        sem_down(&s_vaga);

        sem_down(&s_buffer);
        ultima_insersao++;
        if (ultima_insersao == 5)
            ultima_insersao = 0;
        buffer[ultima_insersao] = item;
        printf("%s produziu %d\n", (char*)arg, item);
        sem_up(&s_buffer);


        sem_up(&s_item);
    }   
}

void consumidor(void *arg){

    int item;
    while(1) {
        sem_down(&s_item);

        sem_down(&s_buffer);
        ultimo_consumido++;
        if (ultimo_consumido == 5)
            ultimo_consumido = 0;
        item = buffer[ultimo_consumido];
        printf("%s consumiu %d\n", (char*)arg, item);
        sem_up(&s_buffer);

        sem_up(&s_vaga);

    }

}

int main(){

    printf("main : início\n");

    ppos_init();

    sem_create(&s_buffer, 1);
    sem_create(&s_item, 0);
    sem_create(&s_vaga, 5);


    task_create(&prod1, produtor, "p1");
    task_create(&prod2, produtor, "p2");
    task_create(&prod3, produtor, "p3");
    task_create(&cons1, consumidor, "                  c1");
    task_create(&cons2, consumidor, "                  c2");

    task_join(&prod1);

    return 1;
}
