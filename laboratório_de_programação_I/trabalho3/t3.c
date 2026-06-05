#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tela.h"

//JEAN
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

typedef enum { move, edita_texto, fim, edita_tbusca, edita_cor } modo_t;

typedef struct {
  modo_t modo;

  nota *notas;
  int n_notas;

  int cursor_x;
  int cursor_y;

  nota *nota_corrente;
  nota nota_removida;
  int existe_nota_removida;

  char texto_busca[50];

  int largura_fundo;
  int altura_fundo;

  unsigned char cor_edicao[3];
} estado_t;

int realoca_memoria(estado_t *e, int dmemoria) {
  if (e->n_notas + dmemoria == 0) {
    printf("Você não deve excluir todas as notas!");
    return 0;
  }
  nota *tmp = e->notas;
  tmp = realloc(tmp, (e->n_notas + dmemoria) * sizeof(nota));
  if (tmp == NULL) {
    printf("Não foi possível realocar memória.\n");
    return 0;
  }
  e->notas = tmp;
  e->n_notas += dmemoria;
  return 1;
}

void atualiza_nota_corrente (estado_t *e) {
  retangulo atual;
  e->nota_corrente = NULL;
  for (int i = 0; i < e->n_notas; i++) {
    atual = e->notas[i].retangulo;
    if (e->cursor_x >= atual.x && e->cursor_x <= (atual.x + atual.largura) && e->cursor_y >= atual.y && e->cursor_y < atual.y + atual.altura) {
      e->nota_corrente = &(e->notas[i]);
    }
  }
}

void mover_inicio (estado_t *e) {
  int indice = e->nota_corrente - e->notas;
  nota tmp = *(e->nota_corrente);
  for (int i = indice; i > 0; i--) {
    e->notas[i] = e->notas[i - 1];
  }
  e->notas[0] = tmp;
  atualiza_nota_corrente(e);
}

void mover_fim (estado_t *e) {
  int indice = e->nota_corrente - e->notas;
  nota tmp = *(e->nota_corrente);
  for (int i = indice; i < e->n_notas - 1; i++) {
    e->notas[i] = e->notas[i + 1];
  }
  e->notas[e->n_notas - 1] = tmp;
  atualiza_nota_corrente(e);
}

void gravar_notas (estado_t *e, char file_name[]) {
  FILE *arq = fopen(file_name, "w");
  if (arq == NULL) {
    printf("Não consegui abrir o arquivo para gravar\n");
    return;
  }
  for (int i = 0; i < e->n_notas; i++) {
    fprintf(arq, "%s %hhu %hhu %hhu %d %d %d %d \"%s\"\n", 
            e->notas[i].etiqueta, e->notas[i].cor.r, e->notas[i].cor.g, e->notas[i].cor.b, e->notas[i].retangulo.x, e->notas[i].retangulo.y,
          e->notas[i].retangulo.largura, e->notas[i].retangulo.altura, e->notas[i].texto);
  }
  fclose(arq);
}

void nova_nota (estado_t *e) {
  if(!realoca_memoria(e, 1)) return;


  nota *n = &(e->notas[e->n_notas - 1]);
  strcpy(n->texto, "K");
  strcpy(n->etiqueta, "A00");
  n->cor.r = 120;
  n->cor.g = 120;
  n->cor.b = 120;
  n->retangulo.x = e->cursor_x;
  n->retangulo.y = e->cursor_y;
  n->retangulo.largura = 5;
  n->retangulo.altura = 5;
}

void deleta_nota (estado_t *e) {
  int indice;
  if (e->nota_corrente != NULL) {
    e->nota_removida = *(e->nota_corrente);
    indice = e->nota_corrente - e->notas;
    for(int i = indice; i < e->n_notas - 1; i++) {
      e->notas[i] = e->notas[i + 1];
    }
    if(!realoca_memoria(e, -1)) return;
    e->existe_nota_removida = 1;
  }
  atualiza_nota_corrente(e);
}

void inserir_nota_removida (estado_t *e) {
  if (e->existe_nota_removida) {
    if(!realoca_memoria(e, 1)) return;
    e->nota_removida.retangulo.x = e->cursor_x;
    e->nota_removida.retangulo.y = e->cursor_y;
    e->notas[e->n_notas - 1] = e->nota_removida;
    e->existe_nota_removida = 0;
    atualiza_nota_corrente(e);
  }
}


