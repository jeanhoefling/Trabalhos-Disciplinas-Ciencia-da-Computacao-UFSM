#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "treap.h"

int main () {
    srand(time(NULL));
    Treap arv = cria_arvore();

    for (int i = 0; i < 64; i++) {
        insere(arv, i);
    }


    return 0;   
}