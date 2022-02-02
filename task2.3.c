#include <stdio.h>

int Fib_iter(int n)
{
    if (n < 2)
    {
        return n;
    }
    int prev = 0, current = 1, temp = 1;
    for (int i = 2; i <= n; i++)
    {
        temp = prev + current;
        prev = current;
        current = temp;
    }
    return current;
}

int Fib_rec(int n)
{
    return (n < 2) ? n : Fib_rec(n - 1) + Fib_rec(n - 2);
}

int main()
{
    int n;
    while (scanf("%d", &n) != EOF)
    {
        printf("%d\n", Fib_iter(n));
        printf("%d\n", Fib_rec(n));
    }
    return 0;
}
