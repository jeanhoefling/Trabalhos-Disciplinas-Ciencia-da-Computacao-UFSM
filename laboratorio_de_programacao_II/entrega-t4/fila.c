#include "fila.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define ESQUERDA 0
#define DIREITA  1
#define SEM_DIRECAO 2


typedef struct no {
    struct no *ant;
    struct no *prox;
    void *dado;
} No;

struct fila {
    No* ini;
    No* fim;
    int tam_dado;
    int tam_lista;

    // Campos importantes para o percurso
    No *atual; 
    char direcao;
};

Fila f_cria(int tam_do_dado){
    Fila f = (Fila) malloc(sizeof(struct fila));

    assert(f != NULL);

    f->ini = NULL;
    f->fim = NULL;
    f->tam_dado = tam_do_dado;
    f->tam_lista = 0;
    f->atual = NULL;
    f->direcao = SEM_DIRECAO;

    return f;
}

void f_destrói(Fila self) {
    No *p = self->ini;
    No *temp = p;
    while (p != NULL) {
        temp = p;
        p = p->prox;
        free(temp->dado);
        free(temp);
    }
    free(self);
}

bool f_tá_vazia(Fila self) {
    return self->tam_lista == 0;
}


void f_remove(Fila self, void *pdado) {
    if (self->ini == NULL) return;
    No* inicio = self->ini;
    No *p = inicio->prox;
    if (p != NULL) {
        p->ant = NULL;
    }

    if (pdado != NULL) memcpy(pdado, inicio->dado, self->tam_dado);

    self->ini = p;
    free(inicio->dado);
    free(inicio);

    self->tam_lista--;
    if (self->tam_lista == 0) {
        self->fim = NULL;
    }
}


void f_insere(Fila self, void *pdado) {
    No *p = malloc(sizeof(No));
    assert(p != NULL);
    p->dado = malloc(self->tam_dado);
    assert(p->dado != NULL);
    memcpy(p->dado, pdado, self->tam_dado);

    p->ant = self->fim;
    p->prox = NULL;

    if (self->tam_lista == 0) {
        self->ini = p;
    } else {
        self->fim->prox = p;
    }
    self->fim = p;
    self->tam_lista++;
}


void f_inicia_percurso(Fila self, int pos_inicial) {
    if (self->tam_lista == 0) return;
    if (pos_inicial >  self->tam_lista - 1 || pos_inicial < -self->tam_lista) {
        printf("Posição inicial inválida\n");
        return;
    }
    if (pos_inicial >= 0) {
        self->direcao = DIREITA;
    } else {
        self->direcao = ESQUERDA;
        pos_inicial =  self->tam_lista + pos_inicial;
    }
    No *p = self->ini;
    for (int i = 0; i < pos_inicial; i++) {
        p = p->prox;
    }
    self->atual = p;
}


bool f_próximo(Fila self, void *pdado) {
    if (self->atual == NULL) return false;
    if (pdado != NULL) memcpy(pdado, self->atual->dado, self->tam_dado);
    if (self->direcao == DIREITA) {
        self->atual = self->atual->prox;
    } else {
        self->atual = self->atual->ant;
    }
    return true;
}