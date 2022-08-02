# Contabilização de tarefas
Implementação de como contabilizar os tempos de cada tarefa.

A ideia aqui é simular como o S.O. conta os tempos de cada tarefa e suas ativações.
Aqui eu meço três coisa: o tempo de execução total **exec_time**, o tempo que ele
passou por processamento **processor_time** e quantas ativações ele teve **activations**.

## Descrição
A função que implemento aqui é a **systime()** que basicamente retorna o relógio global do sistema.

O mais importante é como eu contabilizo o tempo. 
A forma que eu contabilizo é basicamente aumentar 1 a casa vez que a função tratador é chamado.
Se o tempo da tarefa diminui 1, então, ela passou 1 segundo no processador.

## test_case
Contém a resposta esperada tanto para a execução do programa **pingpong-contab-prio.c**, quanto **pingpong-contab.c**.

Aqui não da para comparar usando um diff, porque varia de máquina para máquina.
O mais importante é a soma do tempo de processamento de cada tarefa ser parecido e igual ao tempo total.
O tarefa dispatcher deve ser zero (ou proximo de zero) porque ele executa em menos de um milesegundo.

Tanto o programa **pingpong-contab.c** e **pingpong-contab-prio.c**, quanto a resposta esperada 
foi feita por [Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).
###### "That's all folks!!!"