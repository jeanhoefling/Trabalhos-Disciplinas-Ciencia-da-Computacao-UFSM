import time
import sys
from typing import Tuple


def closestPair1(a, b, x):
    minDiff = sys.maxsize
    res = (0, 0)

    for i in range(len(a)):
        for j in range(len(b)):
            diff = abs(a[i] + b[j] - x)

            if diff < minDiff:
                minDiff = diff
                res = (a[i], b[j])

    return res


def closestPair2(a, b, x):
    l, r = 0, len(b) - 1
    diff = float('inf')
    result = [0, 0]

    while l < len(a) and r >= 0:
        s = a[l] + b[r]
        currDiff = abs(s - x)
        if currDiff < diff:
            diff = currDiff
            result[0] = a[l]
            result[1] = b[r]
        if s > x:
            r -= 1
        else:
            l += 1

    return result



# Dois vetores ordenados
a = list(range(1, 1001))
b = list(range(1, 1001))

# Valor para aproximir
x = 1997


inicio_naive = time.perf_counter()

resultado_naive = closestPair1(a, b, x)

fim_naive = time.perf_counter()

tempo_naive = fim_naive - inicio_naive



inicio_dois_pont = time.perf_counter()

resultado_dois_pont = closestPair2(a, b, x)

fim_dois_pont = time.perf_counter()

tempo_dois_pont = fim_dois_pont - inicio_dois_pont


print(f"Valor procurado: {x}")
print()

print("=== Busca Linear ===")
print(f"Resultado: {resultado_naive}")
print(f"Soma: {resultado_naive[0] + resultado_naive[1]}")
print(f"Tempo: {tempo_naive:.6f} segundos")

print()

print("=== Dois Ponteiros ===")
print(f"Resultado: {resultado_dois_pont}")
print(f"Soma: {resultado_dois_pont[0] + resultado_dois_pont[1]}")
print(f"Tempo: {tempo_dois_pont:.6f} segundos")

print()

print("=== Comparação ===")
print(f"Diferença de tempo: {tempo_naive - tempo_dois_pont:.6f} segundos")
print(
    f"Busca linear foi aproximadamente "
    f"{tempo_naive / tempo_dois_pont:.2f}x mais lenta"
)