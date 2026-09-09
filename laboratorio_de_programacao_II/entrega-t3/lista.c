#include "lista.h"

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