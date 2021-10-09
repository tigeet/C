#include<stdio.h>
#include <string.h>
#include <locale.h>
#define STRLEN 50//мб надо изменить
enum state {Space = 0, Word = 1};

int getbytes(FILE *file);
int getlines(FILE *file);
int getwords(FILE *file);


char option[10];
char fileName[STRLEN];


int main(int argc, char* argv[]) {

    if (argc != 3) {
        (argc < 3) ? printf("not enough arguments") : printf("too many arguments");
        return 0;
    }

    strcpy(option, argv[1]);
    strcpy(fileName, argv[2]);

    FILE *file = fopen(fileName, "r");

    if (file == 0) {
        printf("File not found");
        return 0;
    }

    if ((strcmp(option, "-l") == 0) || (strcmp(option, "--lines") == 0))
        printf("%d", getlines(file));
    else if ((strcmp(option, "-w") == 0) || (strcmp(option, "--words") == 0))
        printf("%d", getwords(file));
    else if ((strcmp(option, "-b") == 0) || (strcmp(option, "--bytes") == 0))
        printf("%d", getbytes(file));
    else printf("Command not supported");

    return 0; 
}





int getlines(FILE *file) {
    int lines = 0;
    char string[STRLEN];
    while (fgets(string, STRLEN,  file))
        lines++;
    return lines;
}


int getwords(FILE *file) {
    int c; 
    int words = 0;
    int state = 0;
    while ((c = fgetc(file)) != EOF) {
        if ((c == ' ') || (c == '\n')) 
            state = Space;
        else {
            if (state == Space)
                words++;
            state = Word;
        }
    }
    return words;
}


int getbytes(FILE *file) {
    int c = 0, bytes = 0;
    while ( (c = fgetc(file)) != EOF)  {
        if (c == '\n')
            bytes++;
        bytes++;
    }
    return bytes;
}