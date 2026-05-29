    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <unistd.h>
    #include <wchar.h>
    #include <locale.h>

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
    #define CHAVE 11
    #define SAIDA 12
    #define S_CIMA 13
    #define S_BAIXO 14
    #define S_DIREITA 15
    #define S_ESQUERDA 16

    int vidas = 1;
    int chave = 0;

    //                0    1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16
    int ascii[17] = { 0, 9787, 9785, 9553, 9552, 9639, 9556, 9559, 9562, 9565, 9829, 9919, 9635, 9650, 9660, 9658, 9668 };

    void limparTela() {
        system("clear");
    }

    void exibeChar(int numero) {
        wchar_t caractere = numero;
        wprintf(L"%lc", caractere);
    }

    void imprimeMatriz(int mat[LINHAS][COLUNAS], int rodadas) {
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
        wprintf(L"Vidas: %i\n", vidas);
        if (chave == 0) {
            wprintf(L"Chave: NÃO\n");
        }
        else{
            wprintf(L"Chave: SIM\n");
        }
        wprintf(L"Rodadas: %i\n", rodadas);
    }

    typedef struct {
        char nome[10];
        int linha;
        int coluna;
    } personagem;

    // Função para comparar strings (retorna 1 se forem, 0 se não forem)
    int compara_string (char s1[], char s2[]) {
        int i = 0;
        while (s1[i] == s2[i]) {
            if (s1[i] == '\0') {
                return 1;
            }
            i++;
        }
        return 0;
    }

    // Essa função move retorna 1 se o movimento é válido e 0 se não for
    int move (int matriz[LINHAS][COLUNAS], personagem *personagem, int dlinha, int dcoluna) {
        int proxima_posicao = matriz[personagem->linha + dlinha][personagem->coluna + dcoluna];
        if (proxima_posicao == VAZIO || proxima_posicao == VIDA || proxima_posicao == CHAVE || 
            proxima_posicao == S_CIMA || proxima_posicao == S_BAIXO || proxima_posicao == S_DIREITA || proxima_posicao == S_ESQUERDA) {
            int new_dlinha = 0, new_dcoluna = 0;
            if (compara_string(personagem->nome, "jogador") && (proxima_posicao == VIDA)) {
                vidas++;
            }
            else if (proxima_posicao == CHAVE) {
                if (compara_string(personagem->nome, "jogador")) {
                    chave++;
                }
                else {
                    return 0;
                }
            }
            else if (proxima_posicao == S_CIMA || proxima_posicao == S_BAIXO || proxima_posicao == S_DIREITA || proxima_posicao == S_ESQUERDA) {
                if (proxima_posicao == S_CIMA) {
                    new_dlinha = -1;
                }
                else if (proxima_posicao == S_BAIXO) {
                    new_dlinha = 1;
                }
                else if (proxima_posicao == S_DIREITA) {
                    new_dcoluna = 1;
                }
                else {
                    new_dcoluna = -1;
                }
            }
            matriz[personagem->linha + dlinha][personagem->coluna + dcoluna] = matriz[personagem->linha][personagem->coluna];
            matriz[personagem->linha][personagem->coluna] = VAZIO;
            personagem->coluna = personagem->coluna + dcoluna;
            personagem->linha = personagem->linha + dlinha;
            if (!(new_dcoluna == 0 && new_dlinha == 0)) {
                int conseguiu_move_seta = move(matriz, personagem, new_dlinha, new_dcoluna);
                if (conseguiu_move_seta != 0) {
                    return conseguiu_move_seta;
                }
            }
            return 1;
        }
        else if (proxima_posicao == SAIDA && compara_string(personagem->nome, "jogador") && chave > 0) {
            return 2;
        }
        else if (proxima_posicao == JOGADOR || proxima_posicao == INIMIGO) {
            vidas--;
            return 1;
        }
        else {
            return 0;
        }
    }

    // Essa função retorna 1 se o personagem venceu o jogo, 0 se ainda não
    int move_personagem (int matriz[LINHAS][COLUNAS], personagem *personagem) {
        int conseguiu_movimentar = 0, direcao;
        while (!(conseguiu_movimentar)) {
            direcao = rand() % 4;
            if (direcao == 0) {
                conseguiu_movimentar = move(matriz, personagem, -1, 0);
            }
            else if (direcao == 1) {
                conseguiu_movimentar = move(matriz, personagem, 1, 0);
            }
            else if (direcao == 2) {
                conseguiu_movimentar = move(matriz, personagem, 0, -1);
            }
            else {
                conseguiu_movimentar = move(matriz, personagem, 0, 1);
            }
        }
        if (conseguiu_movimentar == 2) {
            return 1;
        }
        else {
            return 0;
        }
    }

    // Função para gerar posição aleatória para vida, chave ou setas
    void gerar_posicao (int matriz[LINHAS][COLUNAS], int posicao[2]) {
        int linha, coluna;
        do {
            linha = (rand() % (LINHAS - 2)) + 1;
            coluna = (rand() % (COLUNAS - 2)) + 1;
        } while (matriz[linha][coluna] != VAZIO);
        posicao[0] = linha;
        posicao[1] = coluna;
    }

    int main() {

        setlocale(LC_ALL, "");

        srand(time(NULL));

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

        // Definindo a posição inicial do jogador e do inimigo
        personagem jogador = {"jogador", 1, 1}; 
        personagem inimigo = {"inimigo", 9 , 10};

        personagem *ptr_jogador = &jogador;
        personagem *ptr_inimigo = &inimigo;

        int rodada = 0;

        mat[jogador.linha][jogador.coluna] = JOGADOR;
        mat[inimigo.linha][inimigo.coluna] = INIMIGO;
        int posicao[2];
        gerar_posicao(mat, posicao);
        mat[posicao[0]][posicao[1]] = CHAVE;
        gerar_posicao(mat, posicao);
        mat[posicao[0]][posicao[1]] = rand() % 4 + 13;
        mat[1][11] = SAIDA; 

        int venceu = 0;

        while(rodada < 10000 && !(venceu) && vidas > 0) {
            if (rodada % 100 == 0) {
                gerar_posicao(mat, posicao);
                mat[posicao[0]][posicao[1]] = VIDA;
            }
            imprimeMatriz(mat, rodada);
            usleep(0.1 * 1000000);
            venceu = move_personagem(mat, ptr_jogador);

            /* Esse imprime matriz com sleep adicional é importante pra entender a situação do tabuleiro após o movimento de um dos personagens, enquanto o outro 
               ainda não movimentou */
            imprimeMatriz(mat, rodada);
            usleep(0.1 * 1000000);

            move_personagem(mat, ptr_inimigo);
            rodada++;
        }

        if (venceu) {
            wprintf(L"Parabéns, você venceu!!!\n");
        }
        else if (rodada >= 10000) {
            wprintf(L"Que pena, o limite de rodadas foi excedido e você perdeu...\n");
        }
        else if (vidas == 0) {
            wprintf(L"Que pena, o inimigo te pegou e você perdeu...\n");
        }

        return 0;
    }