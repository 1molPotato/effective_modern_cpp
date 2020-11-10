#include <stdio.h>

void foo(const int a) {
    printf("%d", a);
}

int main() {
    foo(2);
    return 0;
}