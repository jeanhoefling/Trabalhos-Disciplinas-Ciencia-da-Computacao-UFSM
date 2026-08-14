#include <bits/stdc++.h>
using namespace std;

int main() {
    string nome;
    cin >> nome;

    double a, b;

    cin >> a;
    cin >> b;

    printf("TOTAL = R$ %.2f\n", a + (b * 15 / 100));

    return 0;
}