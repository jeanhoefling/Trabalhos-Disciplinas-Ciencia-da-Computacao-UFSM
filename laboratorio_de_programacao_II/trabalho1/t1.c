#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct timespec crono;

typedef struct {
    int pontos;
    int tiros;
    int arma;
    int escudos;

    int perdeu;
    crono temp;

    char atacantes[10];
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

//TEMPO

// inicializa um cronômetro com a hora atual
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// retorna o tempo passado desde que o cronômetro *c foi iniciado, em segundos
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);

    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

// DESENHO DA TELA
void desenha_terminal(estado_jogo e) {
    printf("%d %d %d", e.pontos, e.tiros, e.arma);
    for (int i = 1; i <= e.escudos; i++) {
        printf(")");
    }
    for (int i = 0; i < 10; i++) {
        if (e.atacantes[i] == 0) {
            printf(" ");
        } else {
            printf("%c", e.atacantes[i]);
        }
    }
    printf("\n");
}

//DINAMICA DO JOGO
char gera_inimigo () {
    int num = rand() % 11;
    if (num == 10) {
        return 'N';
    } else {
        return '0' + num;
    }
}

void desloca_inimigos (estado_jogo *e) {
    if (e->atacantes[0] != ' ') {
        if (e->escudos == 0) {
            e->perdeu = 1;
            return;
        } else {
            e->escudos--;
        }
    }
    for (int i = 0; i < 9; i++) {
        e->atacantes[i] = e->atacantes[i+1];
    }
    e->atacantes[9] = gera_inimigo();
}

void reset_atacantes (char v[10]) {
    for (int i = 0; i < 10; i++) {
        v[i] = ' ';
    }
}

void exec_onda (estado_jogo *e) {
    reset_atacantes(e->atacantes);
    crono_inicia(&e->temp);
    for (;;) {
        desenha_terminal(*e);
        int c = lechar();
        if (c == 'q') break;
        if(crono_parcial(&e->temp) >= 0.5) {
            desloca_inimigos(e);
            if (e->perdeu == 1) {
                return;
            }
            crono_inicia(&e->temp);
        }
    }
}

int main()
{
    srand(time(NULL));
    configura_terminal();
    estado_jogo e = { 0, 30, 0, 3, 0 };
    for (;;) {
        if (e.perdeu == 1) {
            printf("Você perdeu!");
            break;
        }
        exec_onda(&e);
    }
    normaliza_terminal();
}