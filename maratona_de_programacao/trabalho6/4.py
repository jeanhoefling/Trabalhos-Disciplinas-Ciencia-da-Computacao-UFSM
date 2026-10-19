lin, col = map(int, input().split())

mat = []

validas = 1

for _ in range (lin):
    mat.append(list(input()))

def giro_90(mat):
    mat2 = []
    for j in range (len(mat[0])):
        l = []
        for i in range (len(mat)):
            l.append(mat[i][j])
        l.reverse()
        mat2.append(l)
    return mat2

def giro_cima_baixo(mat):
    mat2 = []
    for i in range (len(mat)):
        l = []
        for j in range (len(mat[0])):
            l.append(mat[lin - i - 1][j])
        mat2.append(l)
    return mat2

mat2 = giro_90(mat)
if mat2 == mat:
    validas += 1
    
mat2 = giro_90(mat2)
if mat2 == mat:
    validas += 1

mat2 = giro_90(mat2)
if mat2 == mat:
    validas += 1

mat2 = giro_cima_baixo(mat)
if mat2 == mat:
    validas += 1

mat2 = giro_90(mat2)
if mat2 == mat:
    validas += 1

mat2 = giro_90(mat2)
if mat2 == mat:
    validas += 1

mat2 = giro_90(mat2)
if mat2 == mat:
    validas += 1

print(validas)