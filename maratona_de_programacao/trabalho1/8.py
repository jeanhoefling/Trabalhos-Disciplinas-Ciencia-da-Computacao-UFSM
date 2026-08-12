t = list(map(int, input().split()))

m = 0
h = 0

if t[3] > t[1]:
    m = t[3] - t[1]
elif t[3] == t[1]:
    m = 0
else:
    m = 60 + (t[3] - t[1])

if t[2] > t[0]:
    h = t[2] - t[0]
    if t[3] < t[1]:
        h -= 1
elif t[2] < t[0]:
    h = 24 + (t[2] - t[0])
    if t[3] < t[1]:
        h -= 1
else:
    if m == 0:
        h = 24
    elif t[3] > t[1]:
        h = 0
    else:
        h = 23
    
print(f"O JOGO DUROU {h} HORA(S) E {m} MINUTO(S)")