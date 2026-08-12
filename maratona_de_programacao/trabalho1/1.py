t = 0
i = 0
while True:
    try:
        s = str(input())
        a = int(input())
        t+=a
        i +=1
    except:
        break;

print(f"{(t/i):.1f}")