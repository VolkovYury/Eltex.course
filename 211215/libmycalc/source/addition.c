//
int addition(int const *op1, int const *op2, int *result)
{
    long operand1 = (long) *op1;
    long operand2 = (long) *op2;
    long sum = operand1+operand2;

    if (sum>=-2147483648 && sum<=2147483647) {
        *result = (int) sum;
        return 1;
    }
    else {
        return 0;
    }
}

