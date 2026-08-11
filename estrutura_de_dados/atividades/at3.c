#include <stdio.h>
#include <stdlib.h>

char *cria1 (int v[], int tamanho);
void cria2 (int v[], int tamanho, char c[]);

int main (void) {
    int v[] = {2,-5,0,-6,1};

    char *v1 = cria1(v, 5);
    int i;
    for (i=0; i<5; i++) {
        printf("%c ",v1[i]);
    }
    printf("\n");
    
    char v2[5];
    cria2(v, 5, v2);
    for (i=0; i<5; i++) {
        printf("%c ",v2[i]);
    }
    free(v1);
}

char * cria1 (int v[], int tamanho) {
    char *vr = malloc(tamanho * sizeof(char));
    for (int i = 0; i < tamanho; i++) {
        if (v[i] > 0) {
            vr[i] = '+';
        } else if (v[i] < 0) {
            vr[i] = '-';
        } else {
            vr[i] = '0';
        }
    }
    return vr;
}

void cria2 (int v[], int tamanho, char c[]) {
    for (int i = 0; i < tamanho; i++) {
        if (v[i] > 0) {
            c[i] = '+';
        } else if (v[i] < 0) {
            c[i] = '-';
        } else {
            c[i] = '0';
        }
    }
}