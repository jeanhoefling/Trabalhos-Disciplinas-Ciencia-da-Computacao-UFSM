from functools import cache
import time
import sys

sys.setrecursionlimit(10000)

## NAIVE
def naive_distSumRec(arr, n, sum_val, i, s):
    if i > n:
        return

    if i == n:
        s.add(sum_val)
        return

    # Include the current element
    naive_distSumRec(arr, n, sum_val + arr[i], i + 1, s)

    # Exclude the current element
    naive_distSumRec(arr, n, sum_val, i + 1, s)


def naive_distinctSum(arr):
    s = set()
    n = len(arr)

    naive_distSumRec(arr, n, 0, 0, s)

    return sorted(s)


## BETTER SEM CACHE
def better_distSumRec(arr, n, sum_val, i, memo):
    if i == n:
        memo[i][sum_val] = 1
        return

    if memo[i][sum_val] != -1:
        return

    # Mark the current state as visited
    memo[i][sum_val] = 1

    # Include the current element
    better_distSumRec(arr, n, sum_val + arr[i], i + 1, memo)

    # Exclude the current element
    better_distSumRec(arr, n, sum_val, i + 1, memo)


# Generate distinct subset sums using memoization
def better_distinctSum(arr):
    n = len(arr)
    total_sum = sum(arr)

    memo = [[-1] * (total_sum + 1) for _ in range(n + 1)]

    better_distSumRec(arr, n, 0, 0, memo)

    result = []

    for sum_val in range(total_sum + 1):
        if memo[n][sum_val] == 1:
            result.append(sum_val)

    return result


## BETTER COM CACHE
def cache_distinctSum(arr):
    n = len(arr)
    resultado = set()

    @cache
    def rec(i, sum_val):
        if i == n:
            resultado.add(sum_val)
            return
        rec(i + 1, sum_val + arr[i])
        rec(i + 1, sum_val)

    rec(0, 0)
    return sorted(resultado)


tams = [10, 20, 25, 50, 100, 150]

for tam in tams:
    arr = []
    for i in range (tam):
        arr.append(i) #Testar com i*100, i*1000...
    if tam <= 25:
        t0 = time.perf_counter()
        result = naive_distinctSum(arr)
    t1 = time.perf_counter()
    result2 = better_distinctSum(arr)
    t2 = time.perf_counter()
    result3 = cache_distinctSum(arr)
    t3 = time.perf_counter()

    print(f"-------- N = {tam} --------")
    if tam <= 25:
        print(f"Solução Naive: {t1-t0:.8f} s")
    print(f"Solução Better sem cache: {t2-t1:.8f} s")
    print(f"Solução Better com cache: {t3-t2:.8f} s")
    print()