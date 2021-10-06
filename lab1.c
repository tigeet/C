#include<stdio.h>
#include <string.h>
#include <locale.h>
#define STRLEN 50


int getbytes(FILE *f);
int getlines(FILE *f);
int getwords(FILE *f);



char option[10];
char fileName[STRLEN];

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Rus");
    strcpy(option, argv[1]);
    strcpy(fileName, argv[2]);

    FILE *f = fopen(fileName, "r");

    if (f == 0) {
        printf("File not found");
        return 0;
    }
    if ((strcmp(option, "-l") == 0) || (strcmp(option, "--lines") == 0))
        printf("%d", getlines(f));
    else if ((strcmp(option, "-w") == 0) || (strcmp(option, "--words") == 0))
        printf("%d", getwords(f));
    else if ((strcmp(option, "-b") == 0) || (strcmp(option, "--bytes") == 0))
        printf("%d", getbytes(f));
    else printf("Command not supported");
    return 0; 
}





int getlines(FILE *f) {
    int lines = 0;
    char str[STRLEN];
    while (fgets(str,STRLEN,  f))
        lines++;
    return lines;
}

int getwords(FILE *f) {
    int c; int words = 0; int state = 0;
    while ((c = fgetc(f)) != EOF) {
        if ((c == ' ') || (c == '\n')) 
            state = 0;
        else {
            if (state == 0)
                words++;
            state = 1;
        }
    }
    return words;
}



int getbytes(FILE *f) {
    int c = 0, size = 0;
    while ( (c = fgetc(f)) != EOF) 
        size++;
    return size;
}