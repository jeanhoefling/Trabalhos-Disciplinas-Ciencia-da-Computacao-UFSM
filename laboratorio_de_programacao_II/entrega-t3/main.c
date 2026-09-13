#include "calc.h"
#include <stdio.h>
#include <assert.h>

int main() {
  char nome[50];
  printf("Escreva o nome do arquivo para calcular: ");
  scanf("%s", nome);
  FILE *arq_in = fopen(nome, "r");
  assert(arq_in != NULL);

  Lista l_in = l_cria();
  Lista l_out = l_cria();
  char lin[2000];
  while (fgets(lin, 2000, arq_in) != NULL) {
    Str s = s_cria(lin);
    l_insere_inicio(l_in, s);
    Str res = calculadora(s);
    l_insere_inicio(l_out, res);
    s_grava_arquivo(res, "resultados.txt");
  }
  l_destroi(l_in);
  l_destroi(l_out);
  fclose(arq_in);
}