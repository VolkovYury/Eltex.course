//
int multiplication(int const *op1, int const *op2, int *result)
{
    long operand1 = (long) *op1;
    long operand2 = (long) *op2;
    long prod = operand1*operand2;

    if (prod>=-2147483648 && prod<=2147483647) {
        *result = (int) prod;
        return 1;
    }
    else {
        return 0;
    }
}