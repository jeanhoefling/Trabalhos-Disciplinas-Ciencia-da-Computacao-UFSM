#include <stdio.h>
#define PI 3.1415926535

double potencia (double base, int expoente) {
    double res = 1;
    if (expoente >= 0) {
        while (expoente > 0) {
            res = res * base;
            expoente--;
        }
        return res;
    }
    else {
        expoente = -expoente;
        while (expoente > 0) {
            res = res * base;
            expoente--;
        }
        return 1/res;
    }
}

double fatorial (double numero) {
    // Um valor de retorno padrão caso receba um valor invalido (negativo)
    if (numero < 0) {
        return -1;
    }


    if (numero == 0) {
        return 1;
    }
    return numero * fatorial (numero-1);
}

double seno (double angulo) {
    double termo = angulo;
    double total = 0;
    long i = 1;

    /*Aqui pensei numa solução que não precisava ficar recalculando fatorial e potência
    toda vez, mas deixei assim porque pedia no enunciado para usar as funções anteriores*/
    while (termo > potencia(10, -10) || termo < -potencia(10, -10)) {
        total += termo;
        i += 2;
        termo = potencia(-1, i/2)*(potencia(angulo, i)/fatorial(i));
    }
    
    total += termo;
    return total;
}

double raiz (double numero) {
    // Retorno padrão para valor recebido inválido (não existe raiz de número negativo nos reais)
    if (numero < 0) {
        return -1;
    }

    double termo_anterior = 5, termo_atual = 6;
    while (termo_atual - termo_anterior > potencia(10, -10) || termo_atual - termo_anterior < -potencia(10, -10)) {
        termo_anterior = termo_atual;
        termo_atual = (termo_anterior+numero/termo_anterior)/2;
    }
    return termo_atual;
}


double cosseno (double angulo) {
    double seno_angulo = seno(angulo);

    //If feito para evitar cálculo de uma raiz negativa por imprecisão da função seno
    if (seno_angulo > 1 || seno_angulo < -1) {
        seno_angulo = (int)seno_angulo;
    } 

    double cosseno_angulo = raiz(1-potencia(seno_angulo, 2));
    int quadrante = ((int)(angulo/(PI/2)))%4 + 1;
    if (quadrante == 2 || quadrante == 3) {
        cosseno_angulo = -cosseno_angulo;
    }
    return cosseno_angulo;
}

//Função criada para converter de angulo de radiano para grau
double conversor (int valor) {
    double valor_convertido_em_graus = ((double)valor/180)*PI;
    return valor_convertido_em_graus;
}

void tabela () {
    //Cabeçalho da tabela
    printf("\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n");
    printf("\u2502angulo\u2502cosseno\u2502seno   \u2502cosseno\u2502seno   \u2502cosseno\u2502seno   \u2502cosseno\u2502seno   \u2502cosseno\u2502seno   \u2502\n");
    printf("\u251C\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524\n");

    //Meio da tabela
    for (int i = 0; i <= 79; i +=5) {
        printf("\u2502%6i\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502\n", i, cosseno(conversor(i)), seno(conversor(i)), cosseno(conversor(i+1)), seno(conversor(i+1)), cosseno(conversor(i+2)), seno(conversor(i+2)), cosseno(conversor(i+3)), seno(conversor(i+3)), cosseno(conversor(i+4)), seno(conversor(i+4)));
        printf("\u251C\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u253C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524\n");
    }

    //Inferior da tabela
    printf("\u2502    85\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502%7.4f\u2502\n", cosseno(conversor(85)), seno(conversor(85)), cosseno(conversor(86)), seno(conversor(86)), cosseno(conversor(87)), seno(conversor(87)), cosseno(conversor(88)), seno(conversor(88)), cosseno(conversor(89)), seno(conversor(89)));
    printf("\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n");

}

int main (void) {
    tabela();
}