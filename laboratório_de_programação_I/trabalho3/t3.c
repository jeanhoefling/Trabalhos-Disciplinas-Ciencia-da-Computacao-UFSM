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

typedef enum { principal, edita_texto, fim, edita_tbusca, edita_cor, edita_etiqueta, edita_ebusca } modo_t;

typedef struct {
  modo_t modo;

  nota *notas;
  int n_notas;

  int cursor_x;
  int cursor_y;

  nota *nota_corrente;
  nota nota_removida;
  int existe_nota_removida;

  char etiqueta_busca[4];
  char texto_busca[50];

  int largura_fundo;
  int altura_fundo;

  unsigned char cor_edicao[3];
  char etiqueta_edicao[4];
} estado_t;

// FUNÇÕES DE UTILIDADE GERAL

int realoca_memoria(estado_t *e, int dmemoria) {

  // Caso especifico em que se deseja deletar a última nota
  if (e->n_notas == 1 && dmemoria == -1) {
    free(e->notas);
    e->notas = NULL;
    e->n_notas = 0;
    return 1;
  }

  // Caso geral
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

// desenha um retângulo colorido com texto dentro
void desenha_ret(int y, int x, int altura, int largura,
                int r, int g, int b,
                char s[], char e[])
{
  t_corfundo(r, g, b);

  for (int i = 0; i < altura; i++) {
    t_lincol(y + i, x);
    printf("%*s", largura, ""); 
  }

  if (largura >= 3) {
    t_lincol(y, x);
    printf("%s", e);
  }


  // verifica se o texto é maior que o espaço da nota
  int texto_maior_que_nota;
  //linhas necessárias para escrever o texto no retangulo
  int lnt;

  if (strlen(s) > largura * altura) {
    texto_maior_que_nota = 1;
    lnt = altura;
  }
  else {
    texto_maior_que_nota = 0;
    lnt = (strlen(s) / largura) == ((float) strlen(s) / largura) ? strlen(s)/largura : strlen(s)/largura + 1;
  }

  if ((r + g + b)/3 < 120) {
    t_cortexto(255, 255, 255);
  }
  else {
    t_cortexto(0, 0, 0);
  }

  int cont = 0;
  for (int i = 0; i < lnt - 1; i++) {
    t_lincol(y + altura/2 - lnt/2 + i, x);
    for (int j = cont; j < cont + largura; j++) {
      printf("%c", s[j]);
    }
    cont += largura;
  }

  //REVER BEM ESSE TLINCOL, FUNCIONA MAS TA ESTRANHO

  if (texto_maior_que_nota) {
    t_lincol(y + altura/2 + lnt/2 - 1 + lnt%2, x);
  }
  else {
    t_lincol(y + altura/2 + lnt/2 - 1 + lnt%2, x + largura/2 - (strlen(s) - cont)/2);
  }

  for (int i = cont; i < strlen(s); i++) {
    printf("%c", s[i]);
    if (i >= largura * altura - 1) {
      break;
    }
  }
}

void muda_modo(estado_t *e, modo_t novo_modo)
{
  e->modo = novo_modo;
}


// FUNÇÕES DO MODO PRINCIPAL

void atualiza_nota_corrente (estado_t *e) {
  retangulo atual;
  e->nota_corrente = NULL;
  for (int i = 0; i < e->n_notas; i++) {
    if (!(strstr(e->notas[i].texto, e->texto_busca)) || strncmp(e->notas[i].etiqueta, e->etiqueta_busca, strlen(e->etiqueta_busca)) != 0) {
      continue;
    }
    atual = e->notas[i].retangulo;
    if (e->cursor_x >= atual.x && e->cursor_x < (atual.x + atual.largura) && e->cursor_y >= atual.y && e->cursor_y < atual.y + atual.altura) {
      e->nota_corrente = &(e->notas[i]);
    }
  }
}

void destaca_nota_corrente(estado_t *e)
{
    retangulo r = e->nota_corrente->retangulo;
    cor c = e->nota_corrente->cor;

    if ((c.r + c.g + c.b)/3 < 120) {
      t_corfundo(255,255,255);
      t_cortexto(0,0,0);
    }
    else {
      t_corfundo(0,0,0);
      t_cortexto(255,255,255);  
    }


    // topo
    if(r.y > 1) {
      for(int x = r.x; x < r.x + r.largura; x++) {
          t_lincol(r.y - 1, x);
          printf("*");
      }
    }

    // baixo
    if (r.y + r.altura < e->altura_fundo + 1) {
      for(int x = r.x; x < r.x + r.largura; x++) {
        t_lincol(r.y + r.altura, x);
        printf("*");
      }
    }

    // lateral esquerda
    if (r.x > 1) {
      for(int y = r.y; y < r.y + r.altura; y++) {
          t_lincol(y, r.x - 1);
          printf("*");
      }
    }

    // lateral direita
    if (r.x + r.largura < e->largura_fundo + 1) {
      for(int y = r.y; y < r.y + r.altura; y++) {
          t_lincol(y, r.x + r.largura);
          printf("*");
      }
    }
}

void mover_inicio (estado_t *e) {
  if (e->nota_corrente == NULL) {
    return;
  }
  int indice = e->nota_corrente - e->notas;
  nota tmp = *(e->nota_corrente);
  for (int i = indice; i > 0; i--) {
    e->notas[i] = e->notas[i - 1];
  }
  e->notas[0] = tmp;
}

void mover_fim (estado_t *e) {
  if (e->nota_corrente == NULL) {
    return;
  }
  int indice = e->nota_corrente - e->notas;
  nota tmp = *(e->nota_corrente);
  for (int i = indice; i < e->n_notas - 1; i++) {
    e->notas[i] = e->notas[i + 1];
  }
  e->notas[e->n_notas - 1] = tmp;
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
  strcpy(n->texto, "Nova nota");
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
  if (e->nota_corrente != NULL && e->n_notas > 0) {
    e->nota_removida = *(e->nota_corrente);
    indice = e->nota_corrente - e->notas;
    for(int i = indice; i < e->n_notas - 1; i++) {
      e->notas[i] = e->notas[i + 1];
    }
    if(!realoca_memoria(e, -1)) return;
    e->existe_nota_removida = 1;
  }
}

void inserir_nota_removida (estado_t *e) {
  if (e->existe_nota_removida) {
    if(!realoca_memoria(e, 1)) return;
    e->nota_removida.retangulo.x = e->cursor_x;
    e->nota_removida.retangulo.y = e->cursor_y;
    e->notas[e->n_notas - 1] = e->nota_removida;
    e->existe_nota_removida = 0;
  }
}

void posiciona_cursor_ultima_nota(estado_t *e) {
  for (int i = e->n_notas - 1; i >= 0; i--) {
    if(strstr(e->notas[i].texto, e->texto_busca) && strncmp(e->notas[i].etiqueta, e->etiqueta_busca, strlen(e->etiqueta_busca)) == 0) {
      e->cursor_x = e->notas[i].retangulo.x;
      e->cursor_y = e->notas[i].retangulo.y;
      return;
    }
  }
}

void move_direita(estado_t *e)
{
  if(e->cursor_x < e->largura_fundo) {
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
    move_direita(e);
}

void move_nota_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.x > 1) {
      e->nota_corrente->retangulo.x--;
    }
    move_esquerda(e);
}

