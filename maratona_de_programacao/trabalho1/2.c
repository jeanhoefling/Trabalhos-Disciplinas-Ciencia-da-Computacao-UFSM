#include <stdio.h>

int main() {
    int notas[] = {10000, 5000, 2000, 1000, 500, 200};
    int moedas[] = {100, 50, 25, 10, 5, 1};

    double valor;

    scanf("%lf", &valor);

    valor *= 100;
    int v = (int)valor;

    printf("NOTAS:\n");

    for (int i = 0; i < 6; i++) {
        printf("%d nota(s) de R$ %.2f\n", v / notas[i], notas[i] / 100.0);
        v = v % notas[i];
    }

    printf("MOEDAS:\n");

    for (int i = 0; i < 6; i++) {
        printf("%d moeda(s) de R$ %.2f\n", v / moedas[i], moedas[i] / 100.0);
        v = v % moedas[i];
    }

    return 0;
}