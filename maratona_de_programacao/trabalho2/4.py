s = str(input())

vog = []

vogais = ["a", "e", "i", "o", "u"]
for i in range (0, len(s)):
    if (s[i] in vogais):
        vog.append(s[i])

for i in range (0, len(vog)//2):
    if vog[i] != vog[len(vog) -1 -i]:
        print("N")
        exit()

print("S")