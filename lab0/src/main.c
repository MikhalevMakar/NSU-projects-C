#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

long long int NewDigit(char number, int b1) {
    if (number >= '0' && number <= '9' && number - '0' < b1) {
        int digit = number - '0';
        return digit;
    }
    else if (number >= 'a' && number <= 'f' && number - 'a' + 10 < b1) {
        int digit = number - 'a' + 10;
        return digit;
    }
    else {
        return -1;
    }
}

int CheckOnB1B2QuantityDot(int b1, int b2, char* number, int N) {
    int sum = 0;
    int notGarbage = 0;
    char digit[] = "0123456789abcdef.";
    if (b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
        return 0;
    }
    for (int i = 0; i < N; ++i) {
        char lower = tolower(number[i]);
        for (unsigned int j = 0; j < sizeof(digit) - 1; ++j) {
            if (digit[j] == lower) {
                notGarbage++;
            }
        }
        if (number[i] != '.' && NewDigit(lower, b1) == -1) {
            return 0;
        }
        if (lower == '.') {
            sum++;
        }
    }
    if (notGarbage != N || sum > 1) {
        return 0;
    }
    return 1;
}

int SearchFractionalPartChackOnDot(char* number, int N) {
    char* b = strchr(number, '.');
    int dot = b - number;
    if (b == NULL) {
        return 0;
    }
    else if (dot == 0 || dot == N - 1) {
        return -1;
    }
    else {
        return dot;
    }
}

long long int IntegerFromB1(char* number, int b1, int N) {
    long long int value = 0;
    for (int i = 0; i < N; i++) {
        char lower = tolower(number[i]);
        value = value * b1 + NewDigit(lower, b1);
    }
    return value;
}

double FractionalFromB1(char* number, int dot, int b1, int N) {
    double value = 0;
    for (int i = dot + 1; i < N; i++) {
        char lower = tolower(number[i]);
        int degree = dot - i;
        value += NewDigit(lower, b1) * pow(b1, degree);
    }
    return value;
}

void FractionIntoB2(double fractional_value, int b2) {
    int N = 45;
    for (int i = 0; i < N; i++) {
        char list[] = "0123456789abcdef";
        double new_fractional = b2 * fractional_value;
        int int_digit = new_fractional;
        printf("%c", list[int_digit]);
        fractional_value = new_fractional - int_digit;
    }
}

void IntegerIntoB2(long long int  value, int b2) {
    char list[] = "0123456789abcdef";
    if (value < b2) {
        printf("%c", list[value]);
        return;
    }
    IntegerIntoB2(value / b2, b2);
    printf("%c", list[value % b2]);
}

int main() {
    char number[15];
    number[14] = '\0';
    int b1, b2;
    long long int  value;
    if (scanf("%d %d %13s", &b1, &b2, number) != 3) {
        return 0;
    }
    int N = strlen(number);
    int isOk = CheckOnB1B2QuantityDot(b1, b2, number, N);
    int dot = SearchFractionalPartChackOnDot(number, N);
    if (isOk == 0 || dot == -1) {
        printf("bad input");
        return 0;
    }
    else if (dot == 0) {
        value = IntegerFromB1(number, b1, N);
        IntegerIntoB2(value, b2);
    }
    else {
        value = IntegerFromB1(number, b1, dot);
        double fractionalValue = FractionalFromB1(number, dot, b1, N);
        if (value > 0) {
            IntegerIntoB2(value, b2);
        }
        else {
            printf("0");
        }
        printf(".");
        FractionIntoB2(fractionalValue, b2);
    }
    return 0;
}

