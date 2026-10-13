#ifndef TREAP_H
#define TREAP_H

#include <stdbool.h>

typedef struct treap *Treap;

/* Retorna NULL se a alocação falhou da árvore falhou, caso contrário retorna um ponteiro
para a árvore */
Treap cria_arvore ();

// Busca um valor na árvore. Retorna true se encontrar, false se não.
bool busca (Treap arv, int valor);

// Insere um no com "valor" na árvore. Retorna true se conseguiu, false se não.
bool insere (Treap arv, int valor);

#endif