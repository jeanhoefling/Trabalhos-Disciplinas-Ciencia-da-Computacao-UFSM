#include <stdio.h>

int main() {
    char nome[100];
    scanf("%s", nome);
    double a, b;

    scanf("%lf", &a);
    scanf("%lf", &b);

    printf("TOTAL = R$ %.2f\n", a + (b * 15 / 100));

    return 0;
}