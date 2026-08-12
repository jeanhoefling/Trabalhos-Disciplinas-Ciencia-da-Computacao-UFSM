a = list(map(float, input().split()))
b = list(map(float, input().split()))

print(f"VALOR A PAGAR: R$ {a[1] * a[2] + b[1] * b[2]:.2f}")