void move_nota_cima(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.y > 1) {
      e->nota_corrente->retangulo.y--;
    }
    move_cima(e);
}

void move_nota_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL &&
    e->nota_corrente->retangulo.y + e->nota_corrente->retangulo.altura < e->altura_fundo + 1) {
        e->nota_corrente->retangulo.y++;
    }
    move_baixo(e);
}

void diminui_direita(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.largura > 1) {
        e->nota_corrente->retangulo.largura--;
        if (e->cursor_x >= e->nota_corrente->retangulo.x + e->nota_corrente->retangulo.largura) {
          move_esquerda(e);
        }
    }
}

void diminui_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.largura > 1) {
        e->nota_corrente->retangulo.largura--;
        e->nota_corrente->retangulo.x++;
        if (e->cursor_x < e->nota_corrente->retangulo.x) {
          move_direita(e);
        }
    }
}

void diminui_cima(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.altura > 1) {
        e->nota_corrente->retangulo.altura--;
        e->nota_corrente->retangulo.y++;
        if (e->cursor_y < e->nota_corrente->retangulo.y) {
          move_baixo(e);
        }
    }
}

void diminui_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL && e->nota_corrente->retangulo.altura > 1) {
        e->nota_corrente->retangulo.altura--;
        if (e->cursor_y >= e->nota_corrente->retangulo.y + e->nota_corrente->retangulo.altura) {
          move_cima(e);
        }
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

void tela_principal(estado_t *e)
{
  t_limpa();
  desenha_ret(1, 1, e->altura_fundo, e->largura_fundo, 10, 20, 30, "fundo", "");
  for (int i = 0; i < e->n_notas; i++) {
    if (strstr(e->notas[i].texto, e->texto_busca) && strncmp(e->notas[i].etiqueta, e->etiqueta_busca, strlen(e->etiqueta_busca)) == 0) {
      desenha_ret(e->notas[i].retangulo.y, e->notas[i].retangulo.x, e->notas[i].retangulo.altura, e->notas[i].retangulo.largura,
                  e->notas[i].cor.r, e->notas[i].cor.g, e->notas[i].cor.b, e->notas[i].texto, e->notas[i].etiqueta);
      }
  }
  if (e->nota_corrente != NULL) {
    destaca_nota_corrente(e);
  }
  t_lincol(e->cursor_y, e->cursor_x);
}

void exec_principal(estado_t *e, char file_name[])
{
  atualiza_nota_corrente(e);
  tela_principal(e);

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
    case T_S_ESQUERDA:
      move_nota_esquerda(e);
      break;
    case T_S_DIREITA:
      move_nota_direita(e);
      break;
    case T_S_CIMA:
      move_nota_cima(e);
      break;
    case T_S_BAIXO:
      move_nota_baixo(e);
      break;
    case T_A_ESQUERDA:
      diminui_esquerda(e);
      break;
    case T_A_DIREITA:
      diminui_direita(e);
      break;
    case T_A_CIMA:
      diminui_cima(e);
      break;
    case T_A_BAIXO:
      diminui_baixo(e);
      break;
    case T_C_ESQUERDA:
      aumenta_esquerda(e);
      break;
    case T_C_DIREITA:
      aumenta_direita(e);
      break;
    case T_C_CIMA:
      aumenta_cima(e);
      break;
    case T_C_BAIXO:
      aumenta_baixo(e);
      break;
    case T_DEL:
      deleta_nota(e);
      break;
    case T_INS:
      inserir_nota_removida(e);
      break;
    case 'p':
      posiciona_cursor_ultima_nota(e);
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
    case 'B':
      muda_modo(e, edita_ebusca);
      break;
    case 'e':
      muda_modo(e, edita_texto);
      break;
    case 't':
      muda_modo(e, edita_etiqueta);
      break;
    case 'c':
      muda_modo(e, edita_cor);
      break;
    case T_ESC:
      gravar_notas(e, file_name);
      muda_modo(e, fim);
      break;
    default: 
      break;
  }
}

