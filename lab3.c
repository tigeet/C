#include <stdio.h>
#include <time.h>
#include<math.h>
#include<string.h>


#define size  400//размер строки запроса
long timestamp[1891714]; int n = 0;//


long utime(char *stime);//преобразует строку времени в unix timestamp
long gettime(char[]);
int geterror(char[]);
void getwindow(long);


int main () {
    FILE* file = fopen("access_log_Jul95", "r");
    int errorCount = 0;
    long wsize = 0;

    char str[size];
    while (fgets(str, size, file)) {
        timestamp[n++] = gettime(str);
        if ((geterror(str) / 100) == 5){
            ++errorCount;
        }
    }

    printf("enter window size\n");
    scanf("%d", &wsize);
    getwindow(wsize);
    
    printf("\n5xx errors: %d\n", errorCount);

    return 0;
}


void getwindow(long width) {
    long m = 0;//макс. окно
    long mp1 = 0; long mp2 = 0;//индексы границ макс. окна
    long p1 = 0; long p2 = 0;//индексы границ текущего окна
    while (p2 < n - 1) {
        if (timestamp[p2] - timestamp[p1] <= width) {
            ++p2;
        } else {
            if (p2 - p1 > m) {
                m = p2 - p1;
                mp1 = p1 + 1;
                mp2 = p2;
            }
            p1++;
        }
    }
    if ((timestamp[p2] - timestamp[p1] <= width) && (p2 - p1 > m)) {
        m = p2 - p1 + 1;
        mp1 = p1 + 1;
        mp2 = p2 + 1;
    }
    printf("count = %d, from %d to %d", m, mp1, mp2);
}

long gettime(char str[]) {
    char buff[40];//хранит строку со временем
    int ptr = 0;//индекс для времени
    int i = 0;//индекс строки
    while (str[i++] != '[');
        
    while (str[i] != ']')
        buff[ptr++] = str[i++];
    return utime(buff);
}

int geterror(char str[]) {
    char buff[4];//хранит код ошибка
    int ptr = 2;//индекс для кода ошибки
    int i = strlen(str) -  1;//индекс строки
    while (str[i--] != ' ');
        
    while (str[i] != ' ')
        buff[ptr--] = str[i--];
    return atof(buff);
}

long utime(char* stime) {
    char time_format[10] = "//::: ";//разделители строки времени

    char buff[7][6] = {""};//хранит разбиение строки stime
    int i = 0;//индекс строки stime
    int bp = 0; int wp = 0;//индексы buff
    int fp = 0; //индекс time_format
    while (stime[i]) {
        if (stime[i] == time_format[fp]) {
            ++bp;
            ++fp;
            wp = 0;
        } else
            buff[bp][wp++] = stime[i]; 
        ++i;
    }

    struct tm tdate = {
        .tm_mday = atof(buff[0]),
        .tm_mon = atof(buff[1]) - 1,
        .tm_year = atof(buff[2]) - 1900,
        .tm_hour = atof(buff[3]),
        .tm_min = atof(buff[4]),
        .tm_sec = atof(buff[5]),
        .tm_isdst = 0
    };

    time_t time = mktime(&tdate);
    return time;
}