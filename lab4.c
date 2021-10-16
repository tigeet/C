#include <stdio.h>
#include <stdlib.h>
#include<string.h>



typedef struct Header{
    char id[3];
    char version[2];
    char flags[1];
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
char read(FILE* fin);

void getHeader(FILE*fin);
void printHeader(FILE* fout);

void printFrame(Frame* frame, FILE* fout);
Frame* getFrame(FILE* fin, FILE*fout);
Frame* initFrame();


Header *header;
Frame *frames[100]; int fp = 0;
int pointer;
int TAG_SIZE;


int main() {
    FILE *fin =  fopen("file.mp3", "rb");
    FILE *fout = fopen("t.txt", "w");
    getHeader(fin);
    
    while (pointer < TAG_SIZE) {
        frames[fp++] = getFrame(fin, fout);
    }

    printHeader(fout);
    for (int i = 0; i < fp;++i) {
        printFrame(frames[i], fout);
    }
    printf("%d ", TAG_SIZE);
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

char read(FILE* fin) {
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


void getHeader(FILE*fin) {
    header = malloc(sizeof(Header));
    int i;
    for (i = 0; i < 3; ++i)
        header->id[i] = fgetc(fin);

    for (i = 0; i < 2; ++i)
        header->version[i] = fgetc(fin);


    header->flags[0] = fgetc(fin);

    for (i = 0; i < 4; ++i)
        header->size[i] = fgetc(fin);

    TAG_SIZE = getSize(header->size);    
}


Frame * getFrame(FILE* fin, FILE*fout) {
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


void printHeader(FILE *fout) {
    int i;
    for (i = 0; i < 3; ++i)
        fprintf(fout, "%c", header->id[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fout, "%c", header->version[i]);


    fprintf(fout, "%c", header->flags[0]);

    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c", header->size[i]);
}


void printFrame(Frame* frame, FILE*fout) {
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