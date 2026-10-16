import time
n = int(input())

pos = [0 for _ in range (n*n)]

rainhas = []

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

def verifica_diagonais(rainhas, i, j):
    for r in rainhas:
        ai, aj = r
        if abs(ai - i) == abs(aj - j) or aj == j:
            return False
    return True

def rec (lin, prof):
    global cont
    if (prof == n):
        cont+=1
        return
    for j in range (0, n):
        if (pos[lin*n +j] == 0 and verifica_diagonais(rainhas, lin, j)):
                rainhas.append((lin, j))
                pos[lin*n + j] = 1
                rec(lin+1, prof+1)
                pos[lin*n + j] = 0
                rainhas.pop()

t1 = time.perf_counter()
cont = 0
rec(0, 0)

print(cont)
t2 = time.perf_counter()
print(t2-t1)


