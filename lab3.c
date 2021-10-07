#include <stdio.h>
#include <time.h>
#include<math.h>
#include<string.h>

#define size 400//размер строки запроса
//разделители строки запроса
//char ex[100] = "03/Jul/1995:10:50:02 -0400";
//tornado.umd.edu - - [28/Jul/1995:13:32:25 -0400] "GET /shuttle/missions/sts-74/sts-74-patch-small.gif HTTP/1.0" 200 5494
long timestamp[100]; long tptr = 0;//массив timestamps для поиска окна
long fcnt = 0; //кол-во неудачных запросов
int window = 1;
long utime(char *stime);//преобразует строку времени в unix timestamp
void observeFile(FILE*);
int findwindow(int);
//2 - время
//4 - запрос
//6 - код ответа

int main () {
    FILE* file = fopen("t.log", "r");
    observeFile(file);

    // for (int i = 0; i < tptr; ++i) 
    //     printf("%d \n", timestamp[i]);

    printf("%d" , findwindow(1));
    return(0);
}

int findwindow(int n) {
    int c = 0;
    int mc = 0;
    for (int i = 0; i < tptr; ++i) {
        for (int j = i + 1; j < tptr; ++j) {
            if (timestamp[j] -  timestamp[i] <= n) {
                ++c;
            } else {
                mc = (c > mc) ? c : mc;
                c = 0;
                break;
            };
        }
    }
    return mc;
}


void observeFile(FILE *file) {
    char request_format[15] = " []\"\"  \0";

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
                *(*(request_buff + bp) + wp) = string_buff[i];
                ++wp;
            }
            
            ++i;
        }
        timestamp[tptr++] = utime(request_buff[2]);

        if ((int)(atof(request_buff[6]) / 500) == 1) {
            fcnt++;
            //printf("%s \n", string_buff);        
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