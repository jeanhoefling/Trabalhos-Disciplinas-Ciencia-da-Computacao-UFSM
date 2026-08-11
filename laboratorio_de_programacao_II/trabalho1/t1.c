#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct timespec crono;

typedef struct {
    int pontos;
    int tiros;
    int escudos;
    int onda;
    int noite;
    int inimigos_onda;

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
char gera_inimigo (estado_jogo *e) {
    int num;
    if (!e->noite) {
        num = rand() % 11;
        if (num == 10) {
            return 'N';
        } else {
            return '0' + num;
        }
    }
    else {
        num = rand() % 6;
        if (num == 5) {
            return 'N';
        } else {
            return '0' + num * 2;
        }
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
    for (int i = 0; i < 9 - e->noite * 5; i++) {
        e->atacantes[i] = e->atacantes[i+1];
    }
    if (e->inimigos_onda < 20 - (e->noite * 5)) {
        e->atacantes[9 - e->noite * 5] = gera_inimigo(e);
        e->inimigos_onda++;
    } else {
        e->atacantes[9 - e->noite * 5] = ' ';
    }
}

void reset_atacantes (estado_jogo *e) {
    for (int i = 0; i < 10; i++) {
        e->atacantes[i] = ' ';
    }
    e->inimigos_onda = 0;
}

void troca_arma (estado_jogo *e) {
    if ((e->arma == '9' && !(e->noite)) || (e->arma == '8' &&(e->noite)) ) {
        e->arma = 'n';
    }
    else if (e->arma == 'n') {
        e->arma = '0';
    }
    else {
        e->arma += 1 + e->noite;
    }
}

void pontos_kill (estado_jogo *e, int i) {
    if (e->atacantes[i] == 'n') {
        e->pontos += 2 * (10 - i - e->noite * 5);
    } else {
        e->pontos += 10 - i - e->noite * 5;
    }
}

void atira (estado_jogo *e) {
    if (e->tiros <= 0) {
        return;
    }
    e->tiros--;
    for (int i = 0; i < 10; i++) {
        if (e->atacantes[i] == e->arma || (e->atacantes[i] == 'N' && e->arma == 'n')) {
            if (e->atacantes[i] != 'N') {
                pontos_kill(e, i);
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

int fim_ataque (char v[10]) {
    for (int i = 0; i < 10; i++) {
        if (v[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

void exec_onda (estado_jogo *e) {
    reset_atacantes(e);
    crono_inicia(&e->temp);
    for (;;) {
        desenha_terminal(*e);
        int c = lechar();
        exec_tecla(c, e);
        if (e->fim || e->perdeu) {
            return;
        }
        if(crono_parcial(&e->temp) >= temp_onda(*e)) {
            desloca_inimigos(e);
            if (fim_ataque(e->atacantes) && e->inimigos_onda == 20 - e->noite*5) {
                e->onda++;
                break;
            }
            crono_inicia(&e->temp);
        }
    }
}

void vira_noite (estado_jogo *e) {
    e->noite = 0;
    int chance_dia = 100 - 20 * (e->onda - 1);
    if (chance_dia < 20) {
        chance_dia = 20;
    }
    int n = rand() % 100 + 1;
    if (n > chance_dia) {
        e->noite = 1;
    } 
}

void pontos_onda (estado_jogo *e) {
    e->pontos += 2 * e->tiros * (e->noite + 1);
    e->pontos += 10 * e->escudos * (e->noite + 1);
}

void recarrega (estado_jogo *e) {
    e->tiros = 30;
    e->escudos = 3;
}

FILE * arquivo_pontos() {
    char r = 'z';
    char arq_name[51];
    while (r != 's' && r != 'n') {
        printf("Você tem um arquivo de pontuações? (s/n)\n");
        r = fgetc(stdin);
        fgetc(stdin);
    }
    printf("Digite o nome desse arquivo (até 50 caracteres): ");
    fgets(arq_name, sizeof(arq_name), stdin); 
    arq_name[strcspn(arq_name, "\n")] = '\0';
    FILE *arq = fopen(arq_name, "r+");
    if (arq == NULL) {
        printf("Arquivo inválido");
        return NULL;
    } else {
        if (r == 'n') {
            fprintf(arq, "0\n0\n0\n");
        }
        return arq;
    }
}

void escreve_arquivo (FILE *arq, int pontos) {
    int p[3];
    char linha[100];
    rewind(arq);
    for (int i = 0; i < 3; i++) {
        fgets(linha, sizeof(linha), arq);
        linha[strcspn(linha, "\n")] = '\0';
        p[i] = atoi(linha);
    }
    if (pontos >= p[0]) {
        p[2] = p[1];
        p[1] = p[0];
        p[0] = pontos;
    } else if (pontos >= p[1]) {
        p[2] = p[1];
        p[1] = pontos;
    } else if (pontos >= p[2]) {
        p[2] = pontos;
    }
    rewind(arq);
    fprintf(arq, "%d\n%d\n%d", p[0], p[1], p[2]);
}

int main() {
    srand(time(NULL));
    FILE *arq = arquivo_pontos();
    if (arq == NULL) {
        return 1;
    }
    configura_terminal();
    estado_jogo e = { 0, 30, 3, 1, 0, 0, 0, 0, '0' };
    for (;;) {
        if (e.perdeu || e.fim) {
            break;
        }
        recarrega(&e);
        vira_noite(&e);
        exec_onda(&e);
        pontos_onda(&e);
    }
    if (e.perdeu) {
        printf("Você perdeu!");
    }
    normaliza_terminal();
    escreve_arquivo(arq, e.pontos);
}