from functools import cache
import time


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
@cache
def cache_distSumRec(arr, i, sum_val):
    if i == len(arr):
        return frozenset([sum_val])

    # Inclui o elemento atual
    incluir = cache_distSumRec(arr, i + 1, sum_val + arr[i])

    # Exclui o elemento atual
    excluir = cache_distSumRec(arr, i + 1, sum_val)

    return incluir | excluir


def cache_distinctSum(arr):
    return sorted(cache_distSumRec(tuple(arr), 0, 0))



if __name__ == "__main__":
    arr = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

    t0 = time.perf_counter()
    result = naive_distinctSum(arr)
    t1 = time.perf_counter()
    result2 = better_distinctSum(arr)
    t2 = time.perf_counter()
    result3 = cache_distinctSum(arr)
    t3 = time.perf_counter()

    print(f"[{', '.join(map(str, result))}], tempo: {t1-t0:.8f} s")
    print(f"[{', '.join(map(str, result2))}], tempo: {t2-t1:.8f} s")
    print(f"[{', '.join(map(str, result3))}], tempo: {t3-t2:.8f} s")