#include <bits/stdc++.h>
using namespace std;

int main() {
    int t[4];
    int m = 0;
    int h = 0;

    cin >> t[0] >> t[1] >> t[2] >> t[3];

    if (t[3] > t[1]) {
        m = t[3] - t[1];
    }
    else if (t[3] == t[1]) {
        m = 0;
    }
    else {
        m = 60 + (t[3] - t[1]);
    }

    if (t[2] > t[0]) {
        h = t[2] - t[0];

        if (t[3] < t[1]) {
            h -= 1;
        }
    }
    else if (t[2] < t[0]) {
        h = 24 + (t[2] - t[0]);

        if (t[3] < t[1]) {
            h -= 1;
        }
    }
    else {
        if (m == 0) {
            h = 24;
        }
        else if (t[3] > t[1]) {
            h = 0;
        }
        else {
            h = 23;
        }
    }

    printf("O JOGO DUROU %d HORA(S) E %d MINUTO(S)\n", h, m);

    return 0;
}