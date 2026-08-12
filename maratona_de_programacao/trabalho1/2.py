notas = [10000, 5000, 2000, 1000, 500, 200]
moedas = [100, 50, 25, 10, 5, 1]

v = float(input())
v*=100

print("NOTAS:")

for nota in notas:
    print(f"{v // nota:.0f} nota(s) de R$ {nota/100:.2f}")
    v = v % nota

print("MOEDAS:")

for moeda in moedas:
    print(f"{v // moeda:.0f} moeda(s) de R$ {moeda/100:.2f}")
    v = v % moeda