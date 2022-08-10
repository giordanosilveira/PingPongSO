# Operador Join
O objetivo deste projeto é contruir uma função de sincronização denominada **task_join**, que permite que uma tarefa espere a conclusão de outra tarefa, de forma similar à chamada POSIX **pthread_join**.

## Descrição
A função que implemento a aqui basicamente são 3: **task_join**, **task_resume**, **task_suspend**. A **task_join** foi explicada logo acima.
    - **task_resume**: coloca uma tarefa de uma fila para a fila de prontas
    - **task_suspend**: coloca a tarefa corrente para dormir na fila de tarefa
    suspensas.

## test_case
Neste diretório contém a resposta esperada para a execução de **pingpong_join.c**.
Como as respostas podem variar de computador para computador, o resultado não precisa ser igual.
O que precisa ser igual, é a main esperando as tarefas terminando de executar para que
ela possa executar.

Tanto o programa **pingpong_join.c**, quanto a resposta esperada foi feita por.
[Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).

###### That's all folks !!!