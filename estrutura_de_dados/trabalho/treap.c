#include "treap.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define FILHO_ESQ 3
#define FILHO_DIR 4 

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
Treap cria_treap () {
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
    no->prio = rand()%200;
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

bool busca_treap (Treap arv, int valor) {
    if (arv == NULL) return false;
    return (busca_no(arv->raiz, valor) != NULL);
}

static void rotacao_direita(No **raiz) {
    No *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;
    *raiz = nova_raiz;
}

static void rotacao_esquerda(No **raiz) {
    No *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;
    *raiz = nova_raiz;
}


static bool insere_no (No **no, int valor) {
    if (*no == NULL) { // Chegou na folha
        No *novo = cria_no(valor);
        if (novo == NULL) return false;

        *no = novo;
        return true;
    }
    
    if ((*no)->valor == valor) {
        printf("Não é permitido adicionar valores duplicados na árvore.\n");
        return false;
    }


    else if ((*no)->valor > valor) {
        bool rec = insere_no(&(*no)->esq, valor);
        if (rec) {
            if ((*no)->esq->prio > (*no)->prio) rotacao_direita(no);
        }
        return rec;
    }
    else {
        bool rec = insere_no(&(*no)->dir, valor);
        if (rec) {
            if ((*no)->dir->prio > (*no)->prio) rotacao_esquerda(no);
        }
        return rec;
    }
}


bool insere_treap (Treap arv, int valor) {
    if (arv == NULL) {
        printf("Tentativa de inserção em árvore inexistente!\n");
        return false;
    }

    if (!insere_no(&arv->raiz, valor)) return false;

    arv->elementos++;
    return true;
}


static int conta_filhos (No *no) {
    if (no == NULL) {
        printf("Tentativa de contar os filhos de nó NULL.\n");
        return -1;
    }

    if (no->esq != NULL && no->dir != NULL) return 2;
    if (no->dir != NULL) return FILHO_DIR;
    if (no->esq != NULL) return FILHO_ESQ;
    return 0;
}

static bool remove_no (No **no, int valor) {
    if (*no == NULL) return false;

    if ((*no)->valor == valor) {
        int filhos = conta_filhos(*no);
        if (filhos == 0) {
            free(*no);
            *no = NULL;
        }
        else if (filhos == FILHO_ESQ) {
            No *removido = *no;
            *no = (*no)->esq;
            free(removido);
        }
        else if (filhos == FILHO_DIR) {
            No *removido = *no;
            *no = (*no)->dir;
            free(removido);
        }
        else if (filhos == 2) {
            if ((*no)->esq->prio > (*no)->dir->prio) rotacao_direita(no);
            else rotacao_esquerda(no);
            return remove_no(no, valor);
        }
        else return false; // conta_filhos retornou -1
        return true;
    }

    else if ((*no)->valor > valor) return remove_no(&(*no)->esq, valor);
    else return remove_no(&(*no)->dir, valor);
}

bool remove_treap (Treap arv, int valor) {
    if (arv == NULL) {
        printf("Tentativa de remoção em árvore inexistente!\n");
        return false;
    }

    if (!remove_no(&arv->raiz, valor)) return false;

    arv->elementos--;
    return true;
}