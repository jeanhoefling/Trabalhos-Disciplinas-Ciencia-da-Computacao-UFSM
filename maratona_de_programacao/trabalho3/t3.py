from collections import deque

def criar_arvore (ordem):
    arvore = [[0 for _ in range (ordem)] for _ in range(ordem)]
    return arvore


## Criando a arvore com ramos longos e não balanceada

arvore = criar_arvore(30)
for i in range (0, 5):
    arvore[i][i+1] = 1
arvore[4][6] = 1
for i in range (6, 8):
    arvore[i][i+1] = 1
arvore[7][9] = 1
arvore[9][10] = 1
arvore[1][13] = 1
arvore[2][11] = 1
arvore[11][12] = 1
for i in range(13, 17):
    arvore[i][i+1] = 1
arvore[16][18] = 1
arvore[18][19] = 1
arvore[0][20] = 1
for i in range(20, 27):
    arvore[i][i+1] = 1
arvore[21][29] = 1
arvore[25][28] = 1

## Funções de busca

def largura (ini, arvore):
    maior_fila = 0
    fila = deque()
    fila.append(ini)
    while len(fila) > 0:
        if len(fila) > maior_fila:
            maior_fila = len(fila)

        no = fila.popleft()
        print(f"{no}", end="")

        for filho in range (len(arvore)):
            if arvore[no][filho] == 1:
                fila.append(filho)
        print(f" (na fila: {' '.join(str(n) for n in fila)})")
    return maior_fila


def profundidade_recursao (ini, arvore, caminho=[]):
    no = ini
    caminho.append(no)

    print(f"{no} (na pilha: {' '.join(str(n) for n in caminho)})")

    maior_stack = len(caminho)
    for filho in range (len(arvore)):
        if arvore[no][filho] == 1:
            res = profundidade_recursao(filho, arvore, caminho)
            if res > maior_stack:
                maior_stack = res
    caminho.pop()
    return maior_stack


def profundidade_stack (ini, arvore):
    print()
    maior_pilha = 0
    pilha = []
    pilha.append(ini)
    while len(pilha) > 0:
        if len(pilha) > maior_pilha:
            maior_pilha = len(pilha)
        no = pilha.pop()
        print(f"{no}", end="")

        for filho in range(len(arvore)):
            if arvore[no][filho] == 1:
                pilha.append(filho)
        print(f" (na pilha: {' '.join(f"{p}" for p in pilha)})")
    return maior_pilha

print(f"-- Maior número de nós na fila (largura): {largura(0, arvore)} --")
print()
print(f"-- Maior pilha na recursiva: {profundidade_recursao(0, arvore)} --")
print(f"-- Maior pilha na stack padrão: {profundidade_stack(0, arvore)} --")
