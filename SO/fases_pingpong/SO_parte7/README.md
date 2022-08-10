# Operador Join
O objetivo deste projeto é modificar seu sistema de forma que a função main seja vista e escalonada como uma tarefa idêntica às demais tarefas (**ping**, **pong**, etc).

## Descrição
Eu não faço nenhuma outra função neste projeto. Em relação ao P6, a única coisa que mudo
é que a **main_task** vira uma tarefa preemptável e é colocada na fila de tarefas prontas.
Basicamente é isso.

## test_case
Neste diretório contém a resposta esperada para a execução de **pingpong_maintask.c**.
Como as respostas podem variar de computador para computador, o resultado não precisa ser igual.
O que precisa ser igual, é a main podendo terminar antes das outras tarefas.

Tanto o programa **pingpong_join.c**, quanto a resposta esperada foi feita por.
[Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).

###### That's all folks !!!