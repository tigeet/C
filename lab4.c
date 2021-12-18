#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
enum type
{
    READ,
    GET
};

typedef struct
{
    unsigned char id[3];
    unsigned char version[2];
    unsigned char flags;
    unsigned char size[4];
} Header;

typedef struct
{
    unsigned char *id;
    unsigned char *size;
    unsigned char *flags;
    unsigned char *content;
} Frame;

void clear(Frame *frame);
long headersize(unsigned char sizearr[]);
long framesize(unsigned char sizearr[]);
void read_(unsigned char *str, long n);
void getheader();
Frame *getframe();
Frame *initframe();
void print(char *p, int n);
void show();
int null(char *p, int n);
void get(const char *q);
Frame *search(const char *q);
Header *header;
Frame *frames[100];
long pointer;
FILE *fin;

char *separate(char *str);
void parsearg(int argc, char *argv[]);

void copyf(char buff[], char origin[]);
void set(const char id[], char val[]);

void printHeader();
void printFrame(Frame *);

char filepath[100];
int main(int argc, char *argv[]) {
    if (argc > 2) {
        char * p = separate(argv[1]);
        if (strcmp(argv[1], "--filepath") == 0) {
            strcpy(filepath, p);
        } else {
            printf("no path");
            return 0;
        }
    }
    fin = fopen(filepath, "r+");
    parsearg(argc, argv);
    fclose(fin);
}

void parsearg(int argc, char *argv[]) {
    if (argc < 3)
    {
        fprintf(fin, "not enough armuments");
        return ;
    }
    else if (argc == 3)
    {
        char *val = separate(argv[2]);
        if (strcmp(argv[2], "--show") == 0)
            show();
        else if (strcmp(argv[2], "--get") == 0)
            get(val);
        else {
            fprintf(fin, "1 arg, not supported");
            return ;
        }
    }
    else if (argc == 4)
    {
        char * id = separate(argv[2]);
        char * value = separate(argv[3]);
        if (strcmp(argv[2], "--set") != 0) {
            printf("no tag selected");
            return ;
        }

        if (strcmp(argv[3], "--value") != 0) {
            printf("no value selected");
            return ;
        }

        set(id, value);
    }
    else
    {
        fprintf(fin, "too many arguments");
        return ;
    }
}


void set(const char id[], char val[]) {   
    Frame * frames[100];
    getheader(); 
    Frame * frame; Frame *q;
    int i = 0;
    int f = 0;
    while (pointer < headersize(header->size) && (frame = getframe())) {
        if (strcmp(frame->id, id) == 0) {
            q = frame;
            f = 1;
            long fsize = strlen(val) + 1;
            //frame->content = realloc(frame->content, fsize);
            for (int i = 0; i < 4; ++i) {
                frame->size[3 - i] = fsize % (long)pow(2, 8);
                fsize /= pow(2, 8);
            } 
            //printf("size: %d\n", fsize);
            char * buff = calloc(strlen(val) + 1, sizeof(char));
            buff[0] = 0;
            for (int i = 1; i < strlen(val) + 1; ++i) {
                buff[i] = val[i - 1];
            }
            //strncpy(buff + 1, val, fsize - 1);
            frame->content = buff;
        }
        frames[i++] = frame;
    }

    if (!f) {
        printf("tag not found");
        return ;
    }

    long size1 = framesize(q->size);
    long size2 = strlen(val) + 1;
    long dsize = size2 - size1;
    long size = headersize(header->size) + dsize;


    fseek(fin, 0, SEEK_SET);
    //fseek(fin, 0, SEEK_SET);
    for (int i = 0; i < 4; ++i) {
        header->size[3 - i] = size % (long)pow(2, 7);
        size /= pow(2, 7);
    }
    printHeader();

    for (int j = 0; j < i; ++j)
        printFrame(frames[j]);
    fprintf(fin, "\0");
 }


