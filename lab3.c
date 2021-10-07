#include <stdio.h>
#include <time.h>
#include<math.h>
#include<string.h>


int main () {
    FILE *file = fopen("t.log", "r");
    char str[100] = "";

    fgets(str, 100, file);
    
    printf(str);

    // char date[100] = "01/10/2001:10:50:02 -0400";
    // char format[100] = "//::: ";
    // char buff[100][100] = {""};
    // int i = 0; int bp = 0; int wp = 0; int fp = 0; 
    // while (date[i]) {
    //     if (date[i] == format[fp]) {
    //         bp += 1;
    //         fp += 1;
    //         wp = 0;
    //     } else {
    //         *(*(buff + bp) + wp) = date[i]; 
    //         wp += 1;
    //     }
    //     ++i;
    // }
    // struct tm tdate = {
    //     .tm_mday = atof(buff[0]),
    //     .tm_mon = atof(buff[1]) - 1,
    //     .tm_year = atof(buff[2]) - 1900,
    //     .tm_hour = atof(buff[3]),
    //     .tm_min = atof(buff[4]),
    //     .tm_sec = atof(buff[5]),
    //     .tm_isdst = 0
    // };


    return(0);
}