// FUNÇÕES MODO EDITA TEXTO
void move_cursor_edita (int *cur, int dx, char texto[]) {
  if ((*cur) + dx >= 0 && (*cur) + dx <= strlen(texto)) {
    (*cur) += dx;
  }
}

void remove_caractere(char s[], int cur)
{
  int l = strlen(s);
  for (int i = cur; i < l - 1; i++) {
    s[i] = s[i + 1];
  }
  s[l - 1] = '\0';
}

void insere_caractere(char s[], char c, int cur)
{
  int l = strlen(s);
  if (strlen(s) < 149) {
    for(int i = l + 1; i > cur; i--) {
      s[i] = s[i - 1];
    }
    s[cur] = c;
  }
}

void grava_texto(estado_t *e, char txt[]) {
  strcpy(e->nota_corrente->texto, txt);
}

void tela_edita_texto(estado_t *e, char texto[], int cur)
{
  t_limpa();
  nota *n = e->nota_corrente;
  //pode ser melhorada para mostrar a nota corrente
  desenha_ret(1, 1, n->retangulo.altura, n->retangulo.largura, n->cor.r, n->cor.g, n->cor.b, texto, n->etiqueta);
  t_lincol(2 + n->retangulo.altura, 1);
  printf("%s", texto);
  t_lincol(2 + n->retangulo.altura, cur + 1);
}