// desenha um retângulo colorido com texto dentro
void desenha_ret(int l, int c, int dl, int dc,
                int r, int g, int b,
                char s[])
{
  // desenha o retângulo com espaços, selecionando a cor como fundo
  t_corfundo(r, g, b);
  // imprime dl linhas com dc espaços cada
  for (int i = 0; i < dl; i++) {
    // posiciona o cursor onde inicia essa linha do retângulo
    t_lincol(l + i, c);
    // %10s imprime pelo menos 10 caracteres, completando com espaços
    //   se a string for menor
    // %.10s imprime no máximo 10 caracteres
    // %10.10s imprime exatamente 10 caracteres
    // se um número for substituído por '*', será pego no próximo argumento
    printf("%*s", dc, ""); // imprime dc espaços (podia tbém ser um for)
  }

  // escreve o tamanho da tela no canto do retângulo
  int alt, larg;
  t_tamanho(&alt, &larg);
  t_lincol(l, c);
  printf("%dx%d", alt, larg);

  // escreve s no centro do retângulo
  t_lincol(l + dl/2, c + dc/2 - strlen(s)/2);
  if ((r + g + b)/3 < 120) {
    t_cortexto(255, 255, 255);
  }
  else {
    t_cortexto(0, 0, 0);
  }
  printf("%s", s);
}

void muda_modo(estado_t *e, modo_t novo_modo)
{
  e->modo = novo_modo;
}

// modo "move"

void tela_move(estado_t *e)
{
  t_limpa();
  desenha_ret(1, 1, e->altura_fundo, e->largura_fundo, 10, 20, 30, "fundo");
  for (int i = 0; i < e->n_notas; i++) {
    if (strcmp(e->texto_busca, "") == 0 || strstr(e->notas[i].texto, e->texto_busca)) {
      desenha_ret(e->notas[i].retangulo.y, e->notas[i].retangulo.x, e->notas[i].retangulo.altura, e->notas[i].retangulo.largura,
                  e->notas[i].cor.r, e->notas[i].cor.g, e->notas[i].cor.b, e->notas[i].texto);
      }
  }
  if (e->nota_corrente != NULL) {
    if ((e->nota_corrente->cor.r + e->nota_corrente->cor.g + e->nota_corrente->cor.b)/3 < 120) {
      t_corfundo(255, 255, 255);
      t_cortexto(0, 0, 0);
    }
    else {
      t_corfundo(0, 0, 0);
      t_cortexto(255, 255, 255);
    }
    t_lincol(e->nota_corrente->retangulo.y, e->nota_corrente->retangulo.x + e->nota_corrente->retangulo.largura - 1);
    printf("c");
  }
  // põe o cursor no centro do retângulo
  t_lincol(e->cursor_y, e->cursor_x);
}

void move_direita(estado_t *e)
{
  if(e->cursor_x < e->largura_fundo + 1) {
    e->cursor_x++;
  }
}

void move_esquerda(estado_t *e)
{
  if (e->cursor_x > 1) {
    e->cursor_x--;
  }
}

void move_cima(estado_t *e)
{
  if (e->cursor_y > 1) {
    e->cursor_y--;
  }
}

void move_baixo(estado_t *e)
{
  if(e->cursor_y < e->altura_fundo) {
    e->cursor_y++;
  }
}

void move_nota_direita(estado_t *e)
{
    if (e->nota_corrente != NULL && 
    e->nota_corrente->retangulo.x + e->nota_corrente->retangulo.largura < e->largura_fundo + 1) {
        e->nota_corrente->retangulo.x++;
    }
}

void move_nota_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.x > 1) {
      e->nota_corrente->retangulo.x--;
    }
}

void move_nota_cima(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.y > 1) {
      e->nota_corrente->retangulo.y--;
    }
}

void move_nota_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL &&
    e->nota_corrente->retangulo.y + e->nota_corrente->retangulo.altura < e->altura_fundo + 1) {
        e->nota_corrente->retangulo.y++;
    }
}

void diminui_direita(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.largura > 1) {
        e->nota_corrente->retangulo.largura--;
    }
}

void diminui_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.largura > 1) {
        e->nota_corrente->retangulo.largura--;
        e->nota_corrente->retangulo.x++;
    }
}

void diminui_cima(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.altura > 1) {
        e->nota_corrente->retangulo.altura--;
        e->nota_corrente->retangulo.y++;
    }
}

void diminui_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.altura > 1) {
        e->nota_corrente->retangulo.altura--;
    
    }
}

