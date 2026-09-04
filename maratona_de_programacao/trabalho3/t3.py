from collections import deque

arvore = [
    # 0  1  2  3  4  5  6  7  8  9
    [0, 1, 1, 0, 0, 0, 0, 0, 0, 0],  # 0
    [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],  # 1
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],  # 2
    [0, 0, 0, 0, 0, 1, 0, 0, 0, 0],  # 3
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],  # 4
    [0, 0, 0, 0, 0, 0, 1, 0, 0, 0],  # 5
    [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],  # 6
    [0, 0, 0, 0, 0, 0, 0, 0, 1, 1],  # 7
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],  # 8
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]   # 9
]

def largura (ini, arvore):
    fila = deque()
    fila.append(ini)
    while len(fila) > 0:
        no = fila.popleft()
        print(no)

        for filho in range (len(arvore)):
            if arvore[no][filho] == 1:
                fila.append(filho)


def profundidade_recursao (ini, arvore):
    no = ini
    print(no)
    for filho in range (len(arvore)):
        if arvore[no][filho] == 1:
            profundidade_recursao(filho, arvore)


def profundidade_stack (ini, arvore):
    pilha = []
    pilha.append(ini)
    while len(pilha) > 0:
        no = pilha.pop()
        print(no)
        for filho in range(len(arvore)):
            if arvore[no][filho] == 1:
                pilha.append(filho)