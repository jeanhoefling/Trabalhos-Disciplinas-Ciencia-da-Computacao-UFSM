// includes, constantes e declarações {{{1
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8    // alocação mínima

struct str {
  byte *s;
  int b_uso;
  int b_aloc;
};

// A memória para conter os bytes de uma string deve ser alocada e/ou
//   realocada conforme a necessidade, cuidando para que a quantidade
//   de memória alocada seja sempre:
//   - nula (não alocada) se a string for vazia, ou
//   - não inferior ao necessário para armazenar os bytes da codificação utf8;
//   - não inferior à alocação mínima;
//   - não superior ao triplo do número de bytes necessários
//     (exceto quando for o mínimo);
//   - uma potência de 2.

// funções auxiliares {{{1

// verifica se a string cad está de acordo com a especificação
// aborta o programa se não tiver
static void s_ok(Str s)
{
  if (s->s == NULL) {
    assert(s->b_uso == 0);
    assert(s->b_aloc == 0);
  }
  else {
    assert(s->b_aloc >= MIN_ALLOC);
    assert(s->b_uso <= s->b_aloc);
    assert(u8_conta_unichar_nos_bytes(s->b_uso, s->s) != -1);
  }
}

//...

// operações de criação e destruição {{{1

Str s_cria(char *strC)
{
  Str str = malloc(sizeof(struct str));
  assert(str != NULL);

  str->s = NULL;
  str->b_uso = 0;
  str->b_aloc = 0;

  if (strC == NULL) return str;
  int tam = strlen(strC);
  if (tam == 0) return str;

  if (u8_conta_unichar_nos_bytes(tam, (byte *)strC) == -1) return str;


  int nb_aloc = MIN_ALLOC;
  while (nb_aloc < tam) {
    nb_aloc *= 2;
  }
  str->s = malloc(sizeof(byte) * nb_aloc);
  assert(str->s != NULL);
  for (int i = 0; i < tam; i++) {
    str->s[i] = strC[i];
  }


  str->b_uso = tam;
  str->b_aloc = nb_aloc;

  return str;
}

void s_destroi(Str s)
{
  s_ok(s);
  free(s->s);
  free(s);
}

// operações de acesso {{{1

int s_tam(Str s)
{
  s_ok(s);
  return u8_conta_unichar_nos_bytes(s->b_uso, s->s);
}

char *s_strc(Str s)
{
  s_ok(s);
  char *strc = (char *)malloc(sizeof(char) * (s->b_uso + 1));
  assert(strc != NULL);

  for (int i = 0; i < s->b_uso; i++) {
    strc[i] = s->s[i]; 
  }
  strc[s->b_uso] = '\0';

  return strc;
}

unichar s_ch(Str s, int pos)
{
  s_ok(s);
  int tam = s_tam(s);
  if (pos > tam - 1 || pos < -tam) return UNI_INV;

  byte *pos_unichar;
  if (pos >= 0) {
    pos_unichar = u8_avanca_unichar(s->s, pos);
  }
  else {
    pos_unichar = u8_avanca_unichar(s->s, tam + pos);
  }
  unichar puni;
  int tam_unichar = u8_nbytes_no_unichar_que_comeca_com(*pos_unichar);
  u8_unichar_nos_bytes(tam_unichar, pos_unichar, &puni);
  return puni;
}

Str s_substring(Str s, int pos, int tam)
{
  s_ok(s);
  if (pos < -s_tam(s)) pos = 0;
  else if (pos < 0) pos = s_tam(s) + pos + 1;
  if (tam < 0) tam = s_tam(s);
  if (pos + tam - 1 > s_tam(s) - 1) tam = s_tam(s) - pos;

  byte *pos_ini = u8_avanca_unichar(s->s, pos);
  byte *pos_fim = u8_avanca_unichar(s->s, pos + tam);
  char strC[pos_fim - pos_ini + 1];
  for (int i = 0; i < pos_fim - pos_ini; i++) {
    strC[i] = pos_ini[i];
  }
  strC[pos_fim - pos_ini] = '\0';
  return s_cria(strC);
}

Str s_copia(Str s)
{
  s_ok(s);
  Str copia = s_substring(s, 0, -1);
  return copia;
}


// operações de busca e comparação {{{1

bool s_igual(Str s, Str sb)
{
  s_ok(s);
  s_ok(sb);
  if (s->b_uso != sb->b_uso) {
    return false;
  }
  for (int i = 0; i < s->b_uso; i++) {
    if (s->s[i] != sb->s[i]) {
      return false;
    }
  }
  return true;
}

int s_busca_c(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);

  if (pos < 0) pos = s_tam(s) + pos + 1;

  Str unichar_s, unichar_sb;
  for (int i = pos; i < s_tam(s); i++) {
    unichar_s = s_substring(s, i, 1);

    for (int j = 0; j < s_tam(s); j++) {
      unichar_sb = s_substring(sb, j, 1);
      if (s_igual(unichar_s, unichar_sb)) {
        s_destroi(unichar_s);
        s_destroi(unichar_sb);
        return i;
      }
      s_destroi(unichar_sb);
    }
    s_destroi(unichar_s);
  }

  return -1;
}

int s_busca_nc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rnc(Str s, int pos, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_s(Str s, int pos, Str buscada)
{
  s_ok(s);
  s_ok(buscada);
  //...
  return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str sb)
{
  s_ok(s);
  s_ok(sb);
  //...
}

void s_anexa(Str s, Str sb)
{
  s_substitui(s, -1, 0, sb);
}

void s_insere(Str s, int pos, Str sb)
{
  //...
}

void s_remove(Str s, int pos, int tam)
{
  //...
}

Str s_apara(Str s, Str sobras)
{
  s_ok(s);
  s_ok(sobras);
  //...
  return NULL;
}

// operações de E/S {{{1

// imprime a string em s na saída padrão
void s_imprime(Str s)
{
  s_ok(s);
  for (int i = 0; i < s->b_uso; i++) {
    printf("%c", s->s[i]);
  }
}

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_le_arquivo(Str nome)
{
  s_ok(nome);
  //...
  return NULL;
}

// grava o conteúdo de s em um arquivo chamado nome
void s_grava_arquivo(Str s, Str nome)
{
  s_ok(s);
  s_ok(nome);
  //...
}


// vim: foldmethod=marker shiftwidth=2

