# Tabela de Senos e Cossenos em C

## Descrição

Este projeto foi desenvolvido em linguagem C como parte de uma atividade acadêmica de programação. O objetivo é implementar funções matemáticas fundamentais sem utilizar bibliotecas prontas, aplicando conceitos de recursão, iteração e métodos numéricos.

O programa calcula e exibe uma tabela de valores de seno e cosseno para ângulos de 0° a 89°, utilizando aproximações matemáticas implementadas manualmente.

## Funcionalidades

* Cálculo de potência com expoentes positivos e negativos.

* Cálculo de fatorial por recursão.

* Cálculo do seno utilizando a Série de Taylor.

* Cálculo de raiz quadrada utilizando o Método de Heron.

* Cálculo do cosseno a partir da identidade trigonométrica:

  sen²(x) + cos²(x) = 1

* Conversão de graus para radianos.

* Geração de tabela formatada utilizando caracteres Unicode de desenho de linhas.

## Conceitos Utilizados

* Funções recursivas
* Laços de repetição
* Métodos numéricos
* Séries infinitas
* Aproximação de funções matemáticas
* Formatação de saída no terminal

## Estrutura do Programa

As principais funções implementadas são:

| Função        | Descrição                                          |
| ------------- | -------------------------------------------------- |
| `potencia()`  | Calcula uma potência                               |
| `fatorial()`  | Calcula o fatorial de um número                    |
| `seno()`      | Calcula o seno usando Série de Taylor              |
| `raiz()`      | Calcula a raiz quadrada usando o Método de Heron   |
| `cosseno()`   | Calcula o cosseno usando identidade trigonométrica |
| `conversor()` | Converte graus para radianos                       |
| `tabela()`    | Exibe a tabela de senos e cossenos                 |

## Compilação

Utilizando GCC:

```bash
gcc programa.c -o programa
```

## Execução

```bash
./programa
```

## Exemplo de Saída

```text
┌──────┬───────┬───────┐
│angulo│cosseno│seno   │
├──────┼───────┼───────┤
│     0│ 1.0000│ 0.0000│
│     5│ 0.9962│ 0.0872│
...
└──────┴───────┴───────┘
```