void aumenta_direita(estado_t *e)
{
    if (e->nota_corrente != NULL &&
    e->nota_corrente->retangulo.x + e->nota_corrente->retangulo.largura < e->largura_fundo + 1) {
        e->nota_corrente->retangulo.largura++;
    }
}

void aumenta_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.x > 1) {
        e->nota_corrente->retangulo.largura++;
        e->nota_corrente->retangulo.x--;
    }
}

void aumenta_cima(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.y > 1) {
        e->nota_corrente->retangulo.altura++;
        e->nota_corrente->retangulo.y--;
    }
}

void aumenta_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL &&
    e->nota_corrente->retangulo.y + e->nota_corrente->retangulo.altura < e->altura_fundo + 1) {
        e->nota_corrente->retangulo.altura++;
    }
}

void exec_move(estado_t *e, char file_name[])
{
  atualiza_nota_corrente(e);
  // modo sem necessidade de manter valores locais, implementado sem
  //   laço próprio

  // desenha a tela
  //PAREI AQUI
  tela_move(e);

  // lê um comando
  tecla_t tec = t_tecla();

  // realiza uma ação conforme o comando lido
  switch (tec) {
    case T_ESQUERDA:
      move_esquerda(e);
      break;
    case T_DIREITA:
      move_direita(e);
      break;
    case T_CIMA:
      move_cima(e);
      break;
    case T_BAIXO:
      move_baixo(e);
      break;
    case T_SHIFT_ESQUERDA:
      move_nota_esquerda(e);
      break;
    case T_SHIFT_DIREITA:
      move_nota_direita(e);
      break;
    case T_SHIFT_CIMA:
      move_nota_cima(e);
      break;
    case T_SHIFT_BAIXO:
      move_nota_baixo(e);
      break;
    case T_ALT_ESQUERDA:
      diminui_esquerda(e);
      break;
    case T_ALT_DIREITA:
      diminui_direita(e);
      break;
    case T_ALT_CIMA:
      diminui_cima(e);
      break;
    case T_ALT_BAIXO:
      diminui_baixo(e);
      break;
    case T_CTRL_ESQUERDA:
      aumenta_esquerda(e);
      break;
    case T_CTRL_DIREITA:
      aumenta_direita(e);
      break;
    case T_CTRL_CIMA:
      aumenta_cima(e);
      break;
    case T_CTRL_BAIXO:
      aumenta_baixo(e);
      break;
    case T_DEL:
      deleta_nota(e);
      break;
    case T_INS:
      inserir_nota_removida(e);
      break;
    case 'i':
      mover_inicio(e);
      break;
    case 'f':
      mover_fim(e);
      break;
    case 'g':
      gravar_notas(e, file_name);
      break;
    case 'n':
      nova_nota(e);
      break;
    case 'b':
      muda_modo(e, edita_tbusca);
      break;
    case 'e':
      muda_modo(e, edita_texto);
      break;
    case 'c':
      muda_modo(e, edita_cor);
      break;
    case T_ESC:
      muda_modo(e, fim);
      break;
    default: ;
  }
}

// modo edita nota

void tela_edita_texto(estado_t *e, char texto[])
{
  t_limpa();
  desenha_ret(2, 3, 20, 50, 10, 20, 30, "fundo");
  char texto_temp[200] = "";
  desenha_ret(e->nota_corrente->retangulo.y, e->nota_corrente->retangulo.x, e->nota_corrente->retangulo.altura, e->nota_corrente->retangulo.largura, 150, 49, 97, texto);
}

void remove_ultimo_caractere(char s[])
{
  int l = strlen(s);
  if (l > 0) {
    s[l - 1] = '\0';
  }
}

void insere_ultimo_caractere(char s[], char c)
{
  int l = strlen(s);
  if (strlen(s) < 149) {
    s[l] = c;
    s[l + 1] = '\0';
  }
}

void grava_texto(estado_t *e, char txt[]) {
  strcpy(e->nota_corrente->texto, txt);
}


void exec_edita_texto(estado_t *e)
{
  // modo com necessidade de manter valores locais, implementado com
  //   laço próprio
  char txt[200] = "";
  strcpy(txt, e->nota_corrente->texto);

  int cursor_edita_x = strlen(e->nota_corrente->texto);

  
  while (e->modo == edita_texto) {
    // desenha a tela
    tela_edita_texto(e, txt);

    // lê um comando
    tecla_t tec = t_tecla();

    // realiza uma ação conforme o comando lido
    if (tec == T_ESC) {
      muda_modo(e, move);
    } else if (tec == T_BS) {
      remove_ultimo_caractere(txt);
    } else if (tec >= 'a' && tec <= 'z') {
      insere_ultimo_caractere(txt, tec);
    } else if (tec == T_ENTER) {
      grava_texto(e, txt);
      muda_modo(e, move);
    }
  }
}

