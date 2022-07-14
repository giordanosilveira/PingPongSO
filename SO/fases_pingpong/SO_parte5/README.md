# Preempção por tempo
Implementação de um escalonador por preempção.
A ideia aqui é simular uma interrupção de hardware. A ideia do projeto é armar um timer que
por bibliotecas do C que simulará como uma interrupção de Hardware. 
Isso é feito com um timer (vide **timer.c**) e um tratados de sinal (vide **signal.c**).
## Descrição
A função que eu implemento aqui é **tratador()**. Eu implemento mais algumas na verdade,
porém é só para fins de modularização, ou inicialização (timer e tratador de alarme). A 
função **escalonador()** é modificada significativamente novamente. 
    * **tratador():** Trata quando uma interrupção de timer é feita.
    * **escalonador():** Ele volta a pegar a primeira tarefa da fila.
## timer.c e signal.c
São arquivos de inspiração. Uma ideia de como implementar um timer e um tratador de sinal.
Ambos foram feito por [Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).
## test_case
Contém a resposta esperada para a execução do programa **pingpong-preempcao.c**
Compare usando um diff. Tanto o programa **pingpong-preempcao.c**, quanto a resposta
esperada foi feita por [Carlos Maziero](http://wiki.inf.ufpr.br/maziero/doku.php?id=start).
    * **OBS:** Não é necessario ser exatamente igual, visto que as funções podem parar em
    lugares diferentes em cada tarefa (por causa do tempo), pode haver variações.
###### "That's all folks!!!"