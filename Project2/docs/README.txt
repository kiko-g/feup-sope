Projeto desenvolvido por T5G5:
    Francisco Gonçalves
    Miguel Pinto
    Tiago Alves

Para a primeira etapa, pretendeu-se desenvolver um servidor com número ilimitado de lugares e threads disponíveis. 
Logo, decidiu-se que a melhor estratégia a implementar seria aproveitar os recursos ilimitados: sempre que o servidor recebe um pedido pelo FIFO público, cria um nova thread para o responder, atribuindo-lhe o lugar disponível por via de um contador global (ou seja, o lugar dado será sempre superior a uma unidade ao valor que o último cliente recebeu). Cada thread acede e incrementa este valor de forma segura, usando um mutex.
De um modo semelhante, na parte do cliente é criada uma thread por cada pedido a ser enviado. O número sequencial do pedido é acedido e incrementado também de forma segura, por uso de um mutex. Como referido no enunciado "após o tempo estipulado para o funcionamento do programa (nsecs), ou após o encerramento do Quarto de Banho, o ciclo de geração de pedidos é terminado". Posto isto, quando uma thread do cliente recebe informação que o quarto de banho está fechado (mensagem com dur=-1 e pl=-1), uma variável global é modificada (de modo segurdo, também com um mutex), simbolizando que o quarto de banho encerrou e parando o ciclo de pedidos.
É de notar que o grupo testou o programa com vários tempos de espera entre pedidos: 50ms, 20ms, 10ms, 5ms e 2ms, com sucesso. O valor default deste tempo de espera é de 5ms, guardado na macro REQUEST_INTERVAL de U1.h.

Para correr o servidor, deve-se navegar até à pasta server, compilar com make e executar ./Q1 -t <nsecs> <fifoname>. Para correr o cliente, deve-se navegar até à pasta client, compilar com make e executar ./U1 -t <nsecs> <fifoname>. Em alternativa, podem-se utilizar os scripts de shell compile.sh, run_client.sh e run_server.sh.