void exec_edita_texto(estado_t *e)
{
  if (e->nota_corrente == NULL) {
    muda_modo(e, principal);
  }
  else {
    char txt[200] = "";
    strcpy(txt, e->nota_corrente->texto);

    int cursor_edita = strlen(e->nota_corrente->texto);

    
    while (e->modo == edita_texto) {
      // desenha a tela
      tela_edita_texto(e, txt, cursor_edita);

      // lê um comando
      tecla_t tec = t_tecla();

      // realiza uma ação conforme o comando lido
      if (tec == T_ESC) {
        muda_modo(e, principal);
      } else if (tec == T_BS) {
        if (cursor_edita > 0) {
          move_cursor_edita(&cursor_edita, -1, txt);
          remove_caractere(txt, cursor_edita);
        }
      } else if ((tec >= 32 && tec <= 126)) {
        insere_caractere(txt, tec, cursor_edita);
        move_cursor_edita(&cursor_edita, 1, txt);
      } else if (tec == T_ENTER) {
        grava_texto(e, txt);
        muda_modo(e, principal);
      } else if (tec == T_ESQUERDA) {
        move_cursor_edita(&cursor_edita, -1, txt);
      } else if (tec == T_DIREITA) {
        move_cursor_edita(&cursor_edita, 1, txt);
      } else if (tec ==  T_DEL) {
        if (cursor_edita < strlen(txt)) {
          remove_caractere(txt, cursor_edita);
        }
      }
    }
  }
}

// FUNÇÕES EDITA TEXTO DE BUSCA

void tela_edita_tbusca (char txt[], int cursor) {
  t_limpa();
  t_lincol(1, 1);
  printf("Edite o texto de busca:");
  t_lincol(2, 1);
  printf("%s", txt);
  t_lincol(2, cursor + 1);
}

void exec_edita_tbusca(estado_t *e)
{
  char txt[200] = "";
  strcpy(txt, e->texto_busca);

  int cursor = strlen(txt);

  
  while (e->modo == edita_tbusca) {
    tela_edita_tbusca(txt, cursor);
    tecla_t tec = t_tecla();

    if (tec == T_ESC) {
      strcpy(e->texto_busca, "");
      muda_modo(e, principal);
    } else if (tec == T_BS) {
      if (cursor > 0) {
        cursor--;
        remove_caractere(txt, cursor);
      }
    } else if (tec >= 32 && tec <= 126) {
      insere_caractere(txt, tec, cursor);
      cursor++;
    } else if (tec == T_ENTER) {
      strcpy(e->texto_busca, txt);
      muda_modo(e, principal);
    } else if (tec == T_ESQUERDA) {
      if (cursor > 0) {
        cursor--;
      }
    } else if (tec == T_DIREITA) {
      if (cursor < strlen(txt)) {
        cursor++;
      }
    } else if (tec == T_DEL) {
      if (cursor < strlen(txt)) {
        remove_caractere(txt, cursor);
      }
    } else if (tec == T_HOME) {
      cursor = 0;
    } else if (tec == T_END) {
      cursor = strlen(txt);
    }
  }
}


