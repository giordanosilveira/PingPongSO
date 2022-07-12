# Escalonador por prioridades
Implementação de um escalonador por prioridade.
## Descrição
As funções que eu implemento aqui são a **task_setprio()** e **task_getprio()**. A 
função **escalonador()** é modificada significativamente. 
    * **task_setprio():** Define uma prioridade para a tarefa.
    * **task_getprio():** Retorna a prioridade da tarefa.
    * **Escalonador():** Agora ela percorre a fila de tarefas procurando a menor tarefa.
    Escolhida a tarefa, tem outro laço para envelhecer as tarefas restantes.
## test_case
Contém a resposta esperada para a execução do programa **pingpong-scheduler.c**
Compare usando um diff. Tanto o programa **pingpong-scheduler.c**, quanto a resposta
esperada foi feita por [Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).
###### "That's all folks!!!"