from collections import deque

def criar_arvore (ordem):
    arvore = [[0 for _ in range (ordem)] for _ in range(ordem)]
    return arvore


## Criando a arvore desbalanceada com grande profundidade
# 0-1
#  \
#   2-3-4-5-6-7-8-9-10

desbalanceada = criar_arvore(11)
desbalanceada[0][1] = 1
desbalanceada[0][2] = 1
for i in range (3, 11):
    desbalanceada[i-1][i] = 1


## Criando a arvore balanceada com pouca profundidade
# 0-1-(3,4,5,6)
#  \
#   2-(7,8,9,10)
balanceada = criar_arvore(11)
for i in range (1, 3):
    balanceada[0][i] = 1
for i in range (3, 7):
    balanceada[1][i] = 1
for i in range (7, 11):
    balanceada[2][i] = 1


## Criando a arvore balanceada com grande profundidade
# 0-1-2-3-4-5
#  \
#   6-7-8-9-10
balanceada2 = criar_arvore(11)
for i in range (0, 5):
    balanceada2[i][i+1] = 1
balanceada2[0][6] = 1
for i in range (6, 10):
    balanceada2[i][i+1] = 1


## Funções de busca

def largura (ini, arvore):
    maior_fila = 0
    fila = deque()
    fila.append(ini)
    while len(fila) > 0:
        if len(fila) > maior_fila:
            maior_fila = len(fila)

        no = fila.popleft()
        print(no)

        for filho in range (len(arvore)):
            if arvore[no][filho] == 1:
                fila.append(filho)
    print()
    return maior_fila


def profundidade_recursao (ini, arvore, profundidade=1):
    no = ini
    print(no)

    maior_stack = profundidade
    for filho in range (len(arvore)):
        if arvore[no][filho] == 1:
            res = profundidade_recursao(filho, arvore, profundidade+1)
            if res > maior_stack:
                maior_stack = res
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
        print(no)
        for filho in range(len(arvore)):
            if arvore[no][filho] == 1:
                pilha.append(filho)
    print()
    return maior_pilha

print(f"""-----Arvore desbalanceada com grande profundidade-----
Maior número de nós na fila (largura): {largura(0, desbalanceada)}
Maior pilha na recursiva: {profundidade_recursao(0, desbalanceada)}
Maior pilha na stack padrão: {profundidade_stack(0, desbalanceada)} 
""")

print(f"""-----Arvore balanceada com pouca profundidade-----
Maior número de nós na fila (largura): {largura(0, balanceada)}
Maior pilha na recursiva: {profundidade_recursao(0, balanceada)}
Maior pilha na stack padrão: {profundidade_stack(0, balanceada)} 
""")

print(f"""-----Arvore balanceada com grande profundidade-----
Maior número de nós na fila (largura): {largura(0, balanceada2)}
Maior pilha na recursiva: {profundidade_recursao(0, balanceada2)}
Maior pilha na stack padrão: {profundidade_stack(0, balanceada2)} 
""")


