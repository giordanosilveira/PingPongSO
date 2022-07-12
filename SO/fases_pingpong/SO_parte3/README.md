# Dispatcher
Implementação de um dispatcher e um escalonador simples.
## Descrição
As funções que eu implemento aqui são a **task_yield()**, **escalonador()** e 
**dispatcher()**. Posso ter implementado mais algumas funções mas é somente para
a modularização.
    * **dispatcher():** Basicamente, carrega uma tarefa escolhida pelo escalonador
    para a execução. 
    * **escalonador():** O escalonador desta parte do projeto é simplemente
    pegar a primeira tarefa da fila.
    * **task_yield():** Passa o controle da main e das outras funções para o
    dispatcher.
## test_case
Contém a resposta esperada para a execução do programa **pingpong-dispatcher.c**
Compare usando um diff. Tanto o programa **pingpong-dispatcher.c** quanto a resposta
esperada foi feita por [Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).
###### "That's all folks!!!"