Projeto desenvolvido por T5G5:
    Francisco Gonçalves
    Miguel Pinto
    Tiago Alves

Para a segunda etapa deste trabalho prentendeu-se desenvolver um servidor com um número limitado de lugares e de threads disponíveis.

De modo a resolver o problema dos lugares limitados, decidiu-se implementar uma fila de espera (struct Queue) onde o elemento mais à frente é o lugar que está mais livre há mais tempo, apenas adicionando na fila o indíce do lugar quando este se encontra desocupado. Assim sendo, sempre que o servidor recebe um pedido do FIFO público, cria uma nova thread para o responder, atribuindo-lhe como lugar o elemento que se encontra na frente da fila. Todas as operações realizadas à queue são realizadas de forma segura, usando um mutex para aceder à struct e um semáforo para garantir que podem aceder à struct apenas quando existem lugares disponíveis. De modo a resolver o problema das threads limitadas, decidiu-se implementar um semáforo: threads novas são criadas apenas quando estão a ser utilizadas menos threads do que o limite imposto.

De um modo semelhante, na parte do cliente é criada uma thread por cada pedido a ser enviado. O número sequencial do pedido é acedido e incrementado também de forma segura, por uso de um mutex. Como referido no enunciado "após o tempo estipulado para o funcionamento do programa (nsecs), ou após o encerramento do Quarto de Banho, o ciclo de geração de pedidos é terminado".

Com o objetivo de poupar recursos, optou-se pela utilização, por parte do servidor, do FIFO público em modo bloqueante. Assim, sempre que é aberto o FIFO, este bloqueia até que um cliente abra para escrita. Note-se que, devido a tal, surge o "edge case" em que nenhum cliente se liga, ficando o servidor indefinidamente bloqueado até que o programa seja interrompido por um SIGINT, por exemplo. Para resolver este problema, implementou-se um alarme (lançamento do sinal SIGALRM por uso da função alarm), de modo a interromper a chamada à função open. Segundo a página man de signal(7): "A process-directed signal may be delivered to any one of the threads that does not currently have the signal blocked. If more than one of the threads has the signal unblocked, then the kernel chooses an arbitrary thread to which to deliver the signal." Portanto, não é garantido que o sinal seja colocado na thread principal: se for colocado numa das threads de atendimento, poderia interromper uma das chamadas às funções write, fazendo com que certos clientes não recebessem resposta ao pedido. Para resolver este caso, todas as threads secundárias bloqueiam SIGALRM por uso da função pthread_sigmask, logo o sinal é obrigatoriamente recebido pela thread principal, que lida também com o caso de errno igual a EINTR na leitura do FIFO principal.

Apesar de ser um caso raro, de modo a lidar com os casos em que o cliente deixa de estar disponível e mesmo assim é efetuada uma escrita no seu FIFO privado, optou-se por lidar com o sinal SIGPIPE gerado, ignorando-o. Neste caso, errno é igual a EPIPE, que é lidado também pelas chamadas de write a FIFOs.

De modo a não atingir o máximo numero de threads imposta pelo sistema, quando não se coloca a opção -n, é atríbuido um valor máximo de 256 Threads. Caso o utilizado pretenda que este número seja ilimitado, deve colocar a flag -n a 0.

Importa também realçar que a transmissão de dados entre servidor e cliente é feita em modo binário, por uso de struct, de modo a tornar o programa mais eficiente: como mencionado pelo professor, "usar estruturas de dados para converter em texto e depois efectuar o processo inverso está longe de ser uma solução simples e eficiente".

Para correr o programa, execute o comando make na raiz do projeto, sendo gerados dois executáveis, Q2 e U2, sendo que para correr cada um deles deve-se dar os seguintes argumentos pela ordem que o utilizador pretender. É ainda de referir que o nome do FIFO suporta caminhos relativos assim como caminhos absolutos.
	./Q2 <-t nsecs> [-l nplaces] [-n nthreads] fifoname
	./U2 <-t nsecs> fifoname