// modo edita texto de busca
void deleta_caracter (char txt[], int cur) {
  int l = strlen(txt);
  if (l > 0) {
    for (int i = (l - cur); i < l; i++) {
      txt[i] = txt[i+1];
    }
  } 
}

void tela_edita_tbusca (char txt[], int cursor) {
  t_limpa();
  desenha_ret(2, 2, 20, 50, 10, 20, 30, txt);
  t_lincol(2 + 20/2, 2 + 50/2 + (strlen(txt) + 1)/2 - cursor);
}

void exec_edita_tbusca(estado_t *e)
{
  // modo com necessidade de manter valores locais, implementado com
  //   laço próprio
  char txt[200] = "";
  strcpy(txt, e->texto_busca);

  int cursor = 0;

  
  while (e->modo == edita_tbusca) {
    // desenha a tela
    tela_edita_tbusca(txt, cursor);

    // lê um comando
    tecla_t tec = t_tecla();

    // realiza uma ação conforme o comando lido
    if (tec == T_ESC) {
      strcpy(e->texto_busca, "");
      muda_modo(e, move);
    } else if (tec == T_BS) {
      remove_ultimo_caractere(txt);
    } else if ((tec >= 'a' && tec <= 'z') || (tec >= 'A' && tec <= 'Z')) {
      insere_ultimo_caractere(txt, tec);
    } else if (tec == T_ENTER) {
      strcpy(e->texto_busca, txt);
      muda_modo(e, move);
    } else if (tec == T_DIREITA) {
      if (cursor > 0) {
        cursor--;
      }
    } else if (tec == T_ESQUERDA) {
      if (cursor < strlen(txt)) {
        cursor++;
      }
    } else if (tec == T_DEL) {
      deleta_caracter(txt, cursor);
    } else if (tec == T_HOME) {
      cursor = strlen(txt);
    } else if (tec == T_END) {
      cursor = 0;
    }
  }
}

void tela_edita_cor (estado_t *e, int c) {
  t_limpa();
  desenha_ret(1, 1, 5, 39, 0, 0, 0, "  r    g    b  ");
  t_lincol(3, 15 + 5*c);
  if (c == 0) {
    t_cortexto(255, 0, 0);
    printf("r");
  }
  else if (c == 1) {
    t_cortexto(0, 255, 0);
    printf("g");
  }
  else if (c == 2) {
    t_cortexto(0, 0, 255);
    printf("b");
  }
  t_lincol(4, 14);
  t_cortexto(255, 255, 255);
  printf("%03d  %03d  %03d", e->cor_edicao[0], e->cor_edicao[1], e->cor_edicao[2]);
  t_lincol(5, 34);
  printf("EX ");
  t_corfundo(e->cor_edicao[0], e->cor_edicao[1], e->cor_edicao[2]);
  printf(" ");
  t_lincol(1, 9);
}

unsigned char soma_digito_cor (tecla_t tec, unsigned char c) {
  if (c * 10 + (tec - '0') <= 255) {
    return c * 10 + (tec - '0');
  }
  else {
    return c;
  }
}

