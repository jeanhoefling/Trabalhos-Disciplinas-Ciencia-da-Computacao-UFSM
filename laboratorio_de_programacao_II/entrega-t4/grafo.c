// funções que implementam as operações básicas de um grafo
//
// um grafo é constituido por nós e arestas que os interligam.
// existe um valor associado a cada nó e a cada aresta. Esses valores são
//   armazenados pelo grafo, mas o grafo desconhece seu tipo, só conhece
//   seu tamanho.
// os nós são identificados por um inteiro que corresponde à ordem em que
//   são inseridos no grafo, com 0 correspondendo ao primeiro.
// a identificação de um nó é sempre um número inferior ao número de nós
//   no grafo. A remoção de um nó altera a identificação dos nós com números
//   superiores.
// as arestas são direcionadas, e são identificadas pelo seu nó de origem
//   e de destino.
// os valores das arestas e dos nós são copiados para o grafo. O acesso a
//   esses valores é por meio de ponteiros. O usuário do grafo pode alterar
//   o valor armazenado no grafo após obter o ponteiro (com g_valor_do_nó
//   por exemplo). O usuário pode também alterar o valor usando uma função
//   de alteração (como g_altera_valor_do_nó).

#include "grafo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define AUMENTA 3
#define DIMINUI 4

typedef struct aresta {
  int origem;
  int destino;
  void *valor;
  struct aresta *prox_entrada;
  struct aresta *prox_saida;
} Aresta;

typedef struct no {
  void *dado;
  Aresta *entradas;
  Aresta *saidas;
} No;


struct grafo {
  No *nos;
  int num_nos;
  int tam_no;
  int tam_aresta;
  int tam_alocado;

  // Para realizar consultas
  Aresta *atual;
  int no_consultado;
  bool consulta_saidas; // True se estiver consultando saidas, false se consultando entradas
};

Grafo g_cria(int tam_nó, int tam_aresta) {
  Grafo g = (Grafo) malloc(sizeof(struct grafo));
  assert(g != NULL);
  g->nos = NULL;
  g->num_nos = 0;
  g->tam_no = tam_nó;
  g->tam_aresta = tam_aresta;
  g->tam_alocado = 0;
  g->atual = NULL;
  g->no_consultado = -1;
  g->consulta_saidas = false;
  return g;
}

// libera a memória ocupada por um grafo
void g_destrói(Grafo self);

// Nós

/* JEAN: Aloca memória para o vetor de nós do grafo conforme necessario (min 8)
O parametro op diz se deve aumentar a alocação ou diminuir*/
static void realoca (Grafo self, char op) {
  if (op == AUMENTA) {
    if (self->tam_alocado == 0) {
      self->nos = malloc(sizeof(No) * 8);
      assert(self->nos != NULL);
      self->tam_alocado = 8;
    }
    else {
      No *temp = realloc(self->nos, sizeof(No) * self->tam_alocado * 2);
      assert(temp != NULL);
      self->nos = temp;
      self->tam_alocado = self->tam_alocado * 2;
    }
  }
  else {
    if (self->num_nos == 0) {
      free(self->nos);
      self->nos = NULL;
      self->tam_alocado = 0;
    }
    else {
      No *temp = realloc(self->nos, sizeof(No) * self->tam_alocado / 2);
      assert(temp != NULL);
      self->nos = temp;
      self->tam_alocado = self->tam_alocado / 2;
    }
  }
}

int g_insere_nó(Grafo self, void *p_dado) {
  if (self->num_nos >= self->tam_alocado) realoca(self, AUMENTA);

  No *no = &self->nos[self->num_nos];

  no->dado = malloc(self->tam_no);
  assert(no->dado != NULL);
  memcpy(no->dado, p_dado, self->tam_no);
  no->entradas = NULL;
  no->saidas = NULL;

  self->num_nos++;
  return self->num_nos - 1;
}

