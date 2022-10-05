#pragma once
#include <stdio.h>

double computePI(int n) {
    if (n < 0)
        return 0.0;

    double sum = 1.0;
    int sign = -1;

    for (int i = 1; i < n; i++) {
        sum += sign / (2.0 * i + 1.0);
        sign = -sign;
    }

    return 4.0 * sum;
}

const size_t size = 10;
struct Random
{
    int offset;
    int n[size];

    void randomise(int seed)
    {
        for (int i = 0; i < size; i++)
        {
            printf("%lf\n", computePI(i + seed));
        }
    }
};