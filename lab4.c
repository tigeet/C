#include <stdio.h>
#include <stdlib.h>
#include<string.h>
enum type {READ, GET};


typedef struct Header{
     unsigned char id[3];
     unsigned char version[2];
     unsigned char flags;
     unsigned char size[4];
} Header;

typedef struct Frame {
     unsigned char *id;
     unsigned char *size;
     unsigned char *flags;
     unsigned char *content;
} Frame;


void clear(Frame * frame);
long headersize( unsigned char sizearr[]);
long framesize( unsigned char sizearr[]);
void read_(unsigned char* str, long n);
void getheader();
void getheaderflag(unsigned char flags);
Frame* getframe();
Frame* initframe();
void print(char *p, int n);
void show();
int null(char* p, int n);
void get(char *q);
Frame* search(char*q);

Header *header;
Frame *frames[100]; int fp = 0;
long pointer;

FILE* fin;
FILE* fout;

char* separate(char* str);
void parsearg(int argc, char* argv[]);

void copyf(char []);
int main(int argc, char* argv[]) {
    //filepath
    fin =  fopen("assets/a.mp3", "rb");
    fout = fopen("t.txt", "w");
    //show();
    copyf("r.mp3");
    // fclose(fin);
    // fclose(fout);
    // printf("%d", rename("t.txt", "tb.txt"));
}

void copyf(char origin[]) {
    FILE *buff = fopen("buff.txt", "w");
    int c;
    while ((c = fgetc(fin)) != EOF) {
        //if (c != 13)
            fputc(c, buff);
    }

    fclose(buff);
    rename("buff.txt", "text.txt");
}



void parsearg(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(fout, "not enough armuments");
    } else if (argc == 3) {
        char* val = separate(argv[2]);
        if (strcmp(argv[2], "--show") == 0)
            show();
        else if (strcmp(argv[2], "--get") == 0)
            get(val);
        else   
            fprintf(fout, "1 arg, not supported");
    } else if (argc == 4) {
        //
    } else {
        fprintf(fout, "too many arguments");
    }
}


char* separate(char* str) {//первая часть - str, вторая - separate
    while (*str != '=' && *str++ != '\0');

    if (*str == '\0')
        return str;
    else 
        *str = '\0';
    return ++str;
}

Frame* initframe() {
    Frame* frame = malloc(sizeof(Frame));
    frame->id = malloc(5*sizeof( unsigned char));
    frame->size = malloc(5*sizeof( unsigned char));
    frame->flags = malloc(3*sizeof( unsigned char));
    frame->content = malloc(sizeof( unsigned char*));
    return frame;
}

void clear(Frame *frame) {
    free(frame->id);
    free(frame->size);
    free(frame->flags);
    free(frame->content);
    free(frame);
}

long headersize(unsigned char sizearr[]) {
    long size = 0;
    for (int i = 0; i < 4; ++i) {
        size += ((sizearr[4 - i - 1]) << 7*i);
    }
    return size;
}

long framesize(unsigned char arr[]) {
    long size = 0;
    for (int i = 0; i < 4; ++i) {
        size += ((arr[4 - i - 1]) << 8*i);
    }
    return size;
}

void getheader() {
    header = malloc(sizeof(Header));
    int i;
    for (i = 0; i < 3; ++i)
        header->id[i] = fgetc(fin);

    for (i = 0; i < 2; ++i)
        header->version[i] = fgetc(fin);

    header->flags = fgetc(fin);

    for (i = 0; i < 4; ++i) {
        header->size[i] = fgetc(fin);
    }
}

void read_(unsigned char* str, long n){
    while (n-- > 0) {
        *str++ = fgetc(fin);
        ++pointer;
    }
    *str = '\0';
}

Frame* getframe() {
    printf("%d\n", pointer);
    Frame* frame = initframe();
    read_(frame->id, 4);
    if (null(frame->id, 4)) {
        return 0;
    }
    read_(frame->size, 4);
    read_(frame->flags, 2);

    int size = framesize(frame->size);
    //fseek(fin, size, SEEK_CUR);
    //pointer += size;
    unsigned char* str = malloc((size + 1L) * sizeof(unsigned char));
    read_(str, size);
    frame->content = str;

    return frame;
}

int null(char* p, int n) {
    int f = 1;
    while (n-- > 0)
        if (*p++ != 0)
            f = 0;
    return f;
 }

void show() {
    getheader();
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = getframe())) {
        print(frame->id, 4);
        fprintf(fout, "  %d  ", framesize(frame->size));
        
        print(frame->content, framesize(frame->size));
        fprintf(fout, "\n");
        clear(frame);
    }
}

void print(char *p, int n) {
    while (n-- > 0) {
        if (*p) 
            fprintf(fout, "%c", *p);
            *p++;
    }
}

void get(char *q) {
    getheader();   
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = search(q))) {
        if (strcmp(frame->id, q) == 0) {
            print(frame->id, 4);
            fprintf(fout, "    ");
            print(frame->content, framesize(frame->size));
            return ;
        }
    }
    fprintf(fout, "tag not found");
}

Frame* search(char* q) {
    int tp;
    Frame* frame = initframe();
    
    read_(frame->id, 4);
    if (null(frame->id, 4)) {
        return 0;
    }
    if (strcmp(q, frame->id) == 0)
        tp = GET;
    else 
        tp = READ;

    read_(frame->size, 4);
    read_(frame->flags, 2);

    long size = framesize(frame->size);
    if (tp == READ) {
        pointer += size;
        fseek(fin, size, SEEK_CUR);
    } else if (tp == GET) {
        unsigned char* str = malloc((size + 1) * sizeof(unsigned char));
        read_(str, size);
        frame->content = str;
    }
    return frame;
}