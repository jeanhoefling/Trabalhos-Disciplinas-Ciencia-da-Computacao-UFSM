def LIS_DP(arr, n):
    lis = [1] * n
    dic = {}
    dic[0] = f"{arr[0]}"
    for i in range (1, n):
        for j in range (i):
            if (arr[i] > arr[j] and lis[i] <= lis[j] ):
                lis[i] = lis[j] + 1;
                dic[i] = f"{dic[j]} -> {arr[i]}"
    max_val = lis[0]
    max_ind = 0
    for ind, val in enumerate(lis):
        if val > max_val:
            max_ind = ind
            max_val = val
    print(dic[max_ind])
    return max_val


inp = [3, 10, 2, 1, 20]
print(LIS_DP(inp, len(inp)))

