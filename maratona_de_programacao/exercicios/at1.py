n = int(input())

pos = [0 for _ in range (n*n)]

visitados = set()

def printa(pos):
    for indice, p in enumerate(pos):
        if (indice%n == 0):
            print()
        if p == 1:
            print("O", end="")
        else:
            print(".", end="")

def conta_1 (pos):
    cont = 0
    for p in pos:
        if p == 1:
            cont += 1
    return cont


def rec (lin, col, prof):
    if (prof == n):
        tup = tuple(pos)
        if tup not in visitados:
            visitados.add(tup)
            printa(pos)
            print()
        return
    for i in range (lin, n):
        for j in range (col, n):
            if (pos[i*n +j] == 0):
                pos[i*n + j] = 1
                rec(i, j, prof+1)
                pos[i*n + j] = 0
        col = 0

rec(0, 0, 0)
print(len(visitados))


