#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define uint1024 unsigned long*
#define SIZE 140
#define FIELD_SIZE 9
unsigned long long max = 1000000000;
//int max = 100;
int size = 35;

//done
uint1024 init();
uint1024 from_uint(unsigned int);
uint1024 add_op(uint1024, uint1024);
//todo
uint1024 subtr_op(uint1024, uint1024);//мб работает
void printf_value(uint1024);//мб работает, съедала нули
void prt(unsigned long, int); //вспом. к printf_value
uint1024 multp_op(uint1024, uint1024);


int main() {
    uint1024 a = init();
    uint1024 b = init();
    a = from_uint(2000);
    b = from_uint(2000);
    uint1024 c = init();
    uint1024 d = init();
    c = from_uint(2000);
    d = from_uint(2000);
    uint1024 e = init();
    uint1024 f = init();
    e = from_uint(2000);
    f = from_uint(2000);

    //printf_value(multp_op(multp_op(multp_op(a, b), multp_op(c, d)), multp_op(e, f)));
    printf_value(add_op(  multp_op(multp_op(a, b), multp_op(c, d)), multp_op(e, f)  ));
    // uint1024 c = subtr_op(a, b);
    // for (int i = 0; i < size; ++i) {
    //     printf("%d ", *(c + i));
    // }
    return 0;
}


void prt(unsigned long x, int i) {
    if (i == FIELD_SIZE) 
        return ;
    prt(x / 10, ++i);
    printf("%c", x % 10 + '0');
}

void printf_value(uint1024 x) {
    int i = size - 1;
    for (i; *(x + i) == 0; --i) ;
    
    printf("%d", *(x + i--));
    
    for (i;i >= 0; --i)
        prt(*(x + i), 0);
}

uint1024 init() {
    uint1024 buff = malloc(SIZE);
    for (int i = 0; i < size; ++i)
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
    for (int i = 0; i < size; ++i) {
        sm = *(a + i) + *(b + i) + o;
        *(buff + i) = sm % max;
        o = sm / max;
    }
    return buff;
}

uint1024 subtr_op(uint1024 a, uint1024 b) {
    uint1024 buff = init();
    signed long long o = 0;
    signed long long sm;
    for (int i = 0; i < size; ++i) {
        sm = (long long )*(a + i) - (long long) *(b + i) - o;
        *(buff + i) = (sm >= 0) ? sm : max + sm;
        o = (sm >= 0) ? 0 : 1;
    }
    return buff;
}


uint1024 multp_op(uint1024 a, uint1024 b) {
    uint1024 buff = init();
    unsigned long long pr_buff = 0;
    unsigned long long c = 0;
    
    for (int pa = 0; pa < size; ++pa) {
        for (int pb = 0; pb < size; ++pb) {
            pr_buff =( (long long)*(a + pa) )* ((long long)*(b + pb)) + c;
            buff[pa + pb] += pr_buff % max;
            c = pr_buff / max;
        }
    }
    return buff;
}



//uint1024 * scanf_val