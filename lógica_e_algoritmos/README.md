# Jogo de Labirinto em Linguagem C

Projeto desenvolvido para a disciplina de **Lógica e Algoritmos / Laboratório de Programação I**, utilizando a linguagem C e execução em terminal.

---

# Sobre o Projeto

O objetivo do trabalho foi desenvolver um jogo simples baseado em uma matriz bidimensional, aplicando conceitos fundamentais de programação estudados durante a disciplina.

O foco principal do projeto não era criar um jogo complexo visualmente, mas sim praticar:

* matrizes;
* funções;
* condicionais;
* laços de repetição;
* structs;
* ponteiros;
* controle de estado;
* organização lógica do código.

O jogo acontece inteiramente no terminal e possui movimentação automática para o jogador e para o inimigo.

---

# Código Base Fornecido Pelo Professor

O professor disponibilizou um código inicial contendo:

* estrutura básica do tabuleiro;
* impressão da matriz no terminal;
* definição dos elementos do jogo;
* renderização utilizando Unicode;
* um exemplo simples de movimentação automática.

O código base já possuía:

* criação da matriz do mapa;
* paredes e obstáculos;
* posicionamento inicial do jogador e inimigo;
* função de impressão da matriz;
* limpeza da tela;
* movimentação simples para a direita.

Trecho do comportamento originalmente fornecido:

```c
mat[jogadorLinha][jogadorColuna] = VAZIO;

jogadorColuna++;

mat[jogadorLinha][jogadorColuna] = JOGADOR;
```

Esse exemplo serviu apenas como ponto de partida para o desenvolvimento completo do jogo.

---

# Funcionalidades Implementadas

A partir do código base, foram implementadas diversas funcionalidades adicionais:

## Sistema completo de movimentação

* movimentação automática aleatória;
* movimentação do jogador;
* movimentação do inimigo;
* verificação de direção válida.

## Sistema de colisão

* bloqueio por paredes;
* bloqueio por obstáculos;
* colisão entre jogador e inimigo;
* detecção de saída;
* coleta de itens.

## Sistema de vidas

* jogador possui vidas;
* vidas podem ser recuperadas;
* derrota ao chegar em zero vidas.

## Sistema de chave e saída

* o jogador precisa coletar a chave;
* somente após isso a saída libera a vitória.

## Geração aleatória de itens

* vidas aparecem periodicamente;
* posições são geradas aleatoriamente;
* setas especiais são geradas aleatoriamente.

## Sistema de setas direcionais

* setas alteram automaticamente a direção do personagem;
* implementação recursiva da movimentação.

## Condições de vitória e derrota

* vitória ao pegar a chave e alcançar a saída;
* derrota ao perder todas as vidas;
* derrota ao atingir o limite máximo de rodadas.

---

# Objetivo do Jogo

O objetivo do jogador é:

1. sobreviver ao inimigo;
2. encontrar a chave escondida no mapa;
3. alcançar a saída do labirinto.

Durante o jogo:

* o inimigo se movimenta automaticamente;
* itens de vida aparecem no cenário;
* setas alteram o caminho do personagem;
* colisões influenciam diretamente no estado do jogo.

---

# Estrutura da Matriz

Toda a lógica do jogo foi construída utilizando uma matriz bidimensional:

```c
int mat[LINHAS][COLUNAS]
```

Cada número representa um elemento do cenário.

| Valor | Elemento          |
| ----- | ----------------- |
| 0     | Espaço vazio      |
| 1     | Jogador           |
| 2     | Inimigo           |
| 3     | Parede vertical   |
| 4     | Parede horizontal |
| 5     | Obstáculo         |
| 10    | Vida              |
| 11    | Chave             |
| 12    | Saída             |
| 13-16 | Setas direcionais |

---

# Tecnologias e Bibliotecas Utilizadas

## Linguagem

* C

## Bibliotecas

* `stdio.h`
* `stdlib.h`
* `time.h`
* `unistd.h`
* `wchar.h`
* `locale.h`

---

# Estrutura de Personagens

Foi utilizada uma struct para representar os personagens:

```c
typedef struct {
    char nome[10];
    int linha;
    int coluna;
} personagem;
```

Isso permitiu organizar:

* nome;
* posição;
* movimentação;
* manipulação dos personagens.

---

# Sistema de Movimentação

A movimentação foi implementada verificando diretamente os valores armazenados na matriz.

Antes de mover:

1. a próxima posição é analisada;
2. o conteúdo da célula é verificado;
3. o movimento é validado;
4. o estado da matriz é atualizado.

---

# Sistema de Colisão

As colisões utilizam diretamente os valores da matriz.

O programa verifica:

* paredes;
* obstáculos;
* inimigos;
* itens;
* saída;
* setas.

Dependendo do elemento encontrado:

* o personagem pode parar;
* perder vida;
* coletar item;
* mudar de direção;
* vencer o jogo.

---

# Sistema de Vida

O jogador inicia com:

```c
int vidas = 1;
```

Itens de vida aparecem aleatoriamente a cada 100 rodadas.

Ao coletar:

* a quantidade de vidas aumenta;
* o jogador consegue sobreviver por mais tempo.

---

# Sistema de Vitória

A saída só pode ser utilizada após a coleta da chave.

Fluxo de vitória:

1. encontrar a chave;
2. alcançar a saída;
3. finalizar o jogo com vitória.

---

# Aleatoriedade

O projeto utiliza geração aleatória para:

* movimentação dos personagens;
* geração de itens;
* geração da chave;
* geração das setas.

Foi utilizada:

```c
srand(time(NULL));
```

---

# Setas Direcionais

As setas especiais foram um dos principais elementos adicionados ao projeto.

Ao pisar em uma seta:

* o personagem automaticamente muda sua direção.

Tipos disponíveis:

* cima;
* baixo;
* esquerda;
* direita.

---

# Interface no Terminal

O jogo utiliza caracteres Unicode para desenhar:

* paredes;
* jogador;
* inimigo;
* itens;
* símbolos especiais.

Exemplos:

* ☻ → jogador
* ☹ → inimigo
* ♥ → vida

Para isso foram utilizadas:

```c
wprintf()
```

e:

```c
setlocale(LC_ALL, "");
```

---

# Principais Funções Desenvolvidas

## `move()`

Responsável por:

* validar movimentos;
* detectar colisões;
* coletar itens;
* controlar interação com setas;
* verificar vitória.

## `move_personagem()`

Controla a movimentação automática dos personagens.

## `gerar_posicao()`

Gera posições aleatórias válidas no mapa.

## `compara_string()`

Função criada manualmente para comparação de strings.

## `imprimeMatriz()`

Responsável por desenhar o estado atual do jogo.

---

# Como Executar

## Compilar

```bash
gcc jogo.c -o jogo
```

## Executar

```bash
./jogo
```

---

# Aprendizados

Durante o desenvolvimento deste projeto foram praticados:

* lógica de programação;
* manipulação de matrizes;
* structs;
* ponteiros;
* modularização com funções;
* controle de fluxo;
* atualização de estado;
* movimentação em grid;
* colisão;
* geração aleatória;
* renderização no terminal.

---

# 👨‍💻 Autor

Projeto desenvolvido por Jean Henrique de Souza Hoefling para fins acadêmicos na disciplina de Lógica e Algoritmos.
