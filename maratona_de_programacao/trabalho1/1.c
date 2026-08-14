#include <stdio.h>

int main() {
    int t = 0;
    int i = 0;
    char nome[100];
    int a;

    while (fgets(nome, sizeof(nome), stdin) != NULL) {
        scanf("%d", &a);
        getchar();

        t += a;
        i++;
    }

    printf("%.1f\n", (double)t / i);

    return 0;
}