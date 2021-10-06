#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define uint1024 unsigned long*
#define FIELD_SIZE 9
#define SIZE 35
#define SIZE_CHAR 160
unsigned long long max = 1000000000;


uint1024 init();
uint1024 from_uint(unsigned int);
uint1024 add_op(uint1024, uint1024);
uint1024 subtr_op(uint1024, uint1024);
void printf_value(uint1024);
void prt(unsigned long, int);
uint1024 multp_op(uint1024, uint1024);
void scanf_val(uint1024);


int main() {
    uint1024 a = init();
    uint1024 b = from_uint(2);
    scanf_val(a);

    printf_value(multp_op(a, b));
    return 0;
}

void prt(unsigned long x, int i) {
    if (i == FIELD_SIZE) 
        return ;
    prt(x / 10, ++i);
    printf("%c", x % 10 + '0');
}

void printf_value(uint1024 x) {
    int i = SIZE - 1;
    for (i; *(x + i) == 0; --i) ;
    
    printf("%d", *(x + i--));
    
    for (i;i >= 0; --i)
        prt(*(x + i), 0);
}

uint1024 init() {
    uint1024 buff = malloc(SIZE * sizeof(unsigned long));
    for (int i = 0; i < SIZE; ++i)
        *(buff + i) = 0;
    return buff;    
}

uint1024 from_uint(unsigned int x) {
    uint1024 buff = init();
    int i = 0;
    while (x != 0) {
        *(buff + i) = x % max;
        x /= max;
        i++;
    }
    return buff;
}

uint1024 add_op(uint1024 a, uint1024 b) {
    uint1024 buff = init();
    int o = 0;
    unsigned long long sm;
    for (int i = 0; i < SIZE; ++i) {
        sm = *(a + i) + *(b + i) + o;
        *(buff + i)= sm % max;
        o = sm / max;
    }
    return buff;
}

uint1024 subtr_op(uint1024 a, uint1024 b) {
    uint1024 buff = init();
    signed long long o = 0;
    signed long long sm;
    for (int i = 0; i < SIZE; ++i) {
        sm = (long long )*(a + i) - (long long) *(b + i) - o;
        *(buff + i) = (sm >= 0) ? sm : max + sm;
        o = (sm >= 0) ? 0 : 1;
    }
    return buff;
}

uint1024 multp_op(uint1024 a, uint1024 b) {
    uint1024 buff = init();
    unsigned long long pr_buff = 0;
    unsigned long long o = 0;

    for (int pa = 0; pa < SIZE; ++pa) {
        for (int pb = 0; pa + pb < SIZE; ++pb) {
            pr_buff =( (long long)*(a + pa) )* ((long long)*(b + pb)) + o;
            buff[pa + pb] += pr_buff % max;
            o = pr_buff / max;
        }
    }
    return buff;
}

void scanf_val(uint1024 a) {
    char str[SIZE_CHAR];
    int k = 0;
    int i = 0;
    for (i; i < SIZE_CHAR; ++i) {
        str[i] = '\0';
    }
    scanf("%s", &str);
    while(str[i] == '\0' ) {i -= 1;}

    while (i >= 0) {
        *(a + (k / FIELD_SIZE)) += (str[i] - '0') * pow(10, k % FIELD_SIZE);
        ++k;
        --i;
    }
}