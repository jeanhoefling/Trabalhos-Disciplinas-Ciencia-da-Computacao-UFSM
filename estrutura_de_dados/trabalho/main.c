#include <stdlib.h>
#include <stdio.h>
#include "treap.h"

int main () {
    Treap arv = cria_treap();

    for (int i = 1; i <= 64; i++) {
        insere_treap(arv, i);
    }

    int escolha = -1;

    while (escolha != 0) {
        int valor;
        printf("---- Menu ----\n1: Busca\n2: Inserção\n3: Remoção\n0: Finalizar\nEscolha: ");
        scanf(" %d", &escolha);
        if (escolha == 0) break;
        if (escolha == 1 || escolha == 2 || escolha == 3) {
            printf("Digite o valor: ");
            scanf(" %d", &valor);
        } else { // escolha inválida
            continue;
        }

        if (escolha == 1) {
            if (busca_treap(arv, valor)) printf("O valor %d está na Treap\n", valor);
            else printf("O valor %d não está na Treap\n", valor);
        } else if (escolha == 2) {
            insere_treap(arv, valor);
        } else {
            remove_treap(arv, valor);
        }

        imprime_ia(arv);
    }

    imprime(arv);

    destroi_treap (arv);
    return 0;   
}