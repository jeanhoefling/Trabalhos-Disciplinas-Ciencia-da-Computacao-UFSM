# Editor de Notas em Terminal

## Descrição

Projeto desenvolvido em linguagem C que simula um quadro de notas adesivas (Post-it) diretamente no terminal.

A aplicação permite criar, editar, mover, redimensionar, pesquisar e organizar notas coloridas em uma área de trabalho. As notas podem ser salvas em arquivos e carregadas posteriormente, mantendo o estado do quadro entre execuções.

## Funcionalidades

* Criação de novas notas
* Edição de texto
* Alteração de cores RGB
* Alteração de etiquetas (tags)
* Busca por texto
* Busca por etiqueta
* Movimentação das notas na tela
* Redimensionamento horizontal e vertical
* Exclusão de notas
* Restauração da última nota removida
* Reorganização da ordem de exibição
* Salvamento e carregamento em arquivos
* Validação de arquivos de entrada
* Registro de erros em arquivo separado

## Conceitos Utilizados

* Structs e tipos compostos
* Enumerações
* Alocação dinâmica de memória (`malloc` e `realloc`)
* Manipulação de arquivos
* Ponteiros
* Modularização em múltiplos arquivos
* Interface em terminal
* Gerenciamento de estados
* Validação de dados

## Estrutura do Projeto

As principais estruturas utilizadas são:

| Estrutura   | Descrição                          |
| ----------- | ---------------------------------- |
| `cor`       | Armazena valores RGB               |
| `retangulo` | Define posição e dimensões da nota |
| `nota`      | Representa uma nota completa       |
| `modo`      | Define o estado atual da aplicação |

Principais funcionalidades implementadas:

| Função | Descrição                  |
| ------ | -------------------------- |
| `INS`  | Insere uma nova nota       |
| `REM`  | Remove uma nota            |
| `ALT`  | Altera o texto de uma nota |
| `COR`  | Modifica a cor da nota     |
| `ETI`  | Altera a etiqueta          |
| `BUS`  | Busca por texto            |
| `BET`  | Busca por etiqueta         |
| `SAL`  | Salva as notas em arquivo  |
| `INI`  | Move nota para o início    |
| `FIM`  | Move nota para o final     |

## Compilação

Utilizando GCC:

```bash
gcc main.c tela.c -o postit
```

## Execução

```bash
./postit
```

Ao iniciar, o programa solicita o arquivo contendo as notas que serão carregadas.

## Exemplo de Uso

```text
+------------------+
| Trabalho de C    |
| Entregar sexta   |
+------------------+

+------------------+
| Reunião          |
| 14:00            |
+------------------+
```

As notas podem ser movidas, redimensionadas, editadas e organizadas livremente na área de trabalho.

## Objetivos do Projeto

Este projeto foi desenvolvido para praticar:

* Manipulação de memória dinâmica
* Estruturas de dados em C
* Persistência de dados em arquivos
* Interfaces em terminal
* Organização de software modular
* Tratamento de entrada do usuário

## Autor

Jean Henrique de Souza Hoefling
