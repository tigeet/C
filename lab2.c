#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define FIELD_SIZE 9//ширина поля вывода в printf_value()
#define SIZE 35//размер массива long
#define SIZE_CHAR 310//размер строки в scanf_val()
const unsigned long long MAX = 1000000000;//макс. значение ячеек массива

typedef struct {
    unsigned long value[SIZE];
} uint1024_t;

void clear(uint1024_t * x);
void print_digit(unsigned long, int);

uint1024_t from_uint(unsigned int x);
uint1024_t add_op(uint1024_t x, uint1024_t y);
uint1024_t subtr_op(uint1024_t x, uint1024_t y);
uint1024_t multp_op(uint1024_t x, uint1024_t y);
void printf_value(uint1024_t x);
void scanf_val(uint1024_t* x);


int main() {
    uint1024_t a;
    uint1024_t b = from_uint(123);
    scanf_val(&a);

    uint1024_t c = from_uint(1); uint1024_t d = from_uint(2);
    for (int i = 0; i < 1024; ++i) {
        c = multp_op(c, d);
    }

    printf_value(c); printf("\n");

    printf_value(add_op(a, b)); printf("\n");
    printf_value(subtr_op(a, b)); printf("\n");
    printf_value(multp_op(a, b));
    return 0;
}

void clear(uint1024_t *x) {
    for (int i = 0; i <= SIZE; ++i) {
        x->value[i] = 0;
    }
}


void print_digit(unsigned long x, int i) {
    if (i == FIELD_SIZE) {
        return ;
    }

    print_digit(x / 10, ++i);
    printf("%c", x % 10 + '0');
}


void scanf_val(uint1024_t *x) {
    clear(x);
    char input[SIZE_CHAR];
    int i = 0;
    int n = 0;

    for (n; n < SIZE_CHAR; ++n) {
        input[n] = '\0';
    }

    scanf("%s", &input);
    while(input[n] == '\0' ) {
        n -= 1;
    }

    while (n >= 0) {
        x->value[i / FIELD_SIZE] += (input[n] - '0') * pow(10, i % FIELD_SIZE);
        ++i;
        --n;
    }
}


void printf_value(uint1024_t x) {
    int i;

    for (i = SIZE - 1; x.value[i] == 0; --i) ;
    
    printf("%d", x.value[i]);
    
    for (i -= 1;i >= 0; --i) {
        print_digit(x.value[i], 0);
    }
}


uint1024_t from_uint(unsigned int x) {
    uint1024_t result;
    clear(&result);
    int i = 0;

    while (x != 0) {
        result.value[i] = x % MAX;
        x /= MAX;
        i++;
    }

    return result;
}


uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    clear(&result);
    int shift = 0;
    unsigned long long digitSum;

    for (int i = 0; i < SIZE; ++i) {
        digitSum = x.value[i] + y.value[i] + shift;
        result.value[i] = digitSum % MAX;
        shift = digitSum / MAX;
    }
    return result;
}


uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    clear(&result);
    signed long long shift = 0;
    signed long long digitSum;
    for (int i = 0; i < SIZE; ++i) {
        digitSum = (long long )x.value[i] - (long long) y.value[i] - shift;
        result.value[i] = (digitSum < 0) ? MAX + digitSum : digitSum;
        shift = (digitSum < 0) ? 1 : 0;
    }
    return result;
}


uint1024_t multp_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    clear(&result);
    unsigned long long product;
    unsigned long long shift = 0;

    for (int pa = 0; pa < SIZE; ++pa) {
        for (int pb = 0; pa + pb < SIZE; ++pb) {
            product =((long long)x.value[pa])* ((long long)y.value[pb]);
            product += shift;
            result.value[pa + pb] += product % MAX;
            shift = product / MAX;
        }
    }
    return result;
}