/* JEAN: Função para liberar arestas conectadas a um nó e ajustar o encadeamento
das 2 listas */
static void libera_aresta(Grafo self, No *remov, int nó) {
  for (int i = 0; i < self->num_nos; i++) {
    No *atual = &self->nos[i];
    if (atual == remov) continue;
    Aresta **a = &atual->entradas;
    while ((*a) != NULL) {
      if ((*a)->origem == nó || (*a)->destino == nó) {
        *a = (*a)->prox_entrada;
      } else {
        a = &(*a)->prox_entrada;
      }
    }
  }

  for (int i = 0; i < self->num_nos; i++) {
    No *atual = &self->nos[i];
    if (atual == remov) continue;
    Aresta **a = &atual->saidas;
    while ((*a) != NULL) {
      if ((*a)->origem == nó || (*a)->destino == nó) {
        Aresta *temp = *a;
        *a = temp->prox_saida;
        free(temp);
      } else {
        a = &(*a)->prox_saida;
      }
    }
  }
}
// remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca
//   tivesse existido
void g_remove_nó(Grafo self, int nó) {
  if (nó < 0 || nó >= self->num_nos) return;
  
  No *remov = &self->nos[nó]; 

  libera_aresta(self, remov, nó);

  free(remov->dado);

  for (int i = nó; i < self->num_nos - 1; i++) {
    self->nos[i] = self->nos[i+1];
  }

  self->num_nos--;
  if (self->num_nos <= self->tam_alocado / 2 && self->num_nos >= 8
  || self->num_nos == 0) realoca(self, DIMINUI);
}

// altera o valor associado a um nó (copia o valor apontado por p_dado para
//   uma memória gerenciada pelo grafo)
void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado) {
  if (nó < 0 || nó > self->num_nos - 1) return;
  No *no = &self->nos[nó];
  memcpy(no->dado, p_dado, self->tam_no);
}

// retorna um ponteiro para o valor associado a um nó que está na memória
//   gerenciada pelo grafo.
void *g_valor_do_nó(Grafo self, int nó) {
  if (nó < 0 || nó > self->num_nos - 1) return NULL;
  No no = self->nos[nó];
  return no.dado;
}

// retorna o número de nós do grafo
int g_num_nós(Grafo self) {
  return self->num_nos;
}

// Arestas

// altera o valor da aresta que interliga o nó origem ao nó destino
//   (copia de *p_dado para uma memória gerenciada pelo grafo)
// caso a aresta não exista, deve ser criada
// caso p_dado seja NULL, a aresta deve ser removida

static void remove_aresta (Grafo self, int nó_origem, int nó_destino) {
  No *origem = &self->nos[nó_origem];
  No *destino = &self->nos[nó_destino];

  Aresta **a = &origem->saidas;
  while ((*a)->destino != nó_destino) {
    a = &(*a)->prox_saida;
  }
  Aresta *removida = *a;
  *a = (*a)->prox_saida;

  a = &destino->entradas;
  while (*a != removida) {
    a = &(*a)->prox_entrada;
  }
  *a = (*a)->prox_entrada;

  free(removida->valor);
  free(removida);
}

static void cria_aresta (Grafo self, int nó_origem, int nó_destino, void *p_dado) {
  No *origem = &self->nos[nó_origem];
  Aresta **a = &origem->saidas;
  while ((*a) != NULL) {
    a = &(*a)->prox_saida;
  }
  Aresta *nova = malloc(sizeof(Aresta));
  nova->origem = nó_origem;
  nova->destino = nó_destino;
  nova->prox_saida = NULL;
  nova->prox_entrada = NULL;
  nova->valor = malloc(self->tam_aresta);
  memcpy(nova->valor, p_dado, self->tam_aresta);

  *a = nova;
}

static bool existe_aresta (Grafo self, int nó_origem, int nó_destino) {
  No *origem = &self->nos[nó_origem];
  Aresta *a = origem->saidas;
  while (a != NULL && a->destino != nó_destino) {
    a = a->prox_saida;
  }
  return (a != NULL);
}

void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino, void *p_dado) {
  bool existe = existe_aresta(self, nó_origem, nó_destino);
  if(!existe && p_dado == NULL) return; //não há aresta para remover

  if (!existe && p_dado != NULL) {
    cria_aresta(self, nó_origem, nó_destino, p_dado);
    return;
  }

  if (existe && p_dado == NULL) {
    remove_aresta(self, nó_origem, nó_destino);
    return;
  }

  No *origem = &self->nos[nó_origem];
  Aresta *a = origem->saidas;
  while (a->destino != nó_destino) {
    a = a->prox_saida;
  }
  memcpy(a->valor, p_dado, self->tam_aresta);
}

