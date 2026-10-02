from collections import deque

# Leitura das entradas
l, c = map(int, input().split())

rec = []
for _ in range (0, l):
    s = input()
    lin = []
    for car in s:
        lin.append(car)
    rec.append(lin)

bomba = (0, int(input()))


# direcoes
direcoes = [
    (-1, 0),  # cima
    (1, 0),   # baixo
    (0, -1),  # esquerda
    (0, 1)    # direita
]

pilha_dfs = [bomba]
visitados = {bomba}

while pilha_dfs:
    i, j = pilha_dfs.pop()

    for di, dj in direcoes:
        ni = i + di
        nj = j + dj

        if 0 <= ni < l and 0 <= nj < c:
            if rec[ni][nj] == '.' and (ni, nj) not in visitados:
                visitados.add((ni, nj))
                pilha_dfs.append((ni, nj))

print(visitados)

def bfs_profundidade(inicio):
    fila = deque([inicio])
    visitados = {inicio}

    maior_linha = inicio[0]

    while fila:
        i, j = fila.popleft()

        maior_linha = max(maior_linha, i)

        for di, dj in [(1, 0), (0, -1), (0, 1)]:
            ni = i + di
            nj = j + dj

            if 0 <= ni < l and 0 <= nj < c:
                if rec[ni][nj] == '.' and (ni, nj) not in visitados:
                    visitados.add((ni, nj))
                    fila.append((ni, nj))

    return maior_linha

# Le a matriz e printa quanto de agua em cada nivel
def agua_nivel (mat):
    aguas_nivel = []
    for lin in mat:
        agua_lin = 0
        for i in lin:
            if i == 'A':
                agua_lin += 1
        aguas_nivel.append(f"{agua_lin}")
    print(f"{' '.join(aguas_nivel)}")


def espalha_lateral(i, j):
    # esquerda
    esq = j - 1
    while esq >= 0 and rec[i][esq] == '.':
        rec[i][esq] = 'A'
        esq -= 1

    # direita
    dir = j + 1
    while dir < c and rec[i][dir] == '.':
        rec[i][dir] = 'A'
        dir += 1

    # a própria posição
    rec[i][j] = 'A'

def escolhe_lado(i, j):
    esquerda = (i, j - 1)
    direita = (i, j + 1)

    if 0 <= j - 1 < c and rec[i][j - 1] == '.':
        prof_esq = bfs_profundidade(esquerda)
    else:
        prof_esq = -1

    if 0 <= j + 1 < c and rec[i][j + 1] == '.':
        prof_dir = bfs_profundidade(direita)
    else:
        prof_dir = -1

    print("ESQ:", prof_esq, "DIR:", prof_dir)

    if prof_esq > prof_dir:
        return esquerda
    elif prof_dir > prof_esq:
        return direita
    else:
        return (i, j)


# loop dos bombeamentos
while True:
    qnt = int(input())
    if qnt == 0:
        break
    for i in range (0, qnt):
        posi, posj = bomba
        while True:
            if posi + 1 < l and rec[posi + 1][posj] == '.':
                posi += 1
                continue

            print("água em:", posi, posj)
            posin, posjn = escolhe_lado(posi, posj)
            if (posjn == posj):
                espalha_lateral(posi, posj)
                break
            posj = posjn


