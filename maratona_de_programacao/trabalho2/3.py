while True:
    try:
        n = int(input())

        itens = []
        for i in range (1, n + 1):
            itens.append(i)
        itens2 = []

        while len(itens2) < n - 1:
            itens2.extend(map(int, input().split()))

        itens2.sort()

        for i in range(n - 1):
            if itens[i] != itens2[i]:
                print(itens[i])
                break
        else:
            print(n)

    except:
        break