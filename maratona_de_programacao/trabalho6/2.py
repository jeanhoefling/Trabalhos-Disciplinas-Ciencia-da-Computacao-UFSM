num = int(input())

l = list(map(int, input().split()))

cont_0 = 0
cont_1 = 0

for i in range (3):
    if (l[i] % 2 == 0):
        cont_0 += 1
    else:
        cont_1 += 1

if cont_0 > cont_1:
    for ind, n in enumerate(l):
        if n % 2 == 1:
            print(ind+1)
else:
    for ind, n in enumerate(l):
        if n % 2 == 0:
            print(ind+1)