Frame *getframe() {
    Frame *frame = initframe();
    read_(frame->id, 4);
    if (null(frame->id, 4))
    {
        return 0;
    }
    read_(frame->size, 4);
    read_(frame->flags, 2);

    long size = framesize(frame->size);
    //fseek(fin, size, SEEK_CUR);
    //pointer += size;
    unsigned char *str = malloc((size + 1L) * sizeof(unsigned char));
    read_(str, size);
    frame->content = str;

    return frame;
}


void show() {
    getheader();
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = getframe())) {
        print(frame->id, 4);
        printf("  %d  ", framesize(frame->size));

        print(frame->content, framesize(frame->size));
        printf("\n");
        clear(frame);
    }
}

void print(char *p, int n)
{
    while (n-- > 0)
    {
        if (*p)
            printf("%c", *p);
        *p++;
    }
}

void get(const char *q) {
    getheader();
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = getframe())) {
        if (strcmp(frame->id, q) == 0) {
            print(frame->id, 4);
            printf("  %d  ", framesize(frame->size));

            print(frame->content, framesize(frame->size));
            printf("\n");
        }
        clear(frame);
    }
}

void getheader()
{
    header = malloc(sizeof(Header));
    int i;
    for (i = 0; i < 3; ++i)
        header->id[i] = fgetc(fin);

    for (i = 0; i < 2; ++i)
        header->version[i] = fgetc(fin);

    header->flags = fgetc(fin);

    for (i = 0; i < 4; ++i)
    {
        header->size[i] = fgetc(fin);
    }
}

void read_(unsigned char *str, long n)
{
    while (n-- > 0)
    {
        *str++ = fgetc(fin);
        ++pointer;
    }
    *str = '\0';
}

void printHeader()
{
    int i;
    for (i = 0; i < 3; ++i)
        fprintf(fin, "%c", header->id[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fin, "%c", header->version[i]);

    fprintf(fin, "%c", header->flags);

    for (i = 0; i < 4; ++i)
        fprintf(fin, "%c", header->size[i]);
}

void printFrame(Frame *frame)
{
    int i;
    for (i = 0; i < 4; ++i)
        fprintf(fin, "%c", frame->id[i]);

    for (i = 0; i < 4; ++i)
    {
        fprintf(fin, "%c", frame->size[i]);
    }
    for (i = 0; i < 2; ++i)
        fprintf(fin, "%c", frame->flags[i]);

    //printf("%d  \n ", framesize(frame->size));
    for (i = 0; i < framesize(frame->size); ++i)
        fprintf(fin, "%c", frame->content[i]);
}

int null(char *p, int n)
{
    int f = 1;
    while (n-- > 0)
        if (*p++ != 0)
            f = 0;
    return f;
}


char *separate(char *str)
{ //первая часть - str, вторая - separate
    while (*str != '=' && *str++ != '\0')
        ;

    if (*str == '\0')
        return str;
    else
        *str = '\0';
    return ++str;
}

Frame *initframe() {   
    Frame *frame = malloc(sizeof(Frame));
    frame->id = malloc(5 * sizeof(unsigned char));
    frame->size = malloc(5 * sizeof(unsigned char));
    frame->flags = malloc(3 * sizeof(unsigned char));
    frame->content = malloc(sizeof(unsigned char *));
    return frame;
}

void clear(Frame *frame)
{
    free(frame->id);
    free(frame->size);
    free(frame->flags);
    free(frame->content);
    free(frame);
}

long headersize(unsigned char sizearr[])
{
    long size = 0;
    for (int i = 0; i < 4; ++i)
    {
        size += ((sizearr[4 - i - 1]) << 7 * i);
    }
    return size;
}

long framesize(unsigned char arr[])
{
    long size = 0;
    for (int i = 0; i < 4; ++i)
    {
        size += ((arr[4 - i - 1]) << 8 * i);
    }
    return size;
}