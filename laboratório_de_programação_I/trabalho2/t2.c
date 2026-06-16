#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//1
void preenche_tabuleiro (int vetor[16]) {
    for (int i = 0; i <= 15; i++) {
        vetor[i] = i;
    }
}

//2
int verifica_posicao (int num, int vetor[16]) {
    for (int i = 0; i <= 15; i++) {
        if (vetor[i] == num) {
            return i;
        }
    }
    return -1;
}

//3
bool check_ordem (int vetor[16]) {
    for (int i = 0; i <= 15; i++) {
        if (vetor[i] != i) {
            return false;
        } 
    }
    return true;
} 

//4
void esquerda (int vetor[16]) {
    int pos_15 = 0;
    while (vetor[pos_15] != 15) {
        pos_15++;
    }
    if (pos_15 % 4 != 0) {
        int temp = vetor[pos_15 - 1];
        vetor[pos_15 - 1] = 15;
        vetor[pos_15] = temp;
    }
    return;
}

//5
void direita (int vetor[16]) {
    int pos_15 = 0;
    while (vetor[pos_15] != 15) {
        pos_15++;
    }
    if (pos_15 % 4 != 3) {
        int temp = vetor[pos_15 + 1];
        vetor[pos_15 + 1] = 15;
        vetor[pos_15] = temp;
    }
    return;
}

//6
void acima (int vetor[16]) {
    int pos_15 = 0;
    while (vetor[pos_15] != 15) {
        pos_15++;
    }
    if (pos_15 > 3) {
        int temp = vetor[pos_15 - 4];
        vetor[pos_15 - 4] = 15;
        vetor[pos_15] = temp;
    }
    return;
}

//7
void abaixo (int vetor[16]) {
    int pos_15 = 0;
    while (vetor[pos_15] != 15) {
        pos_15++;
    }
    if (pos_15 < 12) {
        int temp = vetor[pos_15 + 4];
        vetor[pos_15 + 4] = 15;
        vetor[pos_15] = temp;
    }
    return;
}

//8
void mov_aleatorio (int vetor[16]) {
    int mov = rand() % 4;
    if (mov == 0) {
        esquerda(vetor);
    }
    else if (mov == 1) {
        direita(vetor);
    }
    else if (mov == 2) {
        acima(vetor);
    }
    else {
        abaixo(vetor);
    }
}

//9
void embaralha (int vetor[16]) {
    for (int i = 0; i < 100; i++) {
        mov_aleatorio(vetor);
    }
}

//10
void mov_user (int vetor[16], char letra) {
    if (letra == 'w' || letra == 'c' || letra == 'A') {
        abaixo(vetor);
    }
    else if (letra == 'a' || letra == 'e' || letra == 'D') {
        direita(vetor);
    }
    else if (letra == 's' || letra == 'b' || letra == 'B') {
        acima(vetor);
    }
    else if (letra == 'd' || letra == 'C') {
        esquerda(vetor);
    }
}

//11

//Parte adicionada ao programa para facilitar a execução da função solicitada no item 11
//A ordem será verificar esquerda, direita, acima e abaixo.
void verifica_lados (int pos, bool verificado[4]) {
    //Inicializa o vetor "verificado"
    for (int i = 0; i < 4; i++) {
        verificado[i] = false;
    }

    //Esquerda
    if (pos % 4 != 0) {
        verificado[0] = true;
    }
    else {}

    //Direita
    if (pos % 4 != 3) {
        verificado[1] = true;
    }

    //Acima
    if (pos > 3) {
        verificado[2] = true;
    }

    //Abaixo
    if (pos < 12) {
        verificado[3] = true;
    }
    
}

//Função exigida de fato no exercício 11
void move_diff (int vetor[16], int pos) {
    // Caso receba uma posição inválida
    if (pos < 0 || pos > 15) {
        return;
    }

    bool verificado[4];
    verifica_lados(pos, verificado);
    if (verificado[0]) {
        if (vetor[pos - 1] == 15) {
            esquerda(vetor);
        }
    }
    if (verificado[1]) {
        if (vetor[pos + 1] == 15) {
            direita(vetor);
        }
    }
    if (verificado[2]) {
        if (vetor[pos - 4] == 15) {
            acima(vetor);
        }
    }
    if (verificado[3]) {
        if (vetor[pos + 4] == 15) {
            abaixo(vetor);
        }
    }

}

//12
void desenha (int vetor[16]) {
    for (int i = 0; i <= 15; i++) {
        if (i % 4 == 0) {
            printf("\n---------------------");
            printf("\n|");
        }
        printf(" %2i |", vetor[i]);
    }
    printf("\n---------------------");
    printf("\n");
}

// Parte realizada pelo Benhur
void cor_letra(int r, int g, int b)
{
  printf("%c[38;2;%d;%d;%dm", 27, r, g, b);
}

void cor_fundo(int r, int g, int b)
{
  printf("%c[48;2;%d;%d;%dm", 27, r, g, b);
}

void cor_normal()
{
  printf("%c[0m", 27);
}

void cor_tabuleiro()
{
  cor_fundo(140, 70, 20); // marrom
}
void cor_peca()
{
  cor_fundo(200, 200, 170); // branco sujo
  cor_letra(70, 50, 40); // escuro
}
void cor_vazio()
{
  cor_fundo(0, 0, 0); // preto
}

void pula_linha()
{
  printf("\r\n");
}

void limpa_tela()
{
  cor_normal();
  printf("%c[2J", 27); // limpa a tela
  printf("%c[H", 27);  // põe o cursor no início
}

void desenha_borda()
{
  cor_normal();
  printf("%3s", "");
  cor_tabuleiro();
  printf("%32s","");
  cor_normal();
  pula_linha();
}
void imprime_pedaco_linha_v8(int t[], int l, bool com_peca)
{
  char desenho[] = "ABCDEFGHIJKLMNO ";

  cor_normal();
  printf("%3s", "");
  cor_tabuleiro();
  printf("  ");
  for (int i = 0; i < 4; i++) {
    int v = t[l*4 + i];
    v == 15 ? cor_vazio() : cor_peca();
    printf("   %c   ", com_peca ? desenho[v] : ' ');
  }
  cor_tabuleiro();
  printf("  ");
  cor_normal();
  pula_linha();
}

void imprime_linha_v8(int t[], int l)
{
  imprime_pedaco_linha_v8(t, l, false);
  imprime_pedaco_linha_v8(t, l, true);
  imprime_pedaco_linha_v8(t, l, false);
}

void imprime_tabuleiro_v8(int t[])
{
  limpa_tela();
  pula_linha();
  desenha_borda();
  for (int linha = 0; linha < 4; linha++) {
    imprime_linha_v8(t, linha);
  }
  desenha_borda();
}

int main () {
    system("stty raw -echo");
    srand(time(0));
    int vetor[16];
    preenche_tabuleiro (vetor);
    embaralha(vetor);
    char input = 'z';
    while (input != 'Q' && !check_ordem(vetor)) {
        imprime_tabuleiro_v8(vetor);
        printf("\n");
        scanf(" %c", &input);
        mov_user(vetor, input);
    }
    if (check_ordem(vetor)) {
        printf("Você Ganhou!!!");
    }
    pula_linha();
    printf("Tchau");
    pula_linha();
    system("stty sane");
}