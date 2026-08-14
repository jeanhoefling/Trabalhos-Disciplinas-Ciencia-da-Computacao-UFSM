#include <stdio.h>

int main() {
    int t = 0;
    int i = 0;
    string nome;
    int a;

    while (getline(cin, nome)) {
        cin >> a;
        getchar();

        t += a;
        i++;
    }

    printf("%.1f\n", (double)t / i);

    return 0;
}