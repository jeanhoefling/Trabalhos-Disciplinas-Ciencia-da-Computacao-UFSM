#include <stdlib.h>
#include <stdio.h>
#include "treap.h"

int main () {
    Treap arv = cria_treap();

    for (int i = 1; i <= 64; i++) {
        insere_treap(arv, i);
    }


    // ---------- Testando busca ---------- //
    if (busca_treap(arv, -2)) printf("O valor -2 está na Treap\n");
    else printf("O valor -2 não está na Treap\n");

    if (busca_treap(arv, 4)) printf("O valor 4 está na Treap\n");
    else printf("O valor 4 não está na Treap\n");

    // ---------- Testando inserção ---------- //
    insere_treap(arv, 99);
    insere_treap(arv, 4);

    // ---------- Testando remoção ---------- //
    remove_treap(arv, 32);
    remove_treap(arv, -2);

    imprime(arv);
    imprime_ia(arv);
    destroi_treap (arv);
    return 0;   
}