// FUNÇÕES EDITA ETIQUETA DE BUSCA

void tela_edita_ebusca (char txt[], int cursor) {
  t_limpa();
  t_lincol(1, 1);
  printf("Edite a etiqueta de busca:");
  t_lincol(2, 1);
  printf("%s", txt);
  t_lincol(2, cursor + 1);
}

void exec_edita_ebusca(estado_t *e)
{
  char txt[4] = "";
  strcpy(txt, e->etiqueta_busca);

  int cursor;

  
  while (e->modo == edita_ebusca) {
    cursor = strlen(txt);
    tela_edita_ebusca(txt, cursor);
    tecla_t tec = t_tecla();

    if (tec == T_ESC) {
      strcpy(e->etiqueta_busca, "");
      muda_modo(e, principal);
    } else if (tec == T_BS) {
      remove_caractere(txt, cursor);
    } else if ((tec >= 'A' && tec <= 'Z') || (tec >= '0' && tec <= '9')) {
      if (strlen(txt) < 3) {
         insere_caractere(txt, tec, cursor);
      }
    } else if (tec == T_ENTER) {
      strcpy(e->etiqueta_busca, txt);
      muda_modo(e, principal);
    } 
  }
}

// FUNÇÕES EDITA ETIQUETA DE EDICAO

/* Essa função serve tanto para edição de cor quanto de etiqueta, ela é usada no caso em que se deseja
alterar todas as notas visiveis. Se c_e for 1 altera as cores, se for 2 altera etiquetas. */
void altera_notas_visiveis(estado_t *e, int c_e) {
  for (int i = 0; i < e->n_notas; i++) {
    if (strstr(e->notas[i].texto, e->texto_busca) && strncmp(e->notas[i].etiqueta, e->etiqueta_busca, strlen(e->etiqueta_busca)) == 0) {
        if (c_e == 2) {
          strcpy(e->notas[i].etiqueta, e->etiqueta_edicao);
        }
        else if (c_e == 1) {
          e->notas[i].cor.r = e->cor_edicao[0];
          e->notas[i].cor.g = e->cor_edicao[1];
          e->notas[i].cor.b = e->cor_edicao[2];
        }
      }
  }
}

void tela_edita_etiqueta (char txt[], int cursor) {
  t_limpa();
  t_lincol(1, 1);
  printf("Edite a etiqueta:");
  t_lincol(2, 1);
  printf("%s", txt);
  t_lincol(2, cursor + 1);
}

void exec_edita_etiqueta(estado_t *e)
{
  char txt[4] = "";
  strcpy(txt, e->etiqueta_edicao);
  
  if (e->nota_corrente != NULL) {
    strcpy(txt, e->nota_corrente->etiqueta);
  }

  int cursor;

  
  while (e->modo == edita_etiqueta) {
    cursor = strlen(txt);
    tela_edita_etiqueta(txt, cursor);
    tecla_t tec = t_tecla();

    if (tec == T_ESC) {
      muda_modo(e, principal);
    } else if (tec == T_BS) {
      remove_caractere(txt, cursor);
    } else if ((tec >= 'A' && tec <= 'Z') || (tec >= '0' && tec <= '9')) {
      if (strlen(txt) < 3) {
         insere_caractere(txt, tec, cursor);
      }
    } else if (tec == T_ENTER) {
      if (strlen(txt) == 3) {
        strcpy(e->etiqueta_edicao, txt);
        if (e->nota_corrente != NULL) {
          strcpy(e->nota_corrente->etiqueta, txt);
        }
        muda_modo(e, principal);
      }
    } else if (tec == T_CTRL_T) { // Usei shift+direita porque não tinha shift+enter
      strcpy(e->etiqueta_edicao, txt);
      altera_notas_visiveis(e, 2);
      muda_modo(e, principal);
    }
  }
}

// FUNÇÕES EDITA COR

