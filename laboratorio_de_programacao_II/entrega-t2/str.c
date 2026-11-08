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
  int nchars; 
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
    assert(s->nchars == 0);
  }
  else {
    assert(s->b_aloc >= MIN_ALLOC);
    assert(s->b_uso <= s->b_aloc);
    assert(s->nchars >= 1 && s->nchars <= s->b_uso);
  }
}

// operações de criação e destruição {{{1

Str s_cria(char const *strC)
{
  Str str = malloc(sizeof(*str));
  assert(str != NULL);

  str->s = NULL;
  str->b_uso = 0;
  str->b_aloc = 0;
  str->nchars = 0;

  if (strC == NULL) return str;
  int tam = strlen(strC);
  if (tam == 0) return str;

  int nchars = u8_conta_unichar_nos_bytes(tam, (byte *)strC);
  if (nchars == -1) return str;


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
  str->nchars = nchars;

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
  Str s_vazia = s_cria("");

  FILE *arq = fopen(nome, "r");
  if (arq == NULL) return s_vazia;
  fseek(arq, 0, SEEK_END);
  long tamanho = ftell(arq);
  rewind(arq);

  byte *v = malloc(sizeof(byte) * (tamanho + 1));
  if (v == NULL) {
    fclose(arq);
    return s_vazia;
  }
  int c = fgetc(arq);
  int i = 0;
  while (c != EOF) {
    v[i] = c;
    c = fgetc(arq);
    i++;
  }
  v[tamanho] = '\0';

  Str s = s_cria(v);
  s_destroi(s_vazia);
  free(v);
  fclose(arq);
  return s;
}

// operações de acesso {{{1