// retorna um ponteiro para o valor associado à aresta entre nó_origem e 
//   nó_destino, localizado em memória gerenciada pelo grafo.
// retorna NULL se tal aresta não existir
void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino) {
  No *origem = &self->nos[nó_origem];
  Aresta *a = origem->saidas;
  while (a != NULL && a->destino != nó_destino) {
    a = a->prox_saida;
  }
  if (a == NULL) return NULL;
  return a->valor;
}

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que parte desse nó
void g_arestas_que_partem(Grafo self, int nó_origem) {
  self->atual = self->nos[nó_origem].saidas;
  self->no_consultado = nó_origem;
  self->consulta_saidas = true;
}

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que chega nesse nó
void g_arestas_que_chegam(Grafo self, int nó_destino) {
  self->atual = self->nos[nó_destino].entradas;
  self->no_consultado = nó_destino;
  self->consulta_saidas = false;
}

// retorna um ponteiro para o dado associado à próxima aresta, de acordo
//   com a última consulta iniciada por 'g_arestas_que_partem' ou 
//   'g_arestas_que_chegam'
// o valor do nó vizinho ao nó da consulta deve ser colocado em '*p_nó_vizinho'
//   (se não for NULL)
// caso não exista mais aresta que satisfaça a consulta, retorna NULL.
void *g_próxima_aresta(Grafo self, int *p_nó_vizinho) {
  Aresta *a = self->atual;

  if (a == NULL) return NULL;

  if (p_nó_vizinho != NULL) {
    if (self->consulta_saidas) *p_nó_vizinho = a->destino;
    else *p_nó_vizinho = a->origem;
  }

  if (self->consulta_saidas) self->atual = a->prox_saida;
  else self->atual = a->prox_entrada;

  return a->valor;
}

// Algoritmos

// retorna true se grafo é cíclico, false caso contrário

static bool rec_ciclo (Grafo self, int *estados, int no) {
  Aresta *a = self->nos[no].saidas;
  estados[no] = 1;
  while (a != NULL) {
    if (estados[a->destino] == 1) return true;
    if (estados[a->destino] == 0) {
        if (rec_ciclo(self, estados, a->destino)) return true;
    }
    a = a->prox_saida;
  }
  estados[no] = 2;
  return false;
}


bool g_tem_ciclo(Grafo self) {
  if (self->num_nos == 0) return false;

  int *estados = malloc(sizeof(int) * self->num_nos);
  for (int i = 0; i < self->num_nos; i++) {
    estados[i] = 0;
  }
  bool ciclo = 0;
  for (int i = 0; i < self->num_nos; i++) {
    if (estados[i] == 2) continue;
    ciclo = rec_ciclo(self, estados, i);  
    if (ciclo) break;
  }

  free(estados);
  return ciclo;
}

static void calcula_graus_entrada (Grafo self, int *v) {
  for (int i = 0; i < self->num_nos; i++) {
    Aresta *a = self->nos[i].entradas;
    int cont = 0;
    while (a != NULL) {
      a = a->prox_entrada;
      cont++;
    }
    v[i] = cont;
  }
}

static void diminui_graus_entrada(Grafo self, int *v, int i) {
  Aresta *a = self->nos[i].saidas;
  while (a != NULL) {
    v[a->destino]--;
    a = a->prox_saida;
  }
}

// retorna uma fila de inteiros contendo os números dos nós do grafo em
//   uma ordem em que, se o nó 'a' antecede 'b', não existe uma aresta
//   de 'b' para 'a' no grafo
// deve retornar uma fila vazia caso tal ordem não exista
// quem chama esta função é responsável por destruir a fila.
Fila g_ordem_topológica(Grafo self) {
  Fila f = f_cria(4); // 4 bytes -> fila de inteiros
  if (g_tem_ciclo(self) || self->num_nos == 0) return f;

  int graus_entrada[self->num_nos];
  calcula_graus_entrada(self, graus_entrada);

  int cont = 0;
  while (cont < self->num_nos) {
    for (int i = 0; i < self->num_nos; i++) {
      if (graus_entrada[i] == 0) {
        int *p = malloc(sizeof(int));
        *p = i;
        f_insere(f, p);
        diminui_graus_entrada(self, graus_entrada, i);
        graus_entrada[i] = -1;
        break;
      }
    }
    cont++;
  }
  return f;
}