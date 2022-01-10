//
int addition(int const *op1, int const *op2, int *result)
{
    long operand1 = (long) *op1;
    long operand2 = (long) *op2;
    long sum = operand1 + operand2;

    if (sum >= -2147483648 && sum <= 2147483647) {
        *result = (int) sum;
        return 1;
    }
    else {
        return 0;
    }
}

int addition_init()
{
    const int value1 = 1;
    const int value2 = 0;
    int result = 0;

    addition(&value1, &value2, &result);

    return result;
}