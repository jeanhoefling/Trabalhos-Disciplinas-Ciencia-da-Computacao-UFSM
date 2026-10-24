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
def better_distinctSum(arr):
    n = len(arr)
    total_sum = sum(arr)

    dp = [[False] * (total_sum + 1) for _ in range(n + 1)]

    # Sum 0 is possible using an empty subset
    for i in range(n + 1):
        dp[i][0] = True

    # Fill the DP table in bottom-up manner
    for i in range(1, n + 1):
        dp[i][arr[i - 1]] = True

        for j in range(1, total_sum + 1):
            if dp[i - 1][j]:
                dp[i][j] = True
                dp[i][j + arr[i - 1]] = True

    result = []

    for j in range(total_sum + 1):
        if dp[n][j]:
            result.append(j)

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
        arr.append(i)
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