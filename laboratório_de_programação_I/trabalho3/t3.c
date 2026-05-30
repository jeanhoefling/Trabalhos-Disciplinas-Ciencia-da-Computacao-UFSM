#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Uma nota é um pequeno texto (limitado a um número máximo de bytes, que não pode ser inferior a 100), além de informações adicionais que permitem sua organização.
//O programa deve gerenciar um número qualquer de notas, limitado a um máximo (que não pode ser inferior a 100).

//Cada nota deve ser implementada em um registro. O conjunto de notas deve ser mantido em um vetor desses registros.
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} cor;

typedef struct {
    int x;
    int y;
    int largura;
    int altura;
} retangulo;

typedef struct {
    char texto[150];
    cor cor;
    retangulo retangulo;
    char etiqueta[4];
} nota;

int ler_notas (FILE *arq, int *n_notas, nota **notas) {
    int cont, c;
    while (fscanf(arq, "%s %hhu %hhu %hhu %d %d %d %d ", 
        (*notas)[(*n_notas) - 1].etiqueta,
        &(*notas)[(*n_notas) - 1].cor.r,
        &(*notas)[(*n_notas) - 1].cor.g,
        &(*notas)[(*n_notas) - 1].cor.b,
        &(*notas)[(*n_notas) - 1].retangulo.x,
        &(*notas)[(*n_notas) - 1].retangulo.y,
        &(*notas)[(*n_notas) - 1].retangulo.largura,
        &(*notas)[(*n_notas) - 1].retangulo.altura
    ) == 8) {
        cont = 0;

        //Serve para ignorar a primeira " da palavra
        fgetc(arq);


        //AINDA PRECISA RESOLVER QUANDO UMA LINHA TEM MULTIPLAS ""
        while ((c = fgetc(arq)) != '"')
        {
            (*notas)[(*n_notas) - 1].texto[cont] = c;
            cont++;
        }
        (*notas)[(*n_notas) - 1].texto[cont] = '\0';
        
        (*n_notas)++;
        (*notas) = realloc((*notas), (*n_notas) * sizeof(nota));
        if ((*notas) == NULL) {
            printf("Não foi possível realocar a memória");
            return -1;
        }
    }

    (*n_notas)--;
    (*notas) = realloc((*notas), (*n_notas) * sizeof(nota));
    if ((*notas) == NULL) {
        printf("Não foi possível realocar a memória");
        return -1;
    }
    return 0;
}

int main (void) {
    int n_notas = 1;
    int *ptr_n_notas = &n_notas;
    nota *notas = malloc(n_notas * sizeof(nota));
    char file_name[10];
    printf("Digite o arquivo para ler as notas: \n");
    scanf("%s", file_name);
    FILE *arq = fopen(file_name, "r");
    if (arq == NULL) {
        printf("Não consegui abrir o arquivo!\n");
        return -1;
    }

    int conseguiu_ler = ler_notas(arq, ptr_n_notas, &notas);
    if (conseguiu_ler == -1) {
        return -1;
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
    printf("%d", n_notas);
    fclose(arq);
}