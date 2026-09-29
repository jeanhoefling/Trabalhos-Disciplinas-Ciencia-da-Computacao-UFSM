#include "calc.h"
#include <stdio.h>
#include <assert.h>

int main() {
  char nome[50];
  printf("Escreva o nome do arquivo para calcular: ");
  scanf("%49s", nome);
  Str s_arq = s_cria_de_arquivo(nome);
  Str sep = s_cria("\n");
  Lista l_in = l_cria_separando(s_arq, sep);
  Lista l_out = l_cria();
  while (!l_vazia(l_in)) {
    Str s_lin = l_remove_inicio(l_in);
    Str res_calc = calculadora(s_lin);
    l_insere_fim(l_out, res_calc);
    s_destroi(s_lin);
  }
  Str res = s_cria_unindo(l_out, sep);
  s_grava_arquivo(res, "resultados.txt");
  s_destroi(sep);
  s_destroi(res);
  l_destroi(l_out);
  l_destroi(l_in);
  s_destroi(s_arq);
}