#include <stdio.h>

int main() {
    double n1, n2, n3, n4;
    double m1, ne, nf;

    scanf("%lf %lf %lf %lf", &n1, &n2, &n3, &n4);

    m1 = (n1 * 2 + n2 * 3 + n3 * 4 + n4) / 10;

    printf("Media: %.1f\n", m1);

    if (m1 >= 7.0) {
        printf("Aluno aprovado.\n");
    }
    else if (m1 < 5.0) {
        printf("Aluno reprovado.\n");
    }
    else {
        printf("Aluno em exame.\n");

        scanf("%lf", &ne);

        printf("Nota do exame: %.1f\n", ne);

        nf = (ne + m1) / 2;

        if (nf >= 5.0) {
            printf("Aluno aprovado.\n");
        }
        else {
            printf("Aluno reprovado.\n");
        }

        printf("Media final: %.1f\n", nf);
    }

    return 0;
}