//
int subtraction(int const *op1, int const *op2, int *result)
{
    long operand1 = (long) *op1;
    long operand2 = (long) *op2;
    long diff = operand1 - operand2;

    if (diff >= -2147483648 && diff <= 2147483647) {
        *result = (int) diff;
        return 1;
    }
    else {
        return 0;
    }
}

int subtraction_init()
{
    const int value1 = 1;
    const int value2 = 1;
    int result = 0;

    subtraction(&value1, &value2, &result);

    return result;
}