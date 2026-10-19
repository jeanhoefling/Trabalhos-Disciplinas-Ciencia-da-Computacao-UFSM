t = int(input())

for _ in range (t):
    n = int(input())
    sa = input()
    m = int(input())
    sb = input()
    sc = input()

    for i in range(m):
        if sc[i] == 'V':
            sa = f"{sb[i]}{sa}"
        else:
            sa = f"{sa}{sb[i]}"
    print(sa)
