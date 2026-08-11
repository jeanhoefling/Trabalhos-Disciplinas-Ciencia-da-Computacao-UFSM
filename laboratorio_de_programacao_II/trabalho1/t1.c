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

    int fim;

    char arma;
    char esc_atac[13];

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
    printf("%d ", e.pontos);
    if (!e.noite) {
        printf("%d %c", e.tiros, e.arma);
        for (int i = 0; i < 13; i++) {
            printf("%c", e.esc_atac[i]);
        }
    }
    printf("\r");
}

//DINAMICA DO JOGO
void som_especifico (char c) {
    char rota[30];
    if (c == 'N' || c == 'n') {
        sprintf(rota, "aplay -q ./sons/11.3.wav &");
    } else if (c == ')') {
        sprintf(rota, "aplay -q ./sons/12.3.wav &");
    } else if (c == ' ') {
        sprintf(rota, "aplay -q ./sons/x.3.wav &");
    } else {
        sprintf(rota, "aplay -q ./sons/%c.3.wav &", c);
    }
    system(rota);
}

char gera_inimigo (estado_jogo *e) {
    int num;
    char inimigo;
    if (!e->noite) {
        num = rand() % 11;
        if (num == 10) {
            inimigo = 'N';
        } else {
            inimigo = '0' + num;
        }
    }
    else {
        num = rand() % 6;
        if (num == 5) {
            inimigo = 'N';
        } else {
            inimigo = '0' + num * 2;
        }
    }
    som_especifico(inimigo);
    return inimigo;
}

void desloca_inimigos (estado_jogo *e) {
    if (e->esc_atac[e->escudos] != ' ') {
        if (e->escudos == 0) {
            e->fim = 1;
            return;
        } else {
            e->esc_atac[e->escudos - 1] = ' ';
            e->esc_atac[e->escudos] = ' ';
            e->escudos--;
        }
    }
    for (int i = e->escudos; i < 12 - e->noite * 5; i++) {
        e->esc_atac[i] = e->esc_atac[i+1];
    }
    if (e->inimigos_onda < 20 - (e->noite * 5)) {
        e->esc_atac[12 - e->noite * 5] = gera_inimigo(e);
        e->inimigos_onda++;
    } else {
        e->esc_atac[12 - e->noite * 5] = ' ';
    }
}

void reset_atacantes (estado_jogo *e) {
    for (int i = 0; i < e->escudos; i++) {
        e->esc_atac[i] = ')';
    }
    for (int i = e->escudos; i < 13; i++) {
        e->esc_atac[i] = ' ';
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
    som_especifico(e->arma);
}

void pontos_kill (estado_jogo *e, int i) {
    if (e->esc_atac[i] == 'n') {
        e->pontos += 2 * (13 - i - e->noite * 5) * (e->noite + 1);
    } else {
        e->pontos += (13 - i - e->noite * 5) * (e->noite + 1);
    }
}

void atira (estado_jogo *e) {
    if (e->tiros <= 0) {
        return;
    }
    e->tiros--;
    for (int i = e->escudos; i < 13; i++) {
        if (e->esc_atac[i] == e->arma || (e->esc_atac[i] == 'N' && e->arma == 'n')) {
            if (e->esc_atac[i] != 'N') {
                pontos_kill(e, i);
                e->esc_atac[i] = ' ';
            } else {
                e->esc_atac[i] = 'n';
            }
            som_especifico(e->arma);
            return;
        }
    }
    som_especifico('x');
}

void sonar (estado_jogo *e) {
    char rota_completa[250] = "aplay -q ";
    char rota[20];
    for (int i = 0; i < 13; i++) {
        if (e->esc_atac[i] == 'N' || e->esc_atac[i] == 'n') {
            sprintf(rota, "./sons/11.3.wav ");
        } else if (e->esc_atac[i] == ')') {
            sprintf(rota, "./sons/12.3.wav ");
        } else if (e->esc_atac[i] == ' ') {
            sprintf(rota, "./sons/x.3.wav ");
        } else {
            sprintf(rota, "./sons/%c.3.wav ", e->esc_atac[i]);
        }
        strcat(rota_completa, rota);
    }
    strcat(rota_completa, "&");
    system(rota_completa);
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
    if (e.noite) {
        return temp * 3;
    } else {
        return temp;
    }
}

int fim_ataque (estado_jogo e) {
    for (int i = e.escudos; i < 13; i++) {
        if (e.esc_atac[i] != ' ') {
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
        if (e->fim) {
            return;
        }
        if(crono_parcial(&e->temp) >= temp_onda(*e)) {
            desloca_inimigos(e);
            if (fim_ataque(*e) && e->inimigos_onda == 20 - e->noite*5) {
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
    e->arma = '0';
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
    FILE *arq = r == 's' ? fopen(arq_name, "r+") : fopen(arq_name, "w+");
    if (arq == NULL) {
        printf("Arquivo inválido");
        return NULL;
    } else {
        if (r == 'n') {
            fprintf(arq, "0\n0\n0\n");
        }
        system("clear");
        return arq;
    }
}

void tchau (int p[3], int pontos) {
    system("clear");
    printf("---Fim do jogo---\nPontuação Final: %d\n\n----Ranking----\nTOP 1: %d\nTOP 2: %d\nTOP 3: %d\n", pontos, p[0], p[1], p[2]);
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
    tchau(p, pontos);
}

void tela_fonda(estado_jogo *e) {
    system("clear");
    system("aplay -q ./sons/1.3.wav ./sons/5.3.wav ./sons/9.3.wav &");
    printf("---FIM DA ONDA %d---\nTIROS: %d\nESCUDOS: %d\nTOTAL DE PONTOS: %d", e->onda - 1, e->tiros, e->escudos, e->pontos);
    printf("\n\nDigite 'r' se deseja continuar e 'esc' para finalizar");
    int c;
    do {
        c = lechar();
    } while (c != 27 && c != 'r');
    if (c == 27) {
        e->fim = 1;
    }
    system("clear");
}

void som_fjogo() {
    system("aplay -q ./sons/1.3.wav ./sons/3.3.wav ./sons/5.3.wav ./sons/7.3.wav ./sons/9.3.wav &");
}

void fim_onda (estado_jogo *e) {
    tela_fonda(e);
    recarrega(e);
    vira_noite(e);
    sleep(3);
}

int main() {
    srand(time(NULL));
    FILE *arq = arquivo_pontos();
    configura_terminal();
    estado_jogo e = { 0, 30, 3, 1, 0, 0, 0, '0' };
    for (;;) {
        exec_onda(&e);
        pontos_onda(&e);
        if (e.fim) {
            som_fjogo();
            break;
        }
        fim_onda(&e);
    }
    normaliza_terminal();
    escreve_arquivo(arq, e.pontos);
    fclose(arq);
}