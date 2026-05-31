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

  typedef enum { move, edita, fim } modo_t;

  typedef struct {
    modo_t modo;

    nota *notas;
    int n_notas;

    int cursor_x;
    int cursor_y;

    nota *nota_corrente;
  } estado_t;


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
    t_cortexto(0, 0, 0);
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
    desenha_ret(1, 1, 30, 70, 10, 20, 30, "fundo");
    for (int i = 0; i < e->n_notas; i++) {
      desenha_ret(e->notas[i].retangulo.y, e->notas[i].retangulo.x, e->notas[i].retangulo.altura, e->notas[i].retangulo.largura,
                  e->notas[i].cor.r, e->notas[i].cor.g, e->notas[i].cor.b, e->notas[i].texto);
    }
    // põe o cursor no centro do retângulo
    t_lincol(e->cursor_y, e->cursor_x);
  }

  void atualiza_nota_corrente (estado_t *e) {
    retangulo atual;
    for (int i = 0; i < e->n_notas; i++) {
      atual = e->notas[i].retangulo;
      if (e->cursor_x >= atual.x && e->cursor_x <= (atual.x + atual.largura) && e->cursor_y >= atual.y && e->cursor_y <= atual.y + atual.altura) {
        e->nota_corrente = &(e->notas[i]);
      }
    }
  }

  void move_direita(estado_t *e)
  {
    e->cursor_x++;
    atualiza_nota_corrente(e);
  }

  void move_esquerda(estado_t *e)
  {
    e->cursor_x--;
    atualiza_nota_corrente(e);
  }

  void move_cima(estado_t *e)
  {
    e->cursor_y--;
    atualiza_nota_corrente(e);
  }

  void move_baixo(estado_t *e)
  {
    e->cursor_y++;
    atualiza_nota_corrente(e);
  }

void move_nota_direita(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.x++;
    }
}

void move_nota_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.x--;
    }
}

void move_nota_cima(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.y--;
    }
}

void move_nota_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.y++;
    }
}

void diminui_direita(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.largura--;
    }
}

void diminui_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.x++;
        e->nota_corrente->retangulo.largura--;
    }
}

void diminui_cima(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.y++;
        e->nota_corrente->retangulo.altura--;
    }
}

void diminui_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.altura--;
    }
}

void aumenta_direita(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.largura++;
    }
}

void aumenta_esquerda(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.x--;
        e->nota_corrente->retangulo.largura++;
    }
}

void aumenta_cima(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.altura++;
    }
}

void aumenta_baixo(estado_t *e)
{
    if (e->nota_corrente != NULL) {
        e->nota_corrente->retangulo.y--;
        e->nota_corrente->retangulo.altura++;
    }
}

  void exec_move(estado_t *e)
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
      case 'e':
        muda_modo(e, edita);
        break;
      case T_ESC:
        muda_modo(e, fim);
        break;
      default: ;
    }
  }

  // modo edita

  void tela_edita(estado_t *e, char texto[])
  {
    t_limpa();
    desenha_ret(2, 3, 20, 50, 10, 20, 30, "fundo");
    strcat(e->notas[0].texto, texto);
    desenha_ret(e->notas[0].retangulo.y, e->notas[0].retangulo.x, e->notas[0].retangulo.altura, e->notas[0].retangulo.largura, 150, 49, 97, e->notas[0].texto);
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
    s[l] = c;
    s[l + 1] = '\0';
  }

  void exec_edita(estado_t *e)
  {
    // modo com necessidade de manter valores locais, implementado com
    //   laço próprio
    char txt[20] = "";

    while (e->modo == edita) {
      // desenha a tela
      tela_edita(e, txt);

      // lê um comando
      tecla_t tec = t_tecla();

      // realiza uma ação conforme o comando lido
      if (tec == T_ESC) {
        muda_modo(e, move);
      } else if (tec == T_BS) {
        remove_ultimo_caractere(txt);
      } else if (tec >= 'a' && tec <= 'z') {
        insere_ultimo_caractere(txt, tec);
      }
    }
  }

  //JEAN
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

      // Le o arquivo
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


      t_inicia();
      estado_t estado = { move, notas, n_notas, 5, 5, NULL };
      while (estado.modo != fim) {
          switch (estado.modo) {
          case move:
              exec_move(&estado);
              break;
          case edita:
              exec_edita(&estado);
              break;
          default:
              break;
          }
          
      }
      // retorna a tela ao modo normal
      t_fim();
      fclose(arq);
  }