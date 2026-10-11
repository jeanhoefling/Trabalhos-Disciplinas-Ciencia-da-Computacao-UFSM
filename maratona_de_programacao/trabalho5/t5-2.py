import sys
import heapq
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


# Limites do trecho da linha (do primeiro obstaculo a esquerda ate o primeiro da direita)
def limites_linha(i, j):
    esq = j
    while esq - 1 >= 0 and recipiente[i][esq - 1] != 'O':
        esq -= 1

    dir = j
    while dir + 1 < colunas and recipiente[i][dir + 1] != 'O':
        dir += 1

    return esq, dir


# Verdadeiro se todas as celulas do trecho ja tem agua
def linha_cheia(i, j):
    esq, dir = limites_linha(i, j)

    for c in range(esq, dir + 1):
        if not agua[i][c]:
            return False

    return True



# a ideia é tentar guardar bfs's passados e só refazer bfs's parciais quando necessario
visitadas = {}        # (i, j) -> ordem em que a BFS chegou la (serve de desempate)
fila = deque()        # celulas que ainda precisam ter os vizinhos (re)calculados
candidatos = []       # heap de (-linha, ordem, coluna): lugares onde o litro pode ficar
vazou = False         # True quando alguma BFS alcanca a ultima linha


def visitar(i, j):
    global vazou
    visitadas[(i, j)] = len(visitadas) #só uma segunda forma de prioridade em caso de empate na prof

    if i == linhas - 1:
        vazou = True
        return

    embaixo_livre = recipiente[i + 1][j] != 'O'
    if not agua[i][j] and (not embaixo_livre or agua[i + 1][j]):
        heapq.heappush(candidatos, (-i, visitadas[(i, j)], j))

    fila.append((i, j))


# calcula os vizinhos das celulas da fila com o estado atual
def processar():
    while fila and not vazou:
        i, j = fila.popleft()

        embaixo_livre = recipiente[i + 1][j] != 'O'
        vizinhos = []

        if embaixo_livre and not agua[i + 1][j]:
            vizinhos.append((descer(i, j), j))       # só cai
        else:
            vizinhos += [(i, j - 1), (i, j + 1)]     # obstaculo ou agua embaixo: espalha
            if embaixo_livre:
                vizinhos.append((descer(i, j), j))

        if agua[i][j] and linha_cheia(i, j):         # linha cheia: pode subir
            vizinhos.append((i - 1, j))

        for ni, nj in vizinhos:
            if (0 <= ni < linhas and 0 <= nj < colunas
                    and recipiente[ni][nj] != 'O' and (ni, nj) not in visitadas):
                visitar(ni, nj)
                if vazou:
                    return


# bfs inicial a partir da bomba (so uma vez)
visitar(descer(0, coluna_bomba), coluna_bomba)
processar()


# Retorna false se o litro se perdeu (por baixo ou por cima)
def bombear_um_litro():
    if vazou or not candidatos:
        return False  # vazou pelo fundo ou passou do topo

    # melhor candidato (o mais fundo)
    neg_i, _, j = heapq.heappop(candidatos)
    i = -neg_i

    agua[i][j] = True
    agua_por_linha[i] += 1

    #e a celula de cima agora tem agua embaixo vira acessivel e além de cair espalha
    if i > 0 and recipiente[i - 1][j] != 'O' and (i - 1, j) in visitadas:
        if not agua[i - 1][j]:
            heapq.heappush(candidatos, (-(i - 1), visitadas[(i - 1, j)], j))
        fila.append((i - 1, j))

    # se o trecho encheu, todas as celulas dele podem subir
    if linha_cheia(i, j):
        esq, dir = limites_linha(i, j)
        for c in range(esq, dir + 1):
            if (i, c) in visitadas:
                fila.append((i, c))

    processar()
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