void altera_valor_componente (estado_t *e, int c_atual, int dc) {
  unsigned char *comp = &(e->cor_edicao[c_atual]);
  if (*(comp) + dc >= 0 && *(comp) + dc <= 255) {
    *(comp) += dc;
  }
  else if (*(comp) + dc > 255) {
    *(comp) = 255;
  }
  else {
    *(comp) = 0;
  }
}

void altera_componente (int *c_atual, int dc) {
  if (*(c_atual) + dc >= 0 && *(c_atual) + dc <= 2) {
    *(c_atual) += dc;
  }
  else if (*(c_atual) + dc > 2) {
    *(c_atual) = 0;
  }
  else {
    *(c_atual) = 2;
  }
}

unsigned char soma_digito_cor (tecla_t tec, unsigned char c) {
  if (c * 10 + (tec - '0') <= 255) {
    return c * 10 + (tec - '0');
  }
  else {
    return c;
  }
}

void tela_edita_cor (estado_t *e, int c) {
  t_limpa();
  desenha_ret(1, 1, 5, 39, 0, 0, 0, "  r    g    b  ", "");
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

void exec_edita_cor (estado_t *e) {
  if (e->nota_corrente != NULL) {
    e->cor_edicao[0] = e->nota_corrente->cor.r;
    e->cor_edicao[1] = e->nota_corrente->cor.g;
    e->cor_edicao[2] = e->nota_corrente->cor.b;
  }
  int ultimo_foi_digito = 0;
  int cor_atual = 0;
  while (e->modo == edita_cor) {
    tela_edita_cor(e, cor_atual);
    tecla_t tec = t_tecla();

    if (tec == 'e' || tec == 'r') {
      cor_atual = 0;
      ultimo_foi_digito = 0;
    } else if (tec == 'v' || tec == 'g') {
      cor_atual = 1;
      ultimo_foi_digito = 0;
    } else if (tec == 'a' || tec == 'b') {
      cor_atual = 2;
      ultimo_foi_digito = 0;
    } else if (tec == T_CIMA) {
      altera_valor_componente(e, cor_atual, 1);
      ultimo_foi_digito = 0;
    } else if (tec == T_S_CIMA) {
      altera_valor_componente(e, cor_atual, 30);
      ultimo_foi_digito = 0;
    } else if (tec == T_BAIXO) {
      altera_valor_componente(e, cor_atual, -1);
      ultimo_foi_digito = 0;
    } else if (tec == T_S_BAIXO) {
      altera_valor_componente(e, cor_atual, -30);
      ultimo_foi_digito = 0;
    } else if (tec == T_ESQUERDA) {
      altera_componente(&cor_atual, -1);
      ultimo_foi_digito = 0;
    } else if (tec == T_DIREITA) {
      altera_componente(&cor_atual, 1);
      ultimo_foi_digito = 0;
    } else if (tec == T_ENTER) {
      if (e->nota_corrente != NULL) {
        e->nota_corrente->cor.r = e->cor_edicao[0];
        e->nota_corrente->cor.g = e->cor_edicao[1];
        e->nota_corrente->cor.b = e->cor_edicao[2];
        muda_modo(e, principal);
      }
    } else if (tec == T_ESC) {
      muda_modo(e, principal);
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
    else if (tec == T_CTRL_T) {
      altera_notas_visiveis(e, 1);
      muda_modo(e, principal);
    }
  }
}

int ler_notas (FILE *arq, FILE *erros, estado_t *e) {
  char etiqueta[10], texto[300], texto_cortado[150];
  int r, g, b, x, y, largura, altura; 
  int tem_erro, cont, c;
  while (fscanf(arq, "%s %d %d %d %d %d %d %d ", etiqueta, &r, &g, &b, &x, &y, &largura, &altura) == 8) {
    cont = 0;
    tem_erro = 0;

    // Serve pra ignorar a primeira " da palavra
    fgetc(arq);

    while ((c = fgetc(arq)) != '\"' && c != '\n' && c != EOF) {
      texto[cont] = c;
      cont++;
    }

    // Se tiver fechar aspas e tiver texto depois tem erro, se não tiver texto depois não tem
    if (c == '\"') {
      while ((c = fgetc(arq)) != '\n' && c != EOF) {
        texto[cont] = c;
        cont++;
        tem_erro = 1;
      }
    }
    // Se tiver \n ou EOF antes do fechamento das aspas tem erro tambem
    else {
      tem_erro = 1;
    }

    texto[cont] = '\0';

    // Etiqueta de tamanho fora do padrão
    if (strlen(etiqueta) != 3) {
      tem_erro = 1;
    }

    // Etiqueta inválida
    for (int i = 0; i < 3; i++) {
      if (!((etiqueta[i] >= 'A' && etiqueta[i] <= 'Z') ||
          (etiqueta[i] >= '0' && etiqueta[i] <= '9'))) {
            tem_erro = 1;
      }
    }

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255) ||
        x < 1 || y < 1 || largura < 1 || altura < 1) {
      tem_erro = 1;
    }

    // Texto maior que devia
    if (strlen(texto) > 149) {
      strcpy(texto_cortado, texto);
      texto_cortado[149] = '\0';

      /* Esse valor é um caso especifico em que o texto não é invalido porém ultrapassa
      o limite do vetor declarado nesse programa */
      tem_erro = 2;
    }

    // Texto inválido
    for (int i = 0; i < strlen(texto); i++) {
      if (!(texto[i] >= 32 && texto[i] <= 126)) {
            tem_erro = 1;
      }
    }

    if (tem_erro == 1) {
      fprintf(erros, "%s %d %d %d %d %d %d %d \"%s\"\n", etiqueta, r, g, b, x, y, largura, altura, texto);
    }
    else {
      if(!realoca_memoria(e, 1)) return -1;
      strcpy(e->notas[e->n_notas - 1].etiqueta, etiqueta);
      e->notas[e->n_notas - 1].cor.r = r;
      e->notas[e->n_notas - 1].cor.g = g;
      e->notas[e->n_notas - 1].cor.b = b;
      e->notas[e->n_notas - 1].retangulo.x = x;
      e->notas[e->n_notas - 1].retangulo.y = y;
      e->notas[e->n_notas - 1].retangulo.largura = largura;
      e->notas[e->n_notas - 1].retangulo.altura = altura;
      if (tem_erro == 0) {
        strcpy(e->notas[e->n_notas - 1].texto, texto);
      }
      else if (tem_erro == 2) {
        fprintf(erros, "%s %d %d %d %d %d %d %d \"%s\"\n", etiqueta, r, g, b, x, y, largura, altura, texto);
        strcpy(e->notas[e->n_notas - 1].texto, texto_cortado);
      }
    }
  }
  return 0;
}

