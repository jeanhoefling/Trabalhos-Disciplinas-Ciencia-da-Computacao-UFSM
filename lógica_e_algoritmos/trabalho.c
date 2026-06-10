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

    //                0    1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16
    int ascii[17] = { 0, 9787, 9785, 9553, 9552, 9639, 9556, 9559, 9562, 9565, 9829, 9919, 9635, 9650, 9660, 9658, 9668 };

    typedef struct {
        char nome[10];
        int linha;
        int coluna;
        int dlinha;
        int dcoluna;
    } personagem;

    typedef struct {
        int vidas;
        int chave;
        int vidas_mapa;
        int setas_mapa;
        int rodada;
        int venceu;
    } estado_jogo;

    void limparTela() {
        system("clear");
    }

    void exibeChar(int numero) {
        wchar_t caractere = numero;
        wprintf(L"%lc", caractere);
    }

    void imprimeMatriz(int mat[LINHAS][COLUNAS], estado_jogo estado) {
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
        wprintf(L"Vidas: %i\n", estado.vidas);
        if (estado.chave == 0) {
            wprintf(L"Chave: NÃO\n");
        }
        else{
            wprintf(L"Chave: SIM\n");
        }
        wprintf(L"Rodadas: %i\n", estado.rodada);
    }


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

    // Essa função move retorna 1 se o movimento é válido, 0 se não for, 2 se venceu o jogo
    int move (int matriz[LINHAS][COLUNAS], personagem *personagem, estado_jogo *estado) {
        int proxima_posicao = matriz[personagem->linha + personagem->dlinha][personagem->coluna + personagem->dcoluna];
        if (proxima_posicao == VAZIO || proxima_posicao == VIDA || proxima_posicao == CHAVE || 
            proxima_posicao == S_CIMA || proxima_posicao == S_BAIXO || proxima_posicao == S_DIREITA || proxima_posicao == S_ESQUERDA) {
            int new_dlinha = 0, new_dcoluna = 0;
            if (proxima_posicao == VIDA) {
                estado->vidas_mapa--;
                if(compara_string(personagem->nome, "jogador")) {
                    estado->vidas++;
                }
            }
            else if (proxima_posicao == CHAVE) {
                if (compara_string(personagem->nome, "jogador")) {
                    estado->chave++;
                }
                else {
                    return 0;
                }
            }
            else if (proxima_posicao == S_CIMA || proxima_posicao == S_BAIXO || proxima_posicao == S_DIREITA || proxima_posicao == S_ESQUERDA) {
                estado->setas_mapa--;
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
            matriz[personagem->linha + personagem->dlinha][personagem->coluna + personagem->dcoluna] = matriz[personagem->linha][personagem->coluna];
            matriz[personagem->linha][personagem->coluna] = VAZIO;
            personagem->coluna = personagem->coluna + personagem->dcoluna;
            personagem->linha = personagem->linha + personagem->dlinha;
            if (new_dlinha != 0 || new_dcoluna != 0) {
                personagem->dlinha = new_dlinha;
                personagem->dcoluna = new_dcoluna;
            }
            return 1;
        }
        else if (proxima_posicao == SAIDA && compara_string(personagem->nome, "jogador") && estado->chave > 0) {
            estado->venceu = 1;
            return 1;
        }
        else if (proxima_posicao == JOGADOR || proxima_posicao == INIMIGO) {
            estado->vidas--;
            if (estado->vidas == 0) {
                matriz[personagem->linha + personagem->dlinha][personagem->coluna + personagem->dcoluna] = INIMIGO;
                matriz[personagem->linha][personagem->coluna] = VAZIO;
                return 1;
            }
            return 0;
        }
        else {
            return 0;
        }
    }

    void move_personagem (int matriz[LINHAS][COLUNAS], personagem *personagem, estado_jogo *estado) {
        int conseguiu_movimentar = move(matriz, personagem, estado);
        int direcao;
        while (!(conseguiu_movimentar)) {
            personagem->dlinha = 0;
            personagem->dcoluna = 0;
            direcao = rand() % 10000;
            if (direcao < 2500) {
                personagem->dlinha = 1;
                conseguiu_movimentar = move(matriz, personagem, estado);
            }
            else if (direcao < 5000) {
                personagem->dlinha = -1;
                conseguiu_movimentar = move(matriz, personagem, estado);
            }
            else if (direcao < 7500) {
                personagem->dcoluna = 1;
                conseguiu_movimentar = move(matriz, personagem, estado);
            }
            else {
                personagem->dcoluna = -1;
                conseguiu_movimentar = move(matriz, personagem, estado);
            }
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

    void gerar_posicao_saida (int posicao[2]) {
        int linha, coluna;
        do {
            linha = 1 + (rand() % 9);
            coluna = (rand() % 2) * 11;
        } while ((linha == 5 && coluna == 0) || (linha == 4 && coluna == 11));
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
        personagem jogador = {"jogador", 1, 1, 0, -1}; 
        personagem inimigo = {"inimigo", 9 , 10, 0, 1};

        personagem *ptr_jogador = &jogador;
        personagem *ptr_inimigo = &inimigo;

        estado_jogo estado = { 1, 0, 1, 1, 0, 0};
        estado_jogo *ptr_estado = &estado;

        mat[jogador.linha][jogador.coluna] = JOGADOR;
        mat[inimigo.linha][inimigo.coluna] = INIMIGO;
        int posicao[2];
        gerar_posicao(mat, posicao);
        mat[posicao[0]][posicao[1]] = CHAVE;
        gerar_posicao_saida(posicao);
        mat[posicao[0]][posicao[1]] = SAIDA; 
        

        while(estado.rodada < 10000 && !(estado.venceu) && estado.vidas > 0) {
            if (estado.rodada % 150 == 0 && estado.vidas_mapa <= 3 && estado.vidas < 3) {
                gerar_posicao(mat, posicao);
                mat[posicao[0]][posicao[1]] = VIDA;
                estado.vidas_mapa++;
            }
            if (estado.rodada % 100 == 0 && estado.setas_mapa <= 3) {
                gerar_posicao(mat, posicao);
                mat[posicao[0]][posicao[1]] = rand() % 4 + 13;
                estado.setas_mapa++;
            }
            imprimeMatriz(mat, estado);
            usleep(0.1 * 1000000);
            move_personagem(mat, ptr_jogador, ptr_estado);

            /* Esse imprime matriz com sleep adicional é importante pra entender a situação do tabuleiro após o movimento de um dos personagens, enquanto o outro 
               ainda não movimentou */
            imprimeMatriz(mat, estado);
            usleep(0.1 * 1000000);

            if (estado.vidas == 0) {
                break;
            }
            move_personagem(mat, ptr_inimigo, ptr_estado);
            estado.rodada++;
        }

        imprimeMatriz(mat, estado);
        if (estado.venceu) {
            wprintf(L"Parabéns, você venceu!!!\n");
        }
        else if (estado.rodada >= 10000) {
            wprintf(L"Que pena, o limite de rodadas foi excedido e você perdeu...\n");
        }
        else if (estado.vidas == 0) {
            wprintf(L"Que pena, o inimigo te pegou e você perdeu...\n");
        }

        return 0;
    }