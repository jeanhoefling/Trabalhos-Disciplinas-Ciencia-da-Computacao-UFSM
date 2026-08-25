n = int(input())

for i in range (0, n):
    s = input().split()
    s.sort(key=len, reverse=True)
    print(" ".join(s))