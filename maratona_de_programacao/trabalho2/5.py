a, b = map(int, input().split())

li = list(map(int, input().split()))

d={}
if (b >= 2):
    for i in range (0, a):
        if (b - li[i] in d):
            pos2 = d[b - li[i]]
            li2 = [i + 1, pos2]
            li2.sort()
            if (li2[0] != li2[1]):
                print(f"{li2[0]} {li2[1]}")
                exit()
        d[li[i]] = i+1
print("IMPOSSIBLE")