int s_tam(Str_c s)
{
  s_ok(s);
  return s->nchars;
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
  if (pos > tam - 1 || pos < -(tam + 1) || pos == -1) return UNI_INV;

  byte *pos_unichar;
  if (pos >= 0) {
    pos_unichar = u8_avanca_unichar(s->s, pos);
  }
  else {
    pos_unichar = u8_avanca_unichar(s->s, tam + pos + 1);
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
  if (pos > s_tam(s)) pos = s_tam(s);

  int pos_s = u8_avanca_unichar(s->s, pos) - s->s;

  for (int i = pos; i < s_tam(s); i++) {
    unichar c;
    int nb_s = u8_unichar_nos_bytes(s->b_uso - pos_s, s->s + pos_s, &c);
    int pos_sb = 0;
    while (pos_sb < sb->b_uso) {
      unichar cb;
      int nb_sb = u8_unichar_nos_bytes(sb->b_uso - pos_sb, sb->s + pos_sb, &cb);
      if (c == cb) return i;
      pos_sb += nb_sb;
    }
    pos_s += nb_s;
  }

  return -1;
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  bool tem;
  if (pos < 0) pos = s_tam(s) + pos + 1;
  if (pos > s_tam(s)) pos = s_tam(s);

  int pos_s = u8_avanca_unichar(s->s, pos) - s->s;

  for (int i = pos; i < s_tam(s); i++) {
    unichar c;
    tem = false;
    int nb_s = u8_unichar_nos_bytes(s->b_uso - pos_s, s->s + pos_s, &c);
    int pos_sb = 0;
    while (pos_sb < sb->b_uso) {
      unichar cb;
      int nb_sb = u8_unichar_nos_bytes(sb->b_uso - pos_sb, sb->s + pos_sb, &cb);
      if (c == cb) {
        tem = true;
        break;
      }
      pos_sb += nb_sb;
    }
    if (!tem) return i;
    pos_s += nb_s;
  }

  return -1;
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);

  if (pos < 0) pos = s_tam(s) + pos + 1;
  if (pos > s_tam(s)) pos = s_tam(s);

  /* percorre s do início até pos, guardando a última posição 
  em que o caractere pertence a sb, se não tiver nenhum retorna -1 (o esperado) */
  int ultima = -1;
  int pos_s = 0;

  for (int i = 0; i < pos; i++) {
    unichar c;
    int nb_s = u8_unichar_nos_bytes(s->b_uso - pos_s, s->s + pos_s, &c);
    int pos_sb = 0;
    while (pos_sb < sb->b_uso) {
      unichar cb;
      int nb_sb = u8_unichar_nos_bytes(sb->b_uso - pos_sb, sb->s + pos_sb, &cb);
      if (c == cb) {
        ultima = i;
        break;
      }
      pos_sb += nb_sb;
    }
    pos_s += nb_s;
  }

  return ultima;
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  bool tem;

  if (pos < 0) pos = s_tam(s) + pos + 1;
  if (pos > s_tam(s)) pos = s_tam(s);

  int ultima = -1;
  int pos_s = 0;

  for (int i = 0; i < pos; i++) {
    unichar c;
    tem = false;
    int nb_s = u8_unichar_nos_bytes(s->b_uso - pos_s, s->s + pos_s, &c);
    int pos_sb = 0;
    while (pos_sb < sb->b_uso) {
      unichar cb;
      int nb_sb = u8_unichar_nos_bytes(sb->b_uso - pos_sb, sb->s + pos_sb, &cb);
      if (c == cb) {
        tem = true;
        break;
      }
      pos_sb += nb_sb;
    }
    if (!tem) ultima = i;
    pos_s += nb_s;
  }

  return ultima;
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
  s_ok(s);
  s_ok(buscada);

  if (pos < 0) pos = s_tam(s) + pos + 1;
  if (pos > s_tam(s)) pos = s_tam(s);

  if (s_tam(buscada) == 0) return pos;

  int dist = u8_avanca_unichar(s->s, pos) - s->s;

  for (int i = pos; i <= s->nchars - buscada->nchars; i++) {
    if (dist + buscada->b_uso <= s->b_uso &&
      memcmp(s->s + dist, buscada->s, buscada->b_uso) == 0) return i;
    
    dist += u8_nbytes_no_unichar_que_comeca_com(s->s[dist]);
  }

  return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
  s_ok(s);
  if (sb != NULL) s_ok(sb);

  if (pos < -s_tam(s)) pos = 0;
  else if (pos < 0) pos = s_tam(s) + pos + 1;
  if (pos > s_tam(s)) pos = s_tam(s);
  if (tam < 0) tam = s_tam(s);
  if (pos + tam > s_tam(s)) tam = s_tam(s) - pos;

  int ini = u8_avanca_unichar(s->s, pos) - s->s;
  int fim = u8_avanca_unichar(s->s + ini, tam) - s->s;

  // STR_RES = INICIO(S) / SUBSTITUIDA(SB) / FIM(S)

  int b_sb = (sb != NULL) ? sb->b_uso : 0;
  int n_sb = (sb != NULL) ? sb->nchars : 0;
  int b_final_str = s->b_uso - fim; // bytes fim
  int bytes_total = ini + b_sb + b_final_str;
  int nchars_total = s->nchars - tam + n_sb;

  if (bytes_total > s->b_aloc) {
      if (s->b_aloc == 0) s->b_aloc = MIN_ALLOC;
      while (bytes_total > s->b_aloc) s->b_aloc *= 2;
      byte *nova_s = realloc(s->s, sizeof(byte) * s->b_aloc);
      assert(nova_s != NULL);
      s->s = nova_s;
  }

  if (b_final_str > 0 && ini + b_sb != fim) {
    memmove(s->s + ini + b_sb, s->s + fim, b_final_str);
  }
  if (b_sb > 0) memmove(s->s + ini, sb->s, b_sb);

  s->b_uso = bytes_total;
  s->nchars = nchars_total;
  if (s->b_uso == 0) {
      free(s->s);
      s->s = NULL;
      s->b_aloc = 0;
  }
  else {
      while (s->b_uso < s->b_aloc / 2 && s->b_aloc / 2 >= MIN_ALLOC) s->b_aloc = s->b_aloc / 2;
      byte *nova_s = realloc(s->s, sizeof(byte) * s->b_aloc);
      assert(nova_s != NULL);
      s->s = nova_s;
  }
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
  s_ok(s);
  s_ok(sb);
  if (pos < -s_tam(sb)) pos = 0;
  else if (pos < 0) pos = s_tam(sb) + pos + 1;
  if (pos > s_tam(sb)) pos = s_tam(sb);
  if (tam < 0) tam = s_tam(sb);
  if (pos + tam > s_tam(sb)) tam = s_tam(sb) - pos;

  byte *pos_ini = u8_avanca_unichar(sb->s, pos);
  byte *pos_fim = u8_avanca_unichar(sb->s, pos + tam);
  int bytes = pos_fim - pos_ini;

  if (bytes > s->b_aloc) {
    if (s->b_aloc == 0) s->b_aloc = MIN_ALLOC;
    while (bytes > s->b_aloc) s->b_aloc *= 2;
    byte *nova_s = realloc(s->s, sizeof(byte) * s->b_aloc);
    assert(nova_s != NULL);
    s->s = nova_s;
  }

  for (int i = 0; i < pos_fim - pos_ini; i++) {
    s->s[i] = pos_ini[i];
  }
  s->b_uso = bytes;
  s->nchars = tam;
  if (s->b_uso == 0) {
    free(s->s);
    s->s = NULL;
    s->b_aloc = 0;
  } 
  else {
    while (s->b_uso < s->b_aloc / 2 && s->b_aloc / 2 >= MIN_ALLOC) s->b_aloc /= 2;
    byte *nova_s = realloc(s->s, sizeof(byte) * s->b_aloc);
    assert(nova_s != NULL);
    s->s = nova_s;
  }
}

void s_copia(Str s, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  s_substring(s, sb, 0, -1);
}

void s_insere(Str s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  s_substitui(s, pos, 0, sb);
}

void s_insere_c(Str s, int pos, unichar c)
{
  s_ok(s);
  byte *v = malloc(sizeof(byte) * 5);
  assert(v != NULL);
  int nbytes = u8_converte_pra_utf8(c, v);
  assert(nbytes != -1);
  v[nbytes] = '\0';
  Str sb = s_cria(v);
  free(v);
  s_substitui(s, pos, 0, sb);
  s_destroi(sb);
}

void s_anexa(Str s, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  s_substitui(s, -1, 0, sb);
}

void s_anexa_c(Str s, unichar c)
{
  s_ok(s);
  s_insere_c(s, -1, c);
}

void s_remove(Str s, int pos, int tam)
{
  s_ok(s);
  s_substitui(s, pos, tam, NULL);
}

void s_apara(Str s, Str_c sobras)
{
  s_ok(s);
  s_ok(sobras);
  int pos1 = s_busca_nc(s, 0, sobras);
  if (pos1 == -1) {
    s_remove(s, 0, -1);
    return;
  }

  s_remove(s, 0, pos1);
  int pos2 = s_busca_rnc(s, -1, sobras);
  s_remove(s, pos2 + 1, -1);
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
  FILE *arq = fopen(nome, "w");
  assert(arq != NULL);
  fwrite(s->s, sizeof(byte), s->b_uso, arq);
  fclose(arq);
}


// vim: foldmethod=marker shiftwidth=2