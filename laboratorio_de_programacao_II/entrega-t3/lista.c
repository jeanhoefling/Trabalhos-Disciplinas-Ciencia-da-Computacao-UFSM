#include "lista.h"

#include <stdlib.h>
#include <assert.h>

struct no {
  dado_t dado;
  struct no *ant;
  struct no *prox;
};

typedef struct no No;

struct lista {
  No *sentinela;
  int tam;
};

Lista l_cria() {
  Lista l = malloc(sizeof(struct lista));
  assert(l != NULL);
  l->tam = 0;

  l->sentinela = malloc(sizeof(No));
  assert(l->sentinela != NULL);
  l->sentinela->prox = l->sentinela;
  l->sentinela->ant = l->sentinela;

  return l;
}

// cria uma lista contendo substrings de s
// as substrings são separadas por quaisquer caractere de sep
// os caracteres de sep não aparecem nas substrings
// exemplos:
//   "a,ba,ca, te", ", " -> ["a" "ba" "ca" "te"]
//   "aba \ncate\n", "\n" -> ["aba " "cate"]
Lista l_cria_separando(Str s, Str sep);

// libera a memória ocupada por uma lista
void l_destroi(Lista l) {
  No *p = l->sentinela->prox;
  while (p != l->sentinela) {
    p = p->prox;
    s_destroi(p->ant->dado);
    free(p->ant);
  }
  free(l->sentinela);
  free(l);
}


int l_tam(Lista l) {
  return l->tam;
}

// retorna true se a lista tiver cheia
bool l_cheia(Lista l);


bool l_vazia(Lista l) {
  if (l->tam == 0) {
    return true;
  } else {
    return false;
  }
}


void l_imprime(Lista l) {
  No *p = l->sentinela->prox;
  while (p != l->sentinela) {
    s_imprime(p->dado);
    p = p->prox;
  }
  return;
}


void l_insere_inicio(Lista l, dado_t d) {
  No *novo = malloc(sizeof(No));
  assert(novo != NULL);

  novo->dado = d;
  l->tam++;

  novo->prox = l->sentinela->prox;
  (novo->prox)->ant = novo;
  novo->ant = l->sentinela;
  l->sentinela->prox = novo;
}


void l_insere_fim(Lista l, dado_t d) {
  No *novo = malloc(sizeof(No));
  assert(novo != NULL);

  novo->dado = d;
  l->tam++;

  novo->ant = l->sentinela->ant;
  (novo->ant)->prox = novo;
  novo->prox = l->sentinela;
  l->sentinela->ant = novo;
}

// insere o dado d na lista l, de forma que ele fique na posição p
// a primeira posição é 0
void l_insere_pos(Lista l, dado_t d, int p) {
  if (p <= 0) {
    l_insere_inicio(l, d);
    return;
  } else if (p >= l->tam) {
    l_insere_fim(l, d);
    return;
  } 

  No *novo = malloc(sizeof(No));
  assert(novo != NULL);

  novo->dado = d;
  l->tam++;

  No *temp;

  temp = l->sentinela->prox;
  for (int i = 0; i < p; i++) {
    temp = temp->prox;
  }

  novo->ant = temp->ant;
  novo->prox = temp;
  (temp->ant)->prox = novo;
  temp->ant = novo;
}


dado_t l_dado_inicio(Lista l) {
  assert(l->tam != 0);
  return l->sentinela->prox->dado;
}


dado_t l_dado_fim(Lista l) {
  assert(l->tam != 0);
  return l->sentinela->ant->dado;
}


dado_t l_dado_pos(Lista l, int pos) {
  if (pos <= 0) {
    return l_dado_inicio(l);
  } else if (pos >= l->tam) {
    return l_dado_fim(l);
  } 

  No *temp;
  temp = l->sentinela->prox;
  for (int i = 0; i < pos; i++) {
    temp = temp->prox;
  }
  return temp->dado;
}

// remove e retorna o dado no início da lista
dado_t l_remove_inicio(Lista l);

// remove e retorna o dado no final da lista
dado_t l_remove_fim(Lista l);

// remove e retorna o dado na posição pos da lista
dado_t l_remove_pos(Lista l, int pos);


// funções para usar a lista como uma fila

// l_cria, l_destroi, l_vazia

// retorna o dado que está no início da fila
dado_t l_primeiro(Lista l);

// insere um dado no fim da fila
void l_insere(Lista l, dado_t d);

// remove e retorna o dado que está no início da fila
dado_t l_remove(Lista l);


// funções para usar a lista como uma pilha

// l_cria, l_destroi, l_vazia

// retorna o dado que está no topo da pilha
dado_t l_topo(Lista l);

// empilha um dado no topo da pilha
void l_empilha(Lista l, dado_t d);

// remove e retorna o dado que está no topo da pilha
dado_t l_desempilha(Lista l);