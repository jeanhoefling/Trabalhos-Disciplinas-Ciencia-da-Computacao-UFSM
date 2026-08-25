s = input()

c = []

for i in range (0, len(s)):
    if (s[i] != "+"):
        c.append(s[i])

if (len(c) == 1):
    print(c[0])
    exit()

c.sort()


for i in range(0, len(c) - 1):
    sres = "+".join(c)
print(sres)