void exec_edita_cor (estado_t *e) {
  int ultimo_foi_digito = 0;
  int cor_atual = 0;
  while (e->modo == edita_cor) {
    tela_edita_cor(e, cor_atual);
    tecla_t tec = t_tecla();

    if (tec == 'e' || tec == 'r') {
      cor_atual = 0;
    } else if (tec == 'v' || tec == 'g') {
      cor_atual = 1;
    } else if (tec == 'a' || tec == 'b') {
      cor_atual = 2;
    } else if (tec == T_CIMA) {
      if (e->cor_edicao[cor_atual] < 255) {
        e->cor_edicao[cor_atual]++;
      }
    } else if (tec == T_SHIFT_CIMA) {
      if (e->cor_edicao[cor_atual] <= 225) {
        e->cor_edicao[cor_atual] += 30;
      }
      else {
        e->cor_edicao[cor_atual] = 255;
      }
    } else if (tec == T_BAIXO) {
      if (e->cor_edicao[cor_atual] > 0) {
        e->cor_edicao[cor_atual]--;
      }
    } else if (tec == T_SHIFT_BAIXO) {
      if (e->cor_edicao[cor_atual] >= 30) {
        e->cor_edicao[cor_atual] -= 30;
      }
      else {
        e->cor_edicao[cor_atual] = 0;
      }
    } else if (tec == T_ESQUERDA) {
      cor_atual--;
      if (cor_atual < 0) {
        cor_atual = 2;
      }
    } else if (tec == T_DIREITA) {
      cor_atual++;
      if (cor_atual > 2) {
        cor_atual = 0;
      }
    } else if (tec == T_ENTER) {
      e->nota_corrente->cor.r = e->cor_edicao[0];
      e->nota_corrente->cor.g = e->cor_edicao[1];
      e->nota_corrente->cor.b = e->cor_edicao[2];
      muda_modo(e, move);
    } else if (tec == T_ESC) {
      muda_modo(e, move);
    } else if (tec >= '0' && tec <= '9') {
      if (!ultimo_foi_digito) {
        e->cor_edicao[cor_atual] = tec - '0';
        ultimo_foi_digito = 1;
      }
      else {
        e->cor_edicao[cor_atual] = soma_digito_cor(tec, e->cor_edicao[cor_atual]);
      }
    }
    //AQUI UTILIZEI SHIFT+DIREITA PORQUE NÃO HAVIA SHIFT+ENTER
    else if (tec == T_SHIFT_DIREITA) {
      for (int i = 0; i < e->n_notas; i++) {
        if (strstr(e->notas[i].texto, e->texto_busca)) {
          e->notas[i].cor.r = e->cor_edicao[0];
          e->notas[i].cor.g = e->cor_edicao[1];
          e->notas[i].cor.b = e->cor_edicao[2];
        }
      }
      muda_modo(e, move);
    }
  }
}

//JEAN
int ler_notas (FILE *arq, estado_t *e) {
    int cont, c;
    while (fscanf(arq, "%s %hhu %hhu %hhu %d %d %d %d ", 
        e->notas[e->n_notas - 1].etiqueta,
        &e->notas[e->n_notas - 1].cor.r,
        &e->notas[e->n_notas - 1].cor.g,
        &e->notas[e->n_notas - 1].cor.b,
        &e->notas[e->n_notas - 1].retangulo.x,
        &e->notas[e->n_notas - 1].retangulo.y,
        &e->notas[e->n_notas - 1].retangulo.largura,
        &e->notas[e->n_notas - 1].retangulo.altura
    ) == 8) {
        cont = 0;

        //Serve para ignorar a primeira " da palavra
        fgetc(arq);


        //AINDA PRECISA RESOLVER QUANDO UMA LINHA TEM MULTIPLAS ""
        while ((c = fgetc(arq)) != '"')
        {
            e->notas[e->n_notas - 1].texto[cont] = c;
            cont++;
        }
        e->notas[e->n_notas - 1].texto[cont] = '\0';
        
        if(!realoca_memoria(e, 1)) return -1;
    }

    if(!realoca_memoria(e, -1)) return -1;
    return 0;
}


int main (void) {

    // Le o arquivo
    char file_name[30];
    printf("Digite o arquivo para ler as notas: \n");
    scanf("%s", file_name);
    FILE *arq = fopen(file_name, "r");
    if (arq == NULL) {
        printf("Não consegui abrir o arquivo!\n");
        return -1;
    }

    estado_t estado = { move, NULL, 1, 5, 5, NULL, {0}, 0, "", 70, 40, {0}};
    estado.notas = malloc(sizeof(nota));
    if (estado.notas == NULL) {
      printf("Não foi possível alocar memória.\n");
      return -1;
    }

    int conseguiu_ler = ler_notas(arq, &estado);
    if (conseguiu_ler == -1) {
      free(estado.notas);
        return -1;
    }

    fclose(arq);


    t_inicia();
    while (estado.modo != fim) {
        switch (estado.modo) {
        case move:
            exec_move(&estado, file_name);
            break;
        case edita_texto:
            exec_edita_texto(&estado);
            break;
        case edita_tbusca:
            exec_edita_tbusca(&estado);
            break;
        case edita_cor:
            exec_edita_cor(&estado);
            break;
        default:
            break;
        }
        
    }
    // retorna a tela ao modo normal
    t_fim();
    free(estado.notas);

    //Escreve as notas atualizadas no arquivo
}