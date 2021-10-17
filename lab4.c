#include <stdio.h>
#include <stdlib.h>
#include<string.h>



typedef struct Header{
    char id[3];
    char version[2];
    char flags;
    char size[4];
} Header;

typedef struct Frame {
    char *id;
    char *size;
    char *flags;
    char *content;
} Frame;


void clear(Frame * frame);
int getSize(char sizearr[]);
char read();

void getHeader();
void getHeaderFlag(char flags);
void printHeader();

//void getExtHeader(FILE* fin);
void printExtHeader();


void printFrame(Frame* frame);
Frame* getFrame();
Frame* initFrame();
void set(char* []);
void get(char value[]);
void show();

Header *header;
Frame *frames[100]; int fp = 0;
int pointer;
int TAG_SIZE;

FILE* fin;
FILE* fout;


/*
    set, get, printExtHeader написаны в неадекватном состоянии, не копировать
    остальное вроде работает
    все переписать
*/

int main(int argc, char* argv[]) {
    fin =  fopen("a.mp3", "rb");
    fout = fopen("t.txt", "w");


    getHeader(fin);
    // getHeaderFlag(fin, flags);
    show();

    // //printHeader(fout);
    // for (int i = 0; i < fp;++i) {
    //     printFrame(frames[i], fout);
    //     fprintf(fout, "\n");
    // }
    // printf("%d ", TAG_SIZE);
}


void getHeaderFlag(char flags) {
    //if (flags & 64) 
        //getExtHeader(fin);  
}

void show() {
    while (pointer < TAG_SIZE) {
        Frame* frame = getFrame();
        int p;
        for (p = 0; p < 4; ++p)
            fprintf(fout, "%c", frame->id[p]);

        fprintf(fout, " ");

        for (p = 1; p < getSize(frame->size); ++p)
            fprintf(fout, "%c", frame->content[p]);
        
        fprintf(fout, "\n");
        clear(frame);
    }
}
void get(char value[]) {
    while (pointer < TAG_SIZE) {
        Frame* frame = getFrame();
        int f = 1;
        for (int p = 0; f == 1 && p < 4; ++p) {
            f &=  (frame->id[p] == value[p]);
        }
        if (f) {
            printFrame(frame);
            break;
        }
        clear(frame);   
    }
}


void set(char *argv[]) {
    int pa = 0; int ps = 0;
    while (argv[1][pa++] != '=');

    char name[5];
    while (pa < strlen(argv[1]))
        name[ps++] = argv[1][pa++];
    name[4] = '\0';

    pa = 0; ps = 0;
    while (argv[2][pa++] != '=');

    char *value = malloc((strlen(argv[2]) - pa - 1) * sizeof(char));
    while (pa < strlen(argv[2]))
        value[ps++] = argv[2][pa++];

        //setValue
}



Frame* initFrame() {
    Frame* frame = malloc(sizeof(Frame));
    frame->id = malloc(4*sizeof(char));
    frame->size = malloc(4*sizeof(char));
    frame->flags = malloc(2*sizeof(char));
    frame->content = malloc(sizeof(char*));
    return frame;
}

void clear(Frame *frame) {
    free(frame->id);
    free(frame->size);
    free(frame->flags);
    free(frame->content);
    free(frame);
}

char read() {
    ++pointer;
    return fgetc(fin);
}

int getSize(char sizearr[]) {
    int size = 0;
    for (int i = 0; i < 4; ++i) {
        size += (sizearr[4 - i - 1] << 7*i);
    }
    return size;
}

void getHeader() {
    header = malloc(sizeof(Header));
    int i;
    for (i = 0; i < 3; ++i)
        header->id[i] = fgetc(fin);

    for (i = 0; i < 2; ++i)
        header->version[i] = fgetc(fin);


    header->flags = fgetc(fin);

    for (i = 0; i < 4; ++i)
        header->size[i] = fgetc(fin);

    TAG_SIZE = getSize(header->size);    
}

Frame * getFrame() {
    Frame* frame = initFrame();
    int i;
    for (i = 0; i < 4; ++i)
        frame->id[i] = read(fin);

    for (i = 0; i < 4; ++i)
        frame->size[i] = read(fin);

    for (i = 0; i < 2; ++i)
        frame->flags[i] = read(fin);

    int size = getSize(frame->size);
    char *string = malloc(size * sizeof(char));
    for (i = 0; i < size; ++i) {
        string[i] = read(fin);
    }    

    frame->content = string;

    return frame;
}

void printHeader() {
    int i;
    for (i = 0; i < 3; ++i)
        fprintf(fout, "%c", header->id[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fout, "%c", header->version[i]);


    fprintf(fout, "%c", header->flags);

    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c", header->size[i]);
}

void printFrame(Frame* frame) {
    int i;
    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c", frame->id[i]);

    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c", frame->size[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fout, "%c", frame->flags[i]);

    for (i = 0; i < getSize(frame->size); ++i)
        fprintf(fout, "%c", frame->content[i]);
}