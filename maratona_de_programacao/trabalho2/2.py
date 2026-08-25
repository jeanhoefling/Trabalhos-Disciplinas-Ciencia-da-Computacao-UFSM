a, b = map(int, input().split())

casas = list(map(int, input().split()))

entregas = list(map(int, input().split()))

d = {}
for i in range (a):
    d[casas[i]] = i

atual = 0
temp = 0;

for entrega in entregas:
    temp += abs(d[entrega] - atual)
    atual = d[entrega]
print(temp)