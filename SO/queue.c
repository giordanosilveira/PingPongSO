#include "queue.h"

// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size(queue_t *queue) {

    queue_t * aux = queue;
    int contador;
    
    if (! queue)
        return 0;

    while (aux->next != queue) {
        aux = aux->next;
        contador++;
    }

    return contador;
}

// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_append (queue_t **queue, queue_t *elem) {

    queue_t *ultimo;
    
    if (! queue_size(*queue))
        return -1;
    
    if (! elem)
        return -2;

    if (elem->next != NULL || elem->prev != NULL) {
        return -3;
    }

    ultimo = (*queue)->prev;

    elem->next = (*queue);
    elem->prev = ultimo;
    (*queue)->prev = elem;
    ultimo->next = elem;

    return 1;
}
