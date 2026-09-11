#include "calc.h"

// Calcula o valor de expressão e retorna uma nova Str contendo o resultado.
// Em cado de erro, os primeiros caracteres da Str de retorno são "#ERRO ".
Str calculadora(Str expressão) {
  Lista tokens = tokeniza(expressão);
  Lista operandos = l_cria();
  Lista operadores = l_cria();
  for (int i = 0; i < l_tam(tokens); i++) {
    dado_t d = l_dado_pos(tokens, i);
    unichar prim = s_ch(d, 0);
    if (prim == '+' || prim == '-' || prim == '*' || prim == '/' 
      || prim == '^' || prim == '(' || prim == ')' || prim == '=') {
        l_empilha(operadores, d);
      }
  }
}

// Retorna uma nova Lista contendo substrings de txt.
// Uma substring inicia em um caractere diferente de espaço, tabulação,
//   fim de linha.
// Se a substring inicia por um dígito ou um ponto, contém os demais dígitos
//   ou pontos que seguem.
// Se a substring inicia por uma letra ou sublinhado ou `$`, contém os
//   demais letras, sublinhados, `$` ou dígitos que seguem.
// Se a substring inicia por outro caractere, contém somente esse caractere.
// Exemplos:
// " 9. 5" -> ["9." "5"]
// "92+a ba 3b3 ** *  " -> ["92" "+" "a" "ba" "3" "b3" "*" "*" "*"] 
Lista tokeniza(Str txt) {
  Lista l = l_cria();
  Str sep = s_cria("\t \n");
  Str sep2 = s_cria("0123456789.");
  Str sep3 = s_cria("0123456789$_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  int pos = s_busca_nc(txt, 0, sep);
  dado_t subs;
  while (pos != -1) {
    unichar prim = s_ch(txt, pos);
    if ((prim >= '0' && prim <= '9') || prim == '.') {
      int possep = s_busca_nc(txt, pos, sep2);
      // Caso em que não será necessario efetuar mais nenhuma divisão na string
      if (possep == -1) {
          subs = s_cria_substring(txt, pos, s_tam(txt) - pos);
          l_insere_fim(l, subs);
          break;
      }
      subs = s_cria_substring(txt, pos, possep - pos);
      l_insere_fim(l, subs);
      pos = s_busca_nc(txt, possep, sep);   
    }
    else if ((prim >= 'A' && prim <= 'Z') || (prim >= 'a' && prim <= 'z') || prim == '$' || prim == '_') {
      int possep = s_busca_nc(txt, pos, sep3);
      // Caso em que não será necessario efetuar mais nenhuma divisão na string
      if (possep == -1) {
          subs = s_cria_substring(txt, pos, s_tam(txt) - pos);
          l_insere_fim(l, subs);
          break;
      }
      subs = s_cria_substring(txt, pos, possep - pos);
      l_insere_fim(l, subs);
      pos = s_busca_nc(txt, possep, sep);   
    }
    else {
      subs = s_cria_substring(txt, pos, 1);
      l_insere_fim(l, subs);
      pos = s_busca_nc(txt, pos + 1, sep);  
    }
  }
  s_destroi(sep);
  s_destroi(sep2);
  s_destroi(sep3);
  return l;
}