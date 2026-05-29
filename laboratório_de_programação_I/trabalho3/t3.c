#include <stdio.h>
#include <string.h>

//Uma nota é um pequeno texto (limitado a um número máximo de bytes, que não pode ser inferior a 100), além de informações adicionais que permitem sua organização.
//O programa deve gerenciar um número qualquer de notas, limitado a um máximo (que não pode ser inferior a 100).

//Cada nota deve ser implementada em um registro. O conjunto de notas deve ser mantido em um vetor desses registros.
typedef struct cor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} cor;

typedef struct retangulo {
    int x;
    int y;
    int largura;
    int altura;
} retangulo;

typedef struct nota {
    char texto[150];
    cor cor;
    retangulo retangulo;
    char etiqueta[4];

} nota;

int main (void) {
    nota notas[5];
    char file_name[10];
    printf("Digite o arquivo para ler as notas: \n");
    scanf("%s", file_name);
    FILE *arq = fopen(file_name, "r");
    if (arq == NULL) {
        printf("Não consegui abrir o arquivo!\n");
        return -1;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(arq, "%s", notas[i].etiqueta);
        fscanf(arq, "%hhu", &notas[i].cor.r);
        fscanf(arq, "%hhu", &notas[i].cor.g);
        fscanf(arq, "%hhu", &notas[i].cor.b);
        fscanf(arq, "%d", &notas[i].retangulo.x);
        fscanf(arq, "%d", &notas[i].retangulo.y);
        fscanf(arq, "%d", &notas[i].retangulo.largura);
        fscanf(arq, "%d", &notas[i].retangulo.altura);
        fscanf(arq, "%s", notas[i].texto);
    }

    char modo[10] = "start";
    char escolhe_modo;
    char texto_de_busca[150] = "";
    char etiqueta_de_busca[4] = "";

    while (!(strcmp(modo, "terminar") == 0)) {
        printf("Escolha o modo: \n P: Principal | T: Edição de texto | E: Edição de etiqueta | C: Edição de cor | B: Edição de texto de busca | b: Edição de etiqueta de busca | F: Fim ");
        scanf(" %c", &escolhe_modo);

        if (escolhe_modo == 'P') {

        }

        else if (escolhe_modo == 'B') {
            printf("Digite o texto de busca: ");
            scanf("%s", texto_de_busca);
        }

        else if (escolhe_modo == 'b') {
            printf("Digite a etiqueta de busca: ");
            scanf("%s", etiqueta_de_busca);
        }

        if (escolhe_modo == 'F') {
            strcpy(modo, "terminar");
        }
    }
    fclose(arq);
}