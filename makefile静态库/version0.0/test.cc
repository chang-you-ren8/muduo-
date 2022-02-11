#include "add.h"
#include "sub.h"


#include <cstdio>

int main(void)
{
    int a = 10, b = 7;
    printf("a = %d, b = %d\n", a, b);
    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", sub(a, b));

    return 0;
}