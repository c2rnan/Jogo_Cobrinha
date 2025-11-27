<strong>🐍 Snake Terminal — Jogo da Cobrinha em C (Versão Avançada)</strong>

Jogo da Cobrinha desenvolvido inteiramente em linguagem C, executado no terminal do Windows.
Esta versão inclui níveis de dificuldade com 5 mapas diferentes, modo procedural, sistema de recordes, registro de trajeto, menu interativo e diversas melhorias avançadas.

<strong>🎮 Sobre o Jogo</strong>

O jogador controla a cobra utilizando as teclas de movimentação e deve comer 10 maçãs no menor tempo possível.
Ao finalizar o jogo, o tempo é comparado com os recordes e pode ser salvo.

O jogo contém:

5 mapas fixos carregados de arquivos externos

1 mapa aleatório gerado proceduralmente

Sistema de recordes usando arquivo binário recordes.txt

Sistema de trajeto gravado em trajeto_jogo.txt

Menu completo navegável

Detecção de colisão com:

paredes

obstáculos do mapa

corpo da cobra

<strong>🚀 Funcionalidades Principais</strong>
<strong>✔️ 5 Mapas de Dificuldade</strong>

Os arquivos externos:

mapa1.txt  
mapa2.txt  
mapa3.txt  
mapa4.txt  
mapa5.txt

<strong>✔️ Mapa Procedural (Dificuldade 6)</strong>

Gerado com algoritmo de "escavação aleatória".

<strong>✔️ Recordes (Top 5)</strong>

Armazenados em recordes.txt (binário)

Ordenados automaticamente

Substitui o pior tempo se necessário

Pede o nome do jogador ao bater recorde

<strong>✔️ Reexibição do Trajeto</strong>

Utiliza arquivo trajeto_jogo.txt para mostrar os passos feitos pelo jogador.

<strong>✔️ Menu Completo</strong>

Jogar

Último trajeto

Recordes

Dificuldade

Como jogar

Sair

<strong>🗂️ Estrutura do Projeto</strong>

ProjetoSnake/
│
├── main.c                 # Lógica principal, menu, recordes, jogo e mapas<br>
├── mapa1.txt              # Mapa 1 (Muito Fácil)<br>
├── mapa2.txt              # Mapa 2 (Fácil)<br>
├── mapa3.txt              # Mapa 3 (Médio)<br>
├── mapa4.txt              # Mapa 4 (Difícil)<br>
├── mapa5.txt              # Mapa 5 (Expert)<br>
│<br>
├── recordes.txt           # Arquivo binário de recordes<br>
├── trajeto_jogo.txt       # Último trajeto salvo<br>
│<br>
└── (executável gerado)<br>

<strong>▶️ Como Jogar</strong>

No Windows (MinGW):

| Tecla | Função   |
| ----- | -------- |
| W / ↑ | Cima     |
| S / ↓ | Baixo    |
| A / ← | Esquerda |
| D / → | Direita  |
| Q     | Sair     |

<strong>🧠 Conceitos de Programação Utilizados</strong>

Este projeto utiliza vários tópicos avançados de C:

<strong>🔸 Alocação Dinâmica</strong>

malloc() e free() para a cobra e para o trajeto.

<strong>🔸 Arquivos</strong>

.txt para mapas

.txt para trajeto

.bin para recordes

<strong>🔸 Structs</strong>

Posicao

Recorde

PilhaRecordes

<strong>🔸 Ordenação (Bubble Sort)</strong>

Usado para manter os recordes.

<strong>🔸 Manipulação de Cursor</strong>

SetConsoleCursorPosition()

Atualização em tempo real do terminal

<strong>🔸 Algoritmo Procedural</strong>

Gera mapas aleatórios com random walk.

<strong>🔸 Menus Interativos</strong>

Com movimentação usando setas (códigos 72, 80, 75, 77).

<strong>🎯 Objetivo do Trabalho</strong>

Esse projeto foi desenvolvido como trabalho acadêmico para reforçar conhecimentos em:

Lógica de Programação

Estruturas de Dados

Modularização

Arquivos

Alocação dinâmica

Interação com terminal

📩 Contato

Email: caiozin.r1227@gmail.com

GitHub: https://github.com/c2rnan
