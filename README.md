# Butterfly Barrier Reduce Project

## Membros
- Leandro Aguiar Mota
- Mateus Rosolem Baroni
- Yhan de Brito Pena

## 🖋️ Sobre  
Este projeto implementa uma operação de **redução** utilizando a técnica de **barreira borboleta**, simulada em um ambiente distribuído com comunicação via **sockets**. O objetivo é aplicar a operação de soma a números gerados por oito processos, que se comunicam entre si em fases, até que o resultado final seja enviado ao gerente.

## 🎓 Um projeto de universidade  
Este trabalho é parte do curso de **Programação Concorrente e Paralela**, que requer a implementação de uma operação de redução de soma utilizando sockets, em vez da tradicional biblioteca MPI.

## 📖 Introdução  
A **função MPI Reduce** é usada em programas paralelos para realizar operações de redução (soma, produto, máximo, mínimo, etc.) sobre dados distribuídos entre múltiplos processos. Neste projeto, aplicamos o conceito de **barreira borboleta** para reduzir a comunicação centralizada entre os processos trabalhadores e o gerente.

Em vez de cada processo trabalhador enviar seu resultado diretamente ao gerente, os trabalhadores se comunicam entre si em fases. O número de comunicações em cada fase é reduzido pela metade, até que um único processo envie o resultado final ao gerente.

### Fases de Comunicação:
- **Fase 1**: Trabalhos em pares (W1 com W2, W3 com W4, W5 com W6, W7 com W8).
- **Fase 2**: A soma de W7 e W8 é enviada para W5, e a soma de W3 e W4 para W1.
- **Fase 3**: W1 e W5 se comunicam para gerar o resultado final que é enviado ao gerente.

Essa abordagem reduz o número de comunicações e melhora a eficiência.

## 🛠️ Compilação  
O projeto deve ser compilado e executado em uma única máquina, utilizando **sockets** para a comunicação entre os processos. Abaixo estão as instruções gerais para compilação:

1. Primeiro, é necessário que o **gerente** e os **trabalhadores** sejam compilados separadamente. Utilize um compilador C de sua escolha para compilar o código-fonte de ambos.
2. O processo do gerente deve ser compilado para gerenciar a comunicação e consolidação dos resultados. 
3. Cada trabalhador (W1 a W8) deve ser compilado com seu respectivo código, garantindo que estejam preparados para as comunicações em múltiplas fases da barreira borboleta.

> **Nota**: É importante garantir que as portas usadas para a comunicação entre o gerente e os trabalhadores estejam corretamente definidas.

## 🚀 Execução  
Para executar o projeto, siga as etapas abaixo:

Certifique de ter o gcc instalado

Depois utilize o comando
``` bash app.sh````


1. **Iniciar o Gerente**:  
   O gerente será responsável por aguardar a soma final e exibi-la após todas as fases de comunicação entre os trabalhadores.

2. **Iniciar os Trabalhadores**:  
   Cada trabalhador (W1 a W8) será iniciado separadamente, e cada um deles seguirá a lógica de comunicação especificada nas fases da barreira borboleta.

Os trabalhadores seguirão a lógica de comunicação descrita nas fases, até que o trabalhador final envie o resultado ao gerente.

### Comunicação entre processos
Cada trabalhador e o gerente se comunicam via sockets em portas conhecidas:

- Gerente utiliza uma porta específica para receber os resultados.
- Trabalhadores têm suas portas designadas para envio e recebimento de resultados durante as fases.

A troca de mensagens segue a estrutura da barreira borboleta, conforme ilustrado abaixo:

### Exemplo de Comunicação:
- Na **Fase 1**, os trabalhadores realizam somas em pares: W2 com W1, W4 com W3, W6 com W5, e W8 com W7.
- Na **Fase 2**, os resultados são enviados para novos pares: W7 envia sua soma para W5, e W3 envia sua soma para W1.
- Na **Fase 3**, o trabalhador W5 envia sua soma para W1, que finalmente envia o resultado ao gerente.

Após isso, o gerente exibirá o resultado final da soma, e o projeto estará completo!

A ideia é que o sistema seja simples de configurar, e a execução demonstre a eficiência de comunicação paralela usando sockets.

**Divirta-se explorando os conceitos de paralelismo e comunicação distribuída!**




