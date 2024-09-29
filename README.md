# Membros
Leandro Aguiar Mota
Mateus Rosolem Baroni
Yhan de Brito Pena

Butterfly Barrier Reduce Project
🖋️ Sobre
Este projeto implementa uma operação de redução utilizando a técnica de barreira borboleta, simulada em um ambiente distribuído com comunicação via sockets. O objetivo é aplicar a operação de soma a números gerados por oito processos, que se comunicam entre si em fases, até que o resultado final seja enviado ao gerente.

🎓 Um projeto de universidade
Este trabalho é parte do curso de Computação Paralela e Distribuída, que requer a implementação de uma operação de redução de soma utilizando sockets, em vez da tradicional biblioteca MPI.

📖 Introdução
A função MPI Reduce é usada em programas paralelos para realizar operações de redução (soma, produto, máximo, mínimo, etc.) sobre dados distribuídos entre múltiplos processos. Neste projeto, aplicamos o conceito de barreira borboleta para reduzir a comunicação centralizada entre os processos trabalhadores e o gerente.

Em vez de cada processo trabalhador enviar seu resultado diretamente ao gerente, os trabalhadores se comunicam entre si em fases. O número de comunicações em cada fase é reduzido pela metade, até que um único processo envie o resultado final ao gerente.

Fases de Comunicação:
Fase 1: Trabalhos em pares (W1 com W2, W3 com W4, W5 com W6, W7 com W8).
Fase 2: A soma de W7 e W8 é enviada para W5, e a soma de W3 e W4 para W1.
Fase 3: W1 e W5 se comunicam para gerar o resultado final que é enviado ao gerente.
Essa abordagem reduz o número de comunicações e melhora a eficiência.

🛠️ Compilação
O projeto deve ser compilado e executado em uma única máquina, utilizando sockets para a comunicação entre os processos. Abaixo estão as instruções de compilação:

1. Instalando o GCC
Primeiro, instale o GCC, o compilador C:

bash
Copiar código
sudo apt install gcc
🔔 Nota: Se já possuir o GCC instalado, pule para o próximo passo.

2. Instalando o Make
O make facilita a compilação. Para instalá-lo, execute:

bash
Copiar código
sudo apt install make
🔔 Nota: Se já possuir o Make instalado, pule para o próximo passo.

3. Compilando o projeto (usando Make)
Para compilar o projeto usando o make, execute o seguinte comando:

bash
Copiar código
make
4. Compilando o projeto (sem Make)
Se preferir compilar manualmente, use os seguintes comandos:

bash
Copiar código
gcc -o worker worker.c
gcc -o manager manager.c
🚀 Execução
Para executar o projeto, siga as etapas abaixo:

Iniciar o gerente:
No terminal, inicie o processo gerente, que aguardará o resultado final:

bash
Copiar código
./manager
Iniciar os trabalhadores:
Em terminais separados, inicie os trabalhadores (de W1 a W8) com seus respectivos IDs:

bash
Copiar código
./worker 1
./worker 2
./worker 3
./worker 4
./worker 5
./worker 6
./worker 7
./worker 8
Os trabalhadores seguirão a lógica de comunicação descrita nas fases, até que o trabalhador final envie o resultado ao gerente.

Comunicação entre processos
Cada trabalhador e o gerente se comunicam via sockets em portas conhecidas:

c
Copiar código
#define PORT_MANAGER 8080
#define PORT_WORKER1 8081
#define PORT_WORKER2 8082
#define PORT_WORKER3 8083
#define PORT_WORKER4 8084
#define PORT_WORKER5 8085
#define PORT_WORKER6 8086
#define PORT_WORKER7 8087
#define PORT_WORKER8 8088
A troca de mensagens segue a estrutura da barreira borboleta, conforme ilustrado abaixo:

Exemplo de Comunicação:
Na Fase 1, W2 envia seu valor para W1, W4 para W3, W6 para W5, e W8 para W7.
Na Fase 2, W7 envia sua soma para W5, e W3 envia sua soma para W1.
Na Fase 3, W5 envia sua soma para W1, que envia o resultado final ao gerente.
Após isso, o gerente exibirá o resultado final da soma, e o projeto estará completo!

Divirta-se com a execução e explore os conceitos de paralelismo e comunicação distribuída!