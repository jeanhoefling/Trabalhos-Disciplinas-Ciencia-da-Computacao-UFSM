num = input()
n = int(num)

def lucky(n):
    for c in n:
        if c not in ("4", "7"):
            return False
    return True


for i in range (1, n+1):
    si = str(i)
    if lucky(si):
        if (n % i == 0):
            print("YES")
            exit()

print("NO")


