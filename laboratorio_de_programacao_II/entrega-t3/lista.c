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


Lista l_cria_separando(Str s, Str sep) {
  Lista l = l_cria();
  int pos = s_busca_nc(s, 0, sep);
  dado_t subs;
  while (pos != -1 && s_busca_c(s, pos, sep) != -1) {
    int possep = s_busca_c(s, pos, sep);
    subs = s_cria_substring(s, pos, possep - pos);
    l_insere_fim(l, subs);
    pos = s_busca_nc(s, possep, sep);
  }
  // if trata o caso de ter um sep no ultimo caracter de s;
  if (pos < s_tam(s) && pos != -1) {
    l_insere_fim(l, s_cria_substring(s, pos, s_tam(s) - pos));
  }
  return l;
}

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


bool l_cheia(Lista l) {
  return false;
}


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


dado_t l_remove_inicio(Lista l) {
  assert(!(l->tam == 0));
  No *temp = l->sentinela->prox;
  dado_t d = temp->dado;


  l->sentinela->prox = temp->prox;
  (temp->prox)->ant = l->sentinela;

  free(temp);
  l->tam--;
  return d;
}


dado_t l_remove_fim(Lista l) {
  assert(!(l->tam == 0));
  No *temp = l->sentinela->ant;
  dado_t d = temp->dado;


  l->sentinela->ant = temp->ant;
  (temp->ant)->prox = l->sentinela;

  free(temp);
  l->tam--;
  return d;
}


dado_t l_remove_pos(Lista l, int pos) {
  assert(!(l->tam == 0));
  if (pos <= 0) {
    return l_remove_inicio(l);
  } else if (pos >= l->tam) {
    return l_remove_fim(l);
  }

  No *temp = l->sentinela->prox;
  for (int i = 0; i < pos; i++) {
    temp = temp->prox;
  }
  dado_t d = temp->dado;

  (temp->prox)->ant = temp->ant;
  (temp->ant)->prox = temp->prox;

  free(temp);
  l->tam--;
  return d;
}


// funções para usar a lista como uma fila

dado_t l_primeiro(Lista l) {
    return l_dado_inicio(l);
}


void l_insere(Lista l, dado_t d) {
    l_insere_fim(l, d);
    return;
}


dado_t l_remove(Lista l) {
    return l_remove_inicio(l);
}


// funções para usar a lista como uma pilha

dado_t l_topo(Lista l) {
    return l_dado_fim(l);
}


void l_empilha(Lista l, dado_t d) {
    l_insere_fim(l, d);
    return;
}


dado_t l_desempilha(Lista l) {
    return l_remove_fim(l);
}