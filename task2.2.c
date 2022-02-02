#include <stdio.h>

int main()
{
    int n;
    double a, x;
    scanf("%d %lf %lf", &n, &x, &a);
    int k = n - 1;
    double f = a, df = 0, Int_f = a / (n + 1) * x;
    while (scanf("%lf", &a) != EOF)
    {
        df = df * x + f;
        f = f * x + a;
        Int_f = (Int_f + a / (k + 1)) * x;
        k--;
    }
    printf("%lf\n%lf\n%lf", f, df, Int_f);
    return 0;
}
