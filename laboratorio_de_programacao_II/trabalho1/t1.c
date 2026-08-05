#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pontos;
    int tiros;
    int arma;
    int escudos;
    int atacantes[10];
} estado_jogo;

//TERMINAL E LEITURA DE TECLAS
void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

void normaliza_terminal()
{
    system("stty sane");
}

char lechar()
{
    fflush(stdout);
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}

// DESENHO DA TELA
void desenha_terminal(estado_jogo e) {
    printf("%d %d %d", e.pontos, e.tiros, e.arma);
    for (int i = 1; i <= 3; i++) {
        if (i <= e.escudos) {
            printf(")");
        }
    }
    for (int i = 0; i < 10; i++) {
        if (e.atacantes[i] == 0) {
            printf(" ");
        } else {
            printf("%d", e.atacantes[i]);
        }
    }
    printf("\n");
}

int main()
{
    configura_terminal();
    estado_jogo e = { 0, 30, 0, 3, {0} };

    for (;;) {
        int c = lechar();

        desenha_terminal(e);

        if (c == 'q') break;
    }
    normaliza_terminal();
}