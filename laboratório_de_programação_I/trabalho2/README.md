# Quebra-Cabeça Deslizante (15 Puzzle)

## Descrição

Implementação do clássico jogo de peças deslizantes (15 Puzzle) em linguagem C.

O programa utiliza um vetor de 16 posições para representar um tabuleiro 4×4 contendo 15 peças e um espaço vazio. O objetivo do jogador é reorganizar as peças até que todas estejam na ordem correta.

Além da lógica de movimentação, o projeto conta com uma interface visual colorida no terminal, suporte às teclas WASD e setas do teclado e embaralhamento aleatório do tabuleiro.

## Funcionalidades

* Inicialização automática do tabuleiro.
* Embaralhamento aleatório das peças.
* Movimentação utilizando:

  * W, A, S, D
  * Teclas direcionais (setas)
* Verificação automática de vitória.
* Interface gráfica em terminal utilizando códigos ANSI.
* Limpeza e atualização da tela em tempo real.
* Controle do terminal em modo raw para leitura imediata das teclas.

## Conceitos Utilizados

* Vetores
* Funções
* Manipulação de matrizes através de índices
* Geração de números aleatórios
* Estruturas condicionais
* Laços de repetição
* Programação modular
* Controle de terminal ANSI
* Manipulação de entrada do usuário

## Estrutura do Projeto

Principais funções implementadas:

| Função                   | Descrição                                |
| ------------------------ | ---------------------------------------- |
| `preenche_tabuleiro()`   | Inicializa o tabuleiro em ordem          |
| `verifica_posicao()`     | Localiza uma peça no vetor               |
| `check_ordem()`          | Verifica se o jogo foi resolvido         |
| `esquerda()`             | Move a peça vazia para a esquerda        |
| `direita()`              | Move a peça vazia para a direita         |
| `acima()`                | Move a peça vazia para cima              |
| `abaixo()`               | Move a peça vazia para baixo             |
| `mov_aleatorio()`        | Executa uma movimentação aleatória       |
| `embaralha()`            | Mistura o tabuleiro                      |
| `mov_user()`             | Processa comandos do jogador             |
| `move_diff()`            | Move uma peça específica quando possível |
| `imprime_tabuleiro_v8()` | Renderiza o tabuleiro no terminal        |

## Como Compilar

Utilizando GCC:

```bash
gcc puzzle.c -o puzzle
```

## Como Executar

```bash
./puzzle
```

## Controles

| Tecla | Movimento    |
| ----- | ------------ |
| W     | Cima         |
| A     | Esquerda     |
| S     | Baixo        |
| D     | Direita      |
| Q     | Sair do jogo |

Também é possível utilizar as setas do teclado.

## Objetivo

Organizar todas as peças até que o tabuleiro retorne à configuração original:

```text
A B C D
E F G H
I J K L
M N O _
```

Onde o espaço vazio deve ocupar a última posição.

## Exemplo

Ao iniciar, o tabuleiro é embaralhado automaticamente:

```text
   ┌────────────────────────────┐
   │                            │
   │   A     F     D     H      │
   │                            │
   │   E     B           G      │
   │                            │
   │   I     J     K     L      │
   │                            │
   │   M     N     O     C      │
   │                            │
   └────────────────────────────┘
```

O jogador deve realizar movimentos até restaurar a ordem correta das peças.

## Autor

Projeto desenvolvido por Jean Henrique de Souza Hoefling para fins acadêmicos na disciplina de Laboratório de Programação I.
