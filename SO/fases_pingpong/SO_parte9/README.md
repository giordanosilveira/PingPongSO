# Sleep
O objetivo deste projeto é contruir uma função denominada **task_sleep**, que permite que a
tarefa seja suspensa e colocada numa fila de tarefas suspensas sem interferir nas demais. O objetivo é simular a chamda **sleep()**.

## Descrição
Neste projeto eu crio basicamente duas funções: **task_sleep()** e **awake_tasks()**.
    * **task_sleep**: Tira uma tarefa da fila de prontos, coloca ela para dormir e chama o
    dispatcher.
    * **awake_tasks**: Se for o tempo correto, acorda uma tarefa da fila de suspensas. Isto é,
    tira a tarefa da fila de suspensas e coloca ela na fila de prontas.

## test_case
Neste diretório contém a resposta esperada para a execução de **pingpong_sleep.c**.
Como as respostas podem variar de computador para computador, o resultado não precisa ser igual.
O que precisa ser igual, é a os "ok" estarem todos corretos.

Tanto o programa **pingpong_sleep.c**, quanto a resposta esperada foi feita por.
[Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).

###### That's all folks !!!