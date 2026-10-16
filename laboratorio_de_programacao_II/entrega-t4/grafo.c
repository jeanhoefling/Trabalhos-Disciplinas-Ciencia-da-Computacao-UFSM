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

#define SAIDA 1
#define ENTRADA 2

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
};

Grafo g_cria(int tam_nó, int tam_aresta) {
  Grafo g = (Grafo) malloc(sizeof(struct grafo));
  assert(g != NULL);
  g->nos = NULL;
  g->num_nos = 0;
  g->tam_no = tam_nó;
  g->tam_aresta = tam_aresta;
  g->tam_alocado = 0;
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
      self->nos = NULL
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

/* JEAN: Função para liberar memória de uma aresta e ajustar o encadeamento das 2 listas
Se ajuste == SAIDA, significa que precisa ajustar o encadeamento das arestas de saida
Se ajuste == ENTRADA, significa que precisa ajustar o encadeamento das arestas de entrada*/
static void libera_aresta(Grafo self, Aresta *a, char ajuste) {
  if (ajuste == SAIDA) {
    Aresta **at = &self->nos[a->origem].saidas;
    while (*at != NULL && *at != a) {
      at = &(*at)->prox_saida;
    }
    if (*at != NULL) {
        *at = a->prox_saida;
    }
  } 
  else {
    Aresta **at = &self->nos[a->destino].entradas;
    while (*at != a && *at != NULL) {
      at = &(*at)->prox_entrada;
    }
    if (*at != NULL) {
      *at = a->prox_entrada;
    }
  }

  free(a->valor);
  free(a);
}
// remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca
//   tivesse existido
void g_remove_nó(Grafo self, int nó) {
  if (nó < 0 || nó >= self->num_nos) return;
  
  No *remov = &self->nos[nó]; 

  Aresta *a = remov->entradas 
  while (a != NULL) {
    Aresta *temp = a;
    a = a->prox_entrada;
    libera_aresta(self, temp, SAIDA);
  }
  a = remov->saidas
  while (a != NULL) {
    Aresta *temp = a;
    a = a->prox_saida;
    libera_aresta(self, temp, ENTRADA);
  }

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
void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado);

// retorna um ponteiro para o valor associado a um nó que está na memória
//   gerenciada pelo grafo.
void *g_valor_do_nó(Grafo self, int nó);

// retorna o número de nós do grafo
int g_num_nós(Grafo self);

// Arestas

// altera o valor da aresta que interliga o nó origem ao nó destino
//   (copia de *p_dado para uma memória gerenciada pelo grafo)
// caso a aresta não exista, deve ser criada
// caso p_dado seja NULL, a aresta deve ser removida
void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino,
                              void *p_dado);

// retorna um ponteiro para o valor associado à aresta entre nó_origem e 
//   nó_destino, localizado em memória gerenciada pelo grafo.
// retorna NULL se tal aresta não existir
void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino);

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que parte desse nó
void g_arestas_que_partem(Grafo self, int nó_origem);

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que chega nesse nó
void g_arestas_que_chegam(Grafo self, int nó_destino);

// retorna um ponteiro para o dado associado à próxima aresta, de acordo
//   com a última consulta iniciada por 'g_arestas_que_partem' ou 
//   'g_arestas_que_chegam'
// o valor do nó vizinho ao nó da consulta deve ser colocado em '*p_nó_vizinho'
//   (se não for NULL)
// caso não exista mais aresta que satisfaça a consulta, retorna NULL.
void *g_próxima_aresta(Grafo self, int *p_nó_vizinho);

// Algoritmos

// retorna true se grafo é cíclico, false caso contrário
bool g_tem_ciclo(Grafo self);

// retorna uma fila de inteiros contendo os números dos nós do grafo em
//   uma ordem em que, se o nó 'a' antecede 'b', não existe uma aresta
//   de 'b' para 'a' no grafo
// deve retornar uma fila vazia caso tal ordem não exista
// quem chama esta função é responsável por destruir a fila.
Fila g_ordem_topológica(Grafo self);