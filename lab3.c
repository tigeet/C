#include <stdio.h>
#include <time.h>
#include<math.h>
#include<string.h>

#define size 400//размер строки запроса
long timestamp[100]; long tptr = 0;//массив timestamps для поиска окна
long fcnt = 0; //кол-во неудачных запросов
int window = 1;
long utime(char *stime);//преобразует строку времени в unix timestamp
void observeFile(FILE*);
int findwindow(int);
//2 - время
//4 - запрос
//6 - код ответа
//7 - биты

int main () {
    FILE* file = fopen("t.txt", "r");
    observeFile(file);



    //printf("%d" , findwindow(62));
    return(0);
}

int findwindow(int n) {
    int mc = 0;
    for (int i = 0; i < tptr; ++i) {
        int c = 1;
        for (int j = i + 1; j < tptr && (timestamp[j] -  timestamp[i] <= n); ++j)
            ++c;
        mc = (c > mc) ? c: mc;
    }
    return mc;
}


void observeFile(FILE *file) {
    char request_format[15] = " []\"\"  \0";
    int timestamp_buff = 0;
    char string_buff[size] = "";

    while (fgets(string_buff, size, file)) {
        char request_buff[8][100] = {""};
        int i = 0; int fp = 0; int wp = 0; int bp = 0;
        while (string_buff[i] != '\n') {
            if (string_buff[i] == request_format[fp]) {
                ++fp;
                ++bp;
                wp = 0;
            } else {
                request_buff[bp][wp] = string_buff[i];
                ++wp;
            }
            ++i;
        }

        if (tptr == 0) {
            timestamp_buff = utime(request_buff[2]);
        }
        timestamp[tptr++] = utime(request_buff[2]) - timestamp_buff;

        if ((atoi(request_buff[7]) / 100) == 5){ //переделать
            printf("%s \n", string_buff);
            fcnt++;  
        }
    }
}






long utime(char* stime) {
    char time_format[10] = "//::: ";//разделители строки времени

    char buff[100][100] = {""};//переделать
    int i = 0; int bp = 0; int wp = 0; int fp = 0; 
    while (stime[i]) {
        if (stime[i] == time_format[fp]) {
            bp += 1;
            fp += 1;
            wp = 0;
        } else {
            *(*(buff + bp) + wp) = stime[i]; 
            wp += 1;
        }
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