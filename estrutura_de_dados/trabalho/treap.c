#include "treap.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct no {
    struct no *esq;
    struct no *dir;

    int valor;
    int prio;
} No;

struct treap {
    No *raiz;
    int elementos;
};

// Retorna NULL se a alocação falhou da árvore falhou, caso contrário retorna um ponteiro
// para a árvore
Treap cria_arvore () {
    Treap arv = malloc(sizeof(struct treap));
    if (arv == NULL) {
        printf("Alocação de árvore falhou\n");
        return NULL;
    }

    arv->raiz = NULL;
    arv->elementos = 0;

    return arv;
}

static No* cria_no (int valor) {
    No *no = malloc(sizeof(No));
    if (no == NULL) {
        printf("Alocação de nó falhou\n");
        return NULL;
    }

    no->esq = NULL;
    no->dir = NULL;
    no->valor = valor;
    no->prio = rand();
    return no;
}

/* Retorna NULL se o valor não está na arvóre, se estiver retorna um ponteiro 
pro nó com o valor desejado. Deve receber o nó raiz da arvore (ou subárvore) */
static No* busca_no (No *no, int valor) {
    if (no == NULL) return NULL;
    if (no->valor == valor) return no;
    else if (no->valor > valor) return busca_no(no->esq, valor);
    else return busca_no(no->dir, valor);
}

bool busca (Treap arv, int valor) {
    if (arv == NULL) return false;
    return (busca_no(arv->raiz, valor) != NULL);
}


bool insere (Treap arv, int valor) {
    // Caso seja o primeiro nó a ser inserido
    if (arv->elementos == 0) {
        No *novo = cria_no(valor);
        if (novo == NULL) return false;
        arv->raiz = novo;
        arv->elementos++;
        return true;
    }


    insere_no(arv->raiz, NULL, valor);
    if (elemen)

    arv->elementos++;
    return true;
}

bool insere_no (No *no, No* pai, int valor) {
    if (no == NULL) { // Chegou na folha
        No *novo = cria_no(valor);
        if (novo == NULL) return false;

        if (pai->valor > valor) {
            pai->esq = novo;
        } else {
            pai->dir = novo;
        }
        return true;
    }
    
    if (no->valor == valor) {
        printf("Não é permitido adicionar valores duplicados na árvore.\n")
        return false;
    }


    else if (no->valor > valor) return insere_no(no->esq, no, valor);
    else return insere_no_no(no->dir, no, valor);
}