int main (void) {
    // Arquivo de erros
    FILE *erros = fopen("erros.txt", "a");
    if (erros == NULL) {
      printf("Não consegui abrir o arquivo de erros!\n");
      return -1;
    }
    fprintf(erros, "Erros: \n");

    // Le o arquivo
    char file_name[30];
    printf("Digite o arquivo para ler as notas: \n");
    scanf("%s", file_name);
    FILE *arq = fopen(file_name, "r");
    if (arq == NULL) {
        printf("Não consegui abrir o arquivo!\n");
        return -1;
    }

    estado_t estado = { principal, NULL, 0, 1, 1, NULL, {0}, 0, "", "", 80, 24, {0}, ""};

    int conseguiu_ler = ler_notas(arq, erros, &estado);
    if (conseguiu_ler == -1) {
      free(estado.notas);
      return -1;
    }

    fclose(arq);


    t_inicia();
    while (estado.modo != fim) {
        switch (estado.modo) {
        case principal:
            exec_principal(&estado, file_name);
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
        case edita_ebusca:
            exec_edita_ebusca(&estado);
            break;
        case edita_etiqueta:
            exec_edita_etiqueta(&estado);
            break;
        default:
            break;
        }
        
    }
    t_fim();
    free(estado.notas);
    fclose(erros);
}