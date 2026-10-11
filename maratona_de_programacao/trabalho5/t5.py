import sys
from collections import deque

sys.setrecursionlimit(10000)

linhas, colunas = map(int, input().split())

recipiente = []
for _ in range(linhas):
    recipiente.append(input().strip())

coluna_bomba = int(input())

agua = [[False] * colunas for _ in range(linhas)]  # matriz que guarda onde tem agua
agua_por_linha = [0] * linhas                      # vetor de contagem
escapou = False  # se um litro se perdeu, os próximos também (nada mais muda)


# Desce reto ate achar obstaculo
def descer(l, c):
    if l + 1 < linhas and recipiente[l + 1][c] != 'O':
        return descer(l + 1, c)
    return l


# Verdadeiro se todas as celulas da linha (do primeiro obstaculo a esquerda ate o primeiro da direita) 
# ja tem agua
def linha_cheia(i, j):
    esq = j
    while esq - 1 >= 0 and recipiente[i][esq - 1] != 'O':
        esq -= 1

    dir = j
    while dir + 1 < colunas and recipiente[i][dir + 1] != 'O':
        dir += 1

    for c in range(esq, dir + 1):
        if not agua[i][c]:
            return False

    return True



def bfs(linha, coluna):
    visitadas = {(linha, coluna)}
    fila = deque([(linha, coluna)])
    possiveis = []  # posicoes onde o litro pode ficar

    while fila:
        i, j = fila.popleft()

        #buraco no fundo
        if i == linhas - 1:
            return possiveis, True

        embaixo_livre = recipiente[i + 1][j] != 'O'

        if not agua[i][j] and (not embaixo_livre or agua[i + 1][j]):
            possiveis.append((i, j))

        vizinhos = []

        if embaixo_livre and not agua[i + 1][j]:
            vizinhos.append((descer(i, j), j))      # só cai
        else:
            vizinhos += [(i, j - 1), (i, j + 1)]    # obstaculo ou agua embaixo: espalha
            if embaixo_livre:
                vizinhos.append((descer(i, j), j))

        if agua[i][j] and linha_cheia(i, j):
            vizinhos.append((i - 1, j))

        for ni, nj in vizinhos:
            if (0 <= ni < linhas and 0 <= nj < colunas
                    and recipiente[ni][nj] != 'O' and (ni, nj) not in visitadas):
                visitadas.add((ni, nj))
                fila.append((ni, nj))

    return possiveis, False


# Retorna false se o litro se perdeu (por baixo ou por cima)
def bombear_um_litro():
    if recipiente[0][coluna_bomba] == 'O':
        return False

    linha = descer(0, coluna_bomba)
    possiveis, buraco = bfs(linha, coluna_bomba)

    if buraco or not possiveis:
        return False  # vazou pelo fundo ou passou do topo

    # escolhe a mais funda, empate fica a primeira da BFS (mais perto, esquerda primeiro)
    i, j = possiveis[0]

    for ci, cj in possiveis:
        if ci > i:
            i, j = ci, cj

    agua[i][j] = True
    agua_por_linha[i] += 1
    return True


def bombear(qnt):
    global escapou

    for _ in range(qnt):
        if escapou:
            break
        n_escap = bombear_um_litro()
        if not n_escap:
            escapou = True

while True:
    qnt = int(input())

    if qnt == 0:
        break

    bombear(qnt)

    print(' '.join(map(str, agua_por_linha)))