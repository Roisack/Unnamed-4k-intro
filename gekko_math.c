float linear_interpolation(int a, int b, int c)
{
    float out;
    out = a * (1.0 - c) + c * b;
    return out;
}

