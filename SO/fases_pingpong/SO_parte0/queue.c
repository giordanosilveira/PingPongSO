//GRR20197154 Giordano Henrique Silveira

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size(queue_t *queue) {

    queue_t * aux = queue;
    int contador;
    
    if (queue == NULL){
        fprintf(stderr, "O ponteiro da fila é nulo : queue_size()\n");
        return 0;
    }

    if (queue->next == queue && queue->prev == queue) {
        return 1;
    }

    contador = 1;
    while (aux->next != queue) {
        aux = aux->next;
        contador++;
    }

    return contador;
}

// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:

// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir
void queue_print (char *name, queue_t *queue, void print_elem (void*) ){

    queue_t *primeiro, *aux;

    printf("\n%s: [", name);

    //Se o ponteiro não é nulo
    if (! queue) {
        fprintf(stderr, "O ponteiro da fila é nulo\n");
        printf("]\n");
        return;
    }

    //Se a lista é vazia
    if (! queue_size(queue)){
        fprintf(stderr, "A lista é vazia\n");
        printf("]\n");
        return;
    }

    primeiro = queue;
    aux = queue;
    print_elem(aux);
    printf(" ");
    while (aux->next != primeiro) {
        aux = aux->next;
        print_elem(aux);
        if (! (aux->next == primeiro))
            printf(" ");
    }
    printf("]\n");
    return;
}

// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_append (queue_t **queue, queue_t *elem) {

    queue_t *ultimo;
    
    if (! queue) {
        fprintf(stderr, "O ponteiro da fila é nulo: queue_append()\n");
        return -1;
    }
    
    if (! elem) {
        fprintf(stderr, "O ponteiro do elemento é nulo: queue_append()\n");
        return -2;
    }

    if (elem->next != NULL || elem->prev != NULL) {
        fprintf(stderr, "Os ponteiros next ou prev do elemento não são nulos: queue_append()\n");
        return -3;
    }

    //Se a lista estiver vazia
    if (! queue_size(*queue)){
        *queue = elem;
        (*queue)->next = *queue;
        (*queue)->prev = *queue;
        return 1;
    }

    ultimo = (*queue)->prev;

    elem->next = (*queue);
    elem->prev = ultimo;
    (*queue)->prev = elem;
    ultimo->next = elem;

    return 1;
}

// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_remove (queue_t **queue, queue_t *elem){

    //A fila é nula
    if (! queue) {
        fprintf(stderr, "O ponteiro da fila é nulo: queue_remove()\n");
        return -1;
    }

    //O elemento é nulo
    if (! elem){
        fprintf(stderr, "O ponteiro do elemento é nulo: queue_remove()\n");
        return -2;
    }

    //A fila não tem nenhum elemento
    if (! queue_size(*queue)) {
        fprintf(stderr, "A fila está vazia: queue_remove()\n");
        return -3;
    }

    //A fila tem um elemento e o unico elemento é o que você quer remover
    if ((*queue) == elem) {
        if (queue_size(*queue) == 1) {
            (*queue)->next = NULL;
            (*queue)->prev = NULL;
            *queue = NULL;
            return 1;
        }
        else {
            queue_t *aux = *queue;
            *queue = (*queue)->next;
            (*queue)->prev = aux->prev;
            (*queue)->prev->next = (*queue);
            aux->next = NULL;
            aux->prev = NULL;
            return 1;
        }
    }
    
    //Caso geral
    queue_t *atual = *queue;
    queue_t *remover = NULL;

    while (! (atual->next == *queue)) {
        if (atual == elem)
            remover = atual;
        atual = atual->next;
    }
    if (atual == elem)
        remover = atual;

    if (! remover){
        fprintf(stderr, "O elemento não está nessa fila: queue_remove()\n");
        return -4;
    }
    
    remover->prev->next = remover->next;
    remover->next->prev = remover->prev;
    remover->next = NULL;
    remover->prev = NULL;

    return 1;
}
