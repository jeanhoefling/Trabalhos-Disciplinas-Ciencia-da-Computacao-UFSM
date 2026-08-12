a, b, c = map(float, input().split())

delta = b*b - 4*a*c

if a == 0 or delta < 0:
    print("Impossivel calcular")
else:
    R1 = (-b + delta**(1/2))/(2*a)
    R2 = (-b - delta**(1/2))/(2*a)
    print(f"R1 = {R1:.5f}")
    print(f"R2 = {R2:.5f}")