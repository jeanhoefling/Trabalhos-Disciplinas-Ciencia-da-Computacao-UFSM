#ifndef TREAP_H
#define TREAP_H

#include <stdbool.h>

typedef struct treap *Treap;

/* Retorna NULL se a alocação falhou da árvore falhou, caso contrário retorna um ponteiro
para a árvore */
Treap cria_treap ();

// Busca um valor na árvore. Retorna true se encontrar, false se não.
bool busca_treap (Treap arv, int valor);

// Insere um no com "valor" na árvore. Retorna true se conseguiu, false se não.
bool insere_treap (Treap arv, int valor);

// Remove um no com "valor" na árvore. Retorna true se conseguiu, false se não.
bool remove_treap (Treap arv, int valor);

// Imprime a árvore com tabulações referentes a altura de cada nó
void imprime (Treap arv);

// Destrói completamente a árvore (libera todos os nós e a memória da struct treap)
void destroi_treap (Treap arv);

/* Essa função foi feita por IA. Por não se tratar de algo exigido no trabalho
acredito que não tenha nenhum problema usar IA para esta funcionalidade, meu objetivo
com essa função é somente facilitar a visualização da treap durante a apresentação,
para poder mostrar a aleatoriedade. Uma função de impressão mais simples é a imprime,
feita por mim, que também está disponível no TAD. */
void imprime_ia (Treap arv);

#endif