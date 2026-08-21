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
static void s_ok(Str_c s)
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

Str s_cria(char const *strC)
{
  Str str = malloc(sizeof(*str));
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

Str s_cria_substring(Str_c s, int pos, int tam)
{
   Str nova = s_cria("");
   s_substring(nova, s, pos, tam);
   return nova;
}

Str s_cria_cópia(Str_c s)
{
   return s_cria_substring(s, 0, -1);
}

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_cria_de_arquivo(char *nome)
{
  Str s = s_cria("");
  //...
  return s;
}

// operações de acesso {{{1

int s_tam(Str_c s)
{
  s_ok(s);
  return u8_conta_unichar_nos_bytes(s->b_uso, s->s);
}

char *s_strc(Str_c s)
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

unichar s_ch(Str_c s, int pos)
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


// operações de busca e comparação {{{1

bool s_igual(Str_c s, Str_c sb)
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

int s_busca_c(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);

  if (pos < 0) pos = s_tam(s) + pos + 1;

  for (int i = pos; i < s_tam(s); i++) {
    for (int j = 0; j < s_tam(sb); j++) {
      if (s_ch(s, i) == s_ch(sb, j)) {
        return i;
      }
    }
  }

  return -1;
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  bool tem;
  if (pos < 0) pos = s_tam(s) + pos + 1;

  for (int i = pos; i < s_tam(s); i++) {
    tem = false;
    for (int j = 0; j < s_tam(sb); j++) {
      if (s_ch(s, i) == s_ch(sb, j)) {
        tem = true;
        break;
      }
    }
    if (!tem) return i;
  }
  return -1;
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);

  if (pos < 0) pos = s_tam(s) + pos + 1;

  for (int i = pos - 1; i >= 0; i--) {
    for (int j = 0; j < s_tam(sb); j++) {
      if (s_ch(s, i) == s_ch(sb, j)) {
        return i;
      }
    }
  }
  return -1;
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  bool tem;

  if (pos < 0) pos = s_tam(s) + pos + 1;

  for (int i = pos - 1; i >= 0; i--) {
    tem = false;
    for (int j = 0; j < s_tam(sb); j++) {
      if (s_ch(s, i) == s_ch(sb, j)) {
        tem = true;
        break;
      }
    }
    if (!tem) return i;
  }

  return -1;
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
  s_ok(s);
  s_ok(buscada);

  if (pos < 0) pos = s_tam(s) + pos + 1;

  if (s_tam(buscada) == 0) return pos;

  for (int i = pos; i <= s_tam(s) - s_tam(buscada); i++) {
    Str sub_s = s_cria_substring(s, i, s_tam(buscada));
    if (s_igual(sub_s, buscada)) {
      s_destroi(sub_s);
      return i;
    }
    s_destroi(sub_s);
  }

  return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
  s_ok(s);
  s_ok(sb);
  if (pos < -s_tam(sb)) pos = 0;
  else if (pos < 0) pos = s_tam(sb) + pos + 1;
  if (tam < 0) tam = s_tam(sb);
  if (pos + tam - 1 > s_tam(sb) - 1) tam = s_tam(sb) - pos;

  byte *pos_ini = u8_avanca_unichar(sb->s, pos);
  byte *pos_fim = u8_avanca_unichar(sb->s, pos + tam);
  int bytes = pos_fim - pos_ini;

  if (bytes > s->b_aloc && bytes != 0) {
    s->b_aloc = MIN_ALLOC;
    while (s->b_aloc < bytes) s->b_aloc *= 2;

    s->s = realloc(s->s, sizeof(byte) * s->b_aloc);
    assert(s->s != NULL);
  }

  for (int i = 0; i < pos_fim - pos_ini; i++) {
    s->s[i] = pos_ini[i];
  }
  s->b_uso = bytes;
}

void s_copia(Str s, Str_c sb)
{
  s_substring(s, sb, 0, -1);
}

void s_insere(Str s, int pos, Str_c sb)
{
  s_substitui(s, pos, 0, sb);
}

void s_insere_c(Str s, int pos, unichar c)
{
  s_ok(s);
  //...
}

void s_anexa(Str s, Str_c sb)
{
  s_substitui(s, -1, 0, sb);
}

void s_anexa_c(Str s, unichar c)
{
  s_insere_c(s, -1, c);
}

void s_remove(Str s, int pos, int tam)
{
  s_substitui(s, pos, tam, NULL);
}

void s_apara(Str s, Str_c sobras)
{
  s_ok(s);
  s_ok(sobras);
  //...
}

// operações de E/S {{{1

void s_imprime(Str_c s)
{
  s_ok(s);
  for (int i = 0; i < s->b_uso; i++) {
    printf("%c", s->s[i]);
  }
}

void s_grava_arquivo(Str_c s, char *nome)
{
  s_ok(s);
  //...
}


// vim: foldmethod=marker shiftwidth=2