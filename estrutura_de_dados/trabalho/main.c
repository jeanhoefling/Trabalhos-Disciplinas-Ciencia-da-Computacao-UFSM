#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "treap.h"

int main () {
    srand(time(NULL));
    Treap arv = cria_treap();

    for (int i = 0; i < 8; i++) {
        insere_treap(arv, i);
    }

    remove_treap(arv, 3);


    return 0;   
}