#include <stdio.h>
#include <stdbool.h>

int main()
{
    double eps, x;
    scanf("%lf", &eps);
    while (scanf("%lf", &x) != EOF)
    {
        double a = 1, b = 0.5 * (1 + x);
        double f = true;
        while (a - b >= eps || b - a >= eps)
        {
            if (f)
            {
                a = 0.5 * (b + x / b);
            }
            else
            {
                b = 0.5 * (a + x / a);
            }
            f = !f;
        }
        if (f)
        {
            printf("%.10g\n", b);
        }
        else
        {
            printf("%.10g\n", a);
        }
    }
    return 0;
}
