a = int(input())

h = a // 3600
a = a % 3600

m = a // 60
a = a % 60

print(f"{h}:{m}:{a}")