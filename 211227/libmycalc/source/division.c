//
int division(int const *op1, int const *op2, int *result)
{
    if (*op2 == 0) {
        return 0;
    }

    long operand1 = (long) *op1;
    long operand2 = (long) *op2;
    long quotient = operand1 / operand2;

    if (quotient >= -2147483648 && quotient <= 2147483647) {
        *result = (int) quotient;
        return 1;
    }
    else {
        return 0;
    }
}

int division_init()
{
    const int value1 = 1;
    const int value2 = 0;
    int result = 0;

    division(&value1, &value2, &result);

    return result;
}

