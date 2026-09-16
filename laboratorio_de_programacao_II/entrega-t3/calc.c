#include "calc.h"

bool igual_str(chave_t a, chave_t b) {
  return s_igual((Str)a, (Str)b);
}

bool menor_str(chave_t a, chave_t b) {
  return true;
}

bool é_operador (unichar c) {
  if (c == '+' || c == '-' || c == '*' || c == '/'  || c == '^' || c == '(' || c == ')' || c == '=') return 1;
  return 0;
}

bool é_numero (dado_t d) {
  int cont_pontos = 0;
  for (int i = 0; i < s_tam(d); i++) {
    unichar c = s_ch(d, i);
    if (!((c >= '0' && c <= '9') || c == '.')) {
      return false;
    } else if (c == '.') {
      cont_pontos++;
      if (i == 0) {
        return false;
      }
    }
  }
  if (cont_pontos > 1) return false;
  return true;
}

bool é_variavel(dado_t d) {
  unichar c = s_ch(d, 0);
  if (!(c == '$' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) return false;
  for (int i = 1; i < s_tam(d); i++) {
    c = s_ch(d, i);
    if (!(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) return false;
  }
  return true;
}

bool teve_erro (dado_t s1) { //essa serve para verificar se executa_op retornou "#ERRO", não = 0; sim = 1
  dado_t s2 = s_cria("#ERRO");
  if (s_igual(s1, s2)) {
    s_destroi(s2);
    return 1;
  }
  s_destroi(s2);
  return 0;
}

double valor_operando(dado_t operando, Dicionário dic, bool *erro)
{
  if (é_numero(operando)) {
    return s_número(operando);
  }
  dado_t valor = dic_busca(dic, operando);
  if (valor == VALOR_NÃO_EXISTE) {
    *erro = 1;
    return 0;
  }

  return s_número(valor);
}

dado_t executa_op (unichar op, Lista operandos, Dicionário d) {
  if (l_tam(operandos) < 2) {
    return s_cria("#ERRO");
  }
  bool erro = 0;
  dado_t n1 = l_desempilha(operandos);
  dado_t n2 = l_desempilha(operandos);


  if (op != '=') {
    valor_operando(n2, d, &erro);
    valor_operando(n1, d, &erro);
    if (erro) {
      s_destroi(n1); 
      s_destroi(n2);
      return s_cria("#ERRO");
    }
  }

  double res;
  switch (op) {
    case '+':
      res = valor_operando(n2, d, &erro) + valor_operando(n1, d, &erro);
      break;
    case '-':
      res = valor_operando(n2, d, &erro) - valor_operando(n1, d, &erro);
      break;
    case '*':
      res = valor_operando(n2, d, &erro) * valor_operando(n1, d, &erro);
      break;
    case '/':
      if (valor_operando(n1, d, &erro) == 0) {
        s_destroi(n1);
        s_destroi(n2);
        return s_cria("#ERRO");
      }
      res = valor_operando(n2, d, &erro) / valor_operando(n1, d, &erro);
      break;
    case '^':
      res = 1;
      if (valor_operando(n1, d, &erro) > 0) {
        for (int i = 0; i < valor_operando(n1, d, &erro); i++) {
          res *= valor_operando(n2, d, &erro);
        }
      } else {
        for (int i = 0; i < -valor_operando(n1, d, &erro); i++) {
          res /= valor_operando(n2, d, &erro);
        }
      }
      break;
    case '=':
      double valor = valor_operando(n1, d, &erro);
      if (erro || !é_variavel(n2)) {
        s_destroi(n1); 
        s_destroi(n2);
        return s_cria("#ERRO");
      }
      dado_t valor_str = s_cria_número(valor);
      dado_t antigo = dic_insere(d, n2, valor_str);
      if (antigo != VALOR_NÃO_EXISTE) s_destroi(antigo);
      s_destroi(n1);
      return s_cria_cópia(valor_str);
    default:
      s_destroi(n1);
      s_destroi(n2);
      return s_cria("#ERRO esse operador não devia ta aqui");
      break;
  }
  dado_t strres = s_cria_número(res);
  s_destroi(n1);
  s_destroi(n2);
  return strres;
}

int opera_ou_empilha(Lista operadores, Lista operandos, Dicionário variaveis, dado_t d) { // retorna 0 se ok, -1 se err
  int ret;
  unichar op_atual = s_ch(d, 0);
  if (l_vazia(operadores)) {
    if (op_atual != ')') {
        l_empilha(operadores, d);
        return 0;
    }
    else {
      s_destroi(d);
      return -1;
    }
  }
  
  dado_t dado_antes = l_topo(operadores);
  unichar op_antes = s_ch(dado_antes, 0);
  if (op_antes == '-' || op_antes == '+') {
    if (op_atual == '+' || op_atual == '-' || op_atual == ')') {
      s_destroi(l_desempilha(operadores));
      dado_t s = executa_op(op_antes, operandos, variaveis);
      if (!teve_erro(s)) {
        l_empilha(operandos, s);
      } else {
        s_destroi(s);
        s_destroi(d);
        return -1;
      }
      ret = opera_ou_empilha(operadores, operandos, variaveis, d);
      return ret;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else if (op_antes == '*' || op_antes == '/' || op_antes == '^') {
    if (op_atual == '+' || op_atual == '-' || op_atual == ')' || op_atual == '*' || op_atual == '/') {
      s_destroi(l_desempilha(operadores));
      dado_t s = executa_op(op_antes, operandos, variaveis);
      if (!teve_erro(s)) {
        l_empilha(operandos, s);
      } else {
        s_destroi(s);
        s_destroi(d);
        return -1;
      }
      ret = opera_ou_empilha(operadores, operandos, variaveis, d);
      return ret;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else if (op_antes == '(') {
    if (op_atual == ')') {
      s_destroi(l_desempilha(operadores));
      s_destroi(d);
      return 0;
    }
    else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else if (op_antes == '=') {
    if (op_atual == ')') {
      s_destroi(l_desempilha(operadores));
      dado_t s = executa_op(op_antes, operandos, variaveis);
      if (!teve_erro(s)) l_empilha(operandos, s);
      else { s_destroi(s); s_destroi(d); return -1; }
      return opera_ou_empilha(operadores, operandos, variaveis, d);
    } else {
      l_empilha(operadores, d);
      return 0;
    }
  }
  else {
    s_destroi(d);
    return -1;
  }
}

Str finaliza_calc (Lista operadores, Lista operandos, Dicionário variaveis) {
  while (!(l_vazia(operadores))) {
    dado_t d = l_desempilha(operadores);
    unichar op = s_ch(d, 0);
    s_destroi(d);
    // se algum parenteses nao foi fechado
    if (op == '(') {
      return s_cria("#ERRO");
    }

    // executa op
    dado_t s = executa_op(op, operandos, variaveis);
    if (!teve_erro(s)) {
      l_empilha(operandos, s);
    } else {
      s_destroi(s);
      return s_cria("#ERRO");
    }
  }
  if (l_tam(operandos) != 1) {
    return s_cria("#ERRO");
  }
  dado_t final = l_desempilha(operandos);
  if (é_variavel(final)) {
    bool erro = false;
    double valor = valor_operando(final, variaveis, &erro);
    s_destroi(final);
    if (erro) return s_cria("#ERRO");
    return s_cria_número(valor);
  }
  return final;
}

// Calcula o valor de expressão e retorna uma nova Str contendo o resultado.
// Em cado de erro, os primeiros caracteres da Str de retorno são "#ERRO ".
Str calculadora(Str expressão) {
  static Dicionário variaveis = NULL;
  if (variaveis == NULL) variaveis = dic_cria(menor_str, igual_str);
  Lista tokens = tokeniza(expressão);
  Lista operando_num = l_cria();
  Lista operador = l_cria();
  bool num_var = true; // true se for num, false se for variavel 
  int err = 0;
  
  // essa parte poe os tokens nas pilhas certas, é interessante criar uma função separada pra isso
  // ver com professor
  while (!l_vazia(tokens)) {
    num_var = true;
    dado_t d = l_remove_inicio(tokens);
    unichar c = s_ch(d, 0);
    if (é_operador(c)) {
      err = opera_ou_empilha(operador, operando_num, variaveis, d);
      if (err == -1) break;
    }
    else if (é_numero(d) || é_variavel(d)) {
      l_empilha(operando_num, d);
    }
    else {
      err = -1;
    }
  }
  l_destroi(tokens);
  if (err == -1) {
    l_destroi(operador);
    l_destroi(operando_num);
    return s_cria("#ERRO");
  }

  Str sres = finaliza_calc(operador, operando_num, variaveis);
  l_destroi(operador);
  l_destroi(operando_num);
  return sres;
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