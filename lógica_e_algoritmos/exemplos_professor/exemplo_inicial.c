#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#define LINHAS 11
#define COLUNAS 12

#define VAZIO 0
#define JOGADOR 1
#define INIMIGO 2
#define PAREDE_VERTICAL 3
#define PAREDE_HORIZONTAL 4
#define OBSTACULO 5
#define CANTO_SUP_ESQ 6
#define CANTO_SUP_DIR 7
#define CANTO_INF_ESQ 8
#define CANTO_INF_DIR 9
#define VIDA 10

//                0    1     2     3     4     5     6     7     8     9     10
int ascii[15] = { 0, 9787, 9785, 9553, 9552, 9639, 9556, 9559, 9562, 9565, 9829 };

void limparTela() {
    const char *CLEAR_SCREEN_ANSI = "\033[1;1H\033[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, strlen(CLEAR_SCREEN_ANSI));
}

void exibeChar(int numero) {
    wchar_t caractere = numero;
    wprintf(L"%lc", caractere);
}

void imprimeMatriz(int mat[LINHAS][COLUNAS]) {
    int i, j;

    limparTela();

    for(i = 0; i < LINHAS; i++) {
        for(j = 0; j < COLUNAS; j++) {

            if(mat[i][j] == VAZIO) {
                wprintf(L"  ");
            }
            else if(mat[i][j] == PAREDE_HORIZONTAL) {
                exibeChar(ascii[mat[i][j]]);
                exibeChar(ascii[mat[i][j]]);
            }
            else if(mat[i][j] == CANTO_SUP_ESQ || mat[i][j] == CANTO_INF_ESQ) {
                exibeChar(ascii[mat[i][j]]);
                exibeChar(ascii[PAREDE_HORIZONTAL]);
            }
            else {
                exibeChar(ascii[mat[i][j]]);
                wprintf(L" ");
            }
        }

        wprintf(L"\n");
    }
}

int main() {
    setlocale(LC_ALL, "");

    int mat[LINHAS][COLUNAS] = {
        {6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 5, 5, 5, 0, 0, 5, 5, 0, 0, 3},
        {3, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 3},
        {3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 3},
        {3, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 5, 0, 5, 0, 5, 0, 0, 3},
        {3, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 3},
        {3, 0, 5, 5, 0, 5, 0, 0, 0, 5, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 9}
    };

    int jogadorLinha = 1;
    int jogadorColuna = 1;

    int inimigoLinha = 9;
    int inimigoColuna = 10;

    int rodada = 0;

    mat[jogadorLinha][jogadorColuna] = JOGADOR;
    mat[inimigoLinha][inimigoColuna] = INIMIGO;
    mat[5][5] = VIDA;

    while(rodada < 10) {
        imprimeMatriz(mat);

        sleep(1);

        /*
            Exemplo simples de movimento automático.

            Este exemplo apenas move o jogador para a direita.
            O aluno deverá melhorar esta lógica para:
            - não atravessar paredes;
            - detectar colisão;
            - coletar itens;
            - mover inimigos;
            - controlar vidas ou pontos;
            - encerrar o jogo.
        */

        mat[jogadorLinha][jogadorColuna] = VAZIO;

        jogadorColuna++;

        mat[jogadorLinha][jogadorColuna] = JOGADOR;

        rodada++;
    }

    return 0;
}