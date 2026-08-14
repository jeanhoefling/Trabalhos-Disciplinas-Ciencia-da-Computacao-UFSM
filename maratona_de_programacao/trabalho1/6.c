#include <stdio.h>

int main() {
    int a;

    scanf("%d", &a);

    int h = a / 3600;
    a = a % 3600;

    int m = a / 60;
    a = a % 60;

    printf("%d:%d:%d\n", h, m, a);

    return 0;
}