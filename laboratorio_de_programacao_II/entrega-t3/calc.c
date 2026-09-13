#include "calc.h"

dado_t executa_op (unichar op, Lista operandos) {
  dado_t n1 = l_desempilha(operandos);
  dado_t n2 = l_desempilha(operandos);
  double num1 = s_número(n1);
  double num2 = s_número(n2);
  double res;
  switch (op) {
    case '+':
      res = num2 + num1;
      break;
    case '-':
      res = num2 - num1;
      break;
    case '*':
      res = num2 * num1;
      break;
    case '/':
      res = num2 / num1;
      break;
    case '^':
      res = 1;
      for (int i = 0; i < num1; i++) {
        res *= num2;
      }
      break;
  }
  dado_t strres = s_cria_número(res);
  return strres;
}

int opera_ou_empilha(Lista operadores, Lista operandos, dado_t d) { // retorna 0 se ok, -1 se err
  unichar op_atual = s_ch(d, 0);
  if (l_vazia(operadores)) {
    if (op_atual != ')') {
        l_empilha(operadores, d);
        return 0;
    }
    else {
      return -1;
    }
  }
  
  dado_t dado_antes = l_topo(operadores);
  unichar op_antes = s_ch(dado_antes, 0);
  if (op_antes == '-' || op_antes == '+') {
    if (op_atual == '+' || op_atual == '-' || op_atual == ')') {
      l_desempilha(operadores);
      l_empilha(operandos, executa_op(op_antes, operandos));
      opera_ou_empilha(operadores, operandos, d);
      return 0;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else if (op_antes == '*' || op_antes == '/' || op_antes == '^') {
    if (op_atual == '+' || op_atual == '-' || op_atual == ')' || op_atual == '*' || op_atual == '/') {
      l_desempilha(operadores);
      l_empilha(operandos, executa_op(op_antes, operandos));
      opera_ou_empilha(operadores, operandos, d);
      return 0;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else if (op_antes == '(') {
    if (op_atual == ')') {
      l_desempilha(operadores);
      return 0;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
}

// Calcula o valor de expressão e retorna uma nova Str contendo o resultado.
// Em cado de erro, os primeiros caracteres da Str de retorno são "#ERRO ".
Str calculadora(Str expressão) {
  Lista tokens = tokeniza(expressão);
  Lista operando_num = l_cria();
  Lista operador = l_cria();
  bool num_var = true; // true se for num, false se for variavel 
  
  // essa parte poe os tokens nas pilhas certas, é interessante criar uma função separada pra isso
  // ver com professor
  for (int i = 0; i < l_tam(tokens); i++) {
    num_var = true;
    dado_t d = l_dado_pos(tokens, i);
    unichar c = s_ch(d, 0);
    if (c == '+' || c == '-' || c == '*' || c == '/' 
      || c == '^' || c == '(' || c == ')' || c == '=') {
        opera_ou_empilha(operador, operando_num, d);
      }
    // o  else verifica se é num ou variavel
    else {
      for (int i = 0; i < s_tam(d); i++) {
        c = s_ch(d, i);
        if (!((c >= '0' && c <= '9') || c == '.')) {
          num_var = false;
        }
      }
    }
    if (num_var) {
      l_empilha(operando_num, d);
    } else {
      // deverá por no lugar das variaveis
    }
  }
  // a partir daqui tudo deve estar na pilha correta



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