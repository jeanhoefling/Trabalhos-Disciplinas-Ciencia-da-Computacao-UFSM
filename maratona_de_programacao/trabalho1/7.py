n = list(map(float, input().split()))

m1 = (n[0]*2 + n[1]*3 + n[2]*4 + n[3])/10

print(f"Media: {m1:.1f}")
if m1 >= 7:
    print("Aluno aprovado.")
elif m1 < 5:
    print("Aluno reprovado.")
else:
    print("Aluno em exame.")
    ne = float(input())
    print(f"Nota do exame: {ne:.1f}")
    nf = (ne + m1) / 2
    if nf >= 5:
        print("Aluno aprovado.")
    else:
        print("Aluno reprovado.")
    print(f"Media final: {nf:.1f}")