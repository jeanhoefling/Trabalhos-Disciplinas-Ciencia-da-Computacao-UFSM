#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct timespec crono;

typedef struct {
    int pontos;
    int tiros;
    int escudos;
    int onda;
    int noite;

    int perdeu;
    int fim;

    char arma;
    char atacantes[10];

    crono temp;
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
    printf("%d %d %c", e.pontos, e.tiros, e.arma);
    for (int i = 1; i <= e.escudos; i++) {
        printf(")");
    }
    for (int i = 0; i < 10; i++) {
        printf("%c", e.atacantes[i]);
    }
    printf("\r");
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

void troca_arma (estado_jogo *e) {
    if (e->arma == '9') {
        e->arma = 'n';
    }
    else if (e->arma == 'n') {
        e->arma = '0';
    }
    else {
        e->arma++;
    }
}

void atira (estado_jogo *e) {
    for (int i = 0; i < 10; i++) {
        if (e->atacantes[i] == e->arma || (e->atacantes[i] == 'N' && e->arma == 'n')) {
            e->tiros--;
            if (e->atacantes[i] != 'N') {
                e->atacantes[i] = ' ';
            } else {
                e->atacantes[i] = 'n';
            }
            break;
        }
    }
}

void sonar (estado_jogo *e) {
    
}

void exec_tecla(char c, estado_jogo *e) {
    switch (c) {
        case 27:
            e->fim = 1;
            break;
        case '\t':
            troca_arma(e);
            break;
        case '\r':
            atira(e);
            break;
        case ' ':
            sonar(e);
            break;
        default:
            break;
    }
}

float temp_onda (estado_jogo e) {
    float temp = 2.0;
    for (int i = 1; i < e.onda; i++) {
        temp *= 0.9;
    }
    return temp;
}

void exec_onda (estado_jogo *e) {
    reset_atacantes(e->atacantes);
    crono_inicia(&e->temp);
    for (;;) {
        desenha_terminal(*e);
        int c = lechar();
        exec_tecla(c, e);
        if (e->fim) {
            return;
        }
        if(crono_parcial(&e->temp) >= temp_onda(*e)) {
            desloca_inimigos(e);
            if (e->perdeu) {
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
    estado_jogo e = { 0, 30, 3, 1, 0, 0, 0, '0' };
    for (;;) {
        if (e.perdeu || e.fim) {
            break;
        }
        exec_onda(&e);
    }
    if (e.perdeu) {
        printf("Você perdeu!");
    }
    normaliza_terminal();
}