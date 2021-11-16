#include <stdio.h>
#include <stdlib.h>
#include<string.h>



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
int getSize( unsigned char sizearr[]);
int sz( unsigned char sizearr[]);
 unsigned char read_();

void getHeader();
void getHeaderFlag( unsigned char flags);
//void printHeader();

//void getExtHeader(FILE* fin);
//void printExtHeader();


//void printFrame(Frame* frame);
Frame* getFrame();
Frame* initFrame();
void set( unsigned char* []);
void get( unsigned char value[]);
void show(int n);

Header *header;
Frame *frames[100]; int fp = 0;
int pointer;
int TAG_SIZE;

FILE* fin;
FILE* fout;


//размер фрейма - просто без игнора битов
//хедера - с игнором 8х
//сделать fseek для паддинга

/*
    set, get, printExtHeader написаны в неадекватном состоянии, не копировать
    остальное вроде работает
    все переписать
*/  

int main(int argc,  unsigned char* argv[]) {
    fin =  fopen("c.mp3", "rb");
    fout = fopen("t.txt", "w");

// int i = 0;
//     while (i++ < 10000) {
//         //forintf(fout, "%d ", fgetc(fin));
//     }
    getHeader(fin);
    printf("%d \n", getSize(header->size));


    //unsigned char arr[4] = {0, 0, 10, 141};
    //printf("\n%d\n", sz(arr));
    //printHeader();
    int i = 0;
    while (pointer < getSize(header->size)) {
    //while (i < 10) {
        Frame* frame = getFrame();
        //printFrame(frame);
        //printf("        %d\n", getSize(frame->size));
        clear(frame);
        ++i;
    }

    //printf("size:   %d %d", pointer, getSize(header->size));
}





Frame* initFrame() {
    Frame* frame = malloc(sizeof(Frame));
    frame->id = malloc(4*sizeof( unsigned char));
    frame->size = malloc(4*sizeof( unsigned char));
    frame->flags = malloc(2*sizeof( unsigned char));
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

 unsigned char read_() {
    unsigned char c = fgetc(fin);
    if (pointer < 15000) {
        if (c == '\n' || c == 13)
            fprintf(fout, "%d  \n", c);
        else 
            fprintf(fout, "%d   %c\n", c, c);
    }
    ++pointer;
    return c;
}

int getSize( unsigned char sizearr[]) {
    int size = 0;
    for (int i = 0; i < 4; ++i) {
        size += ((sizearr[4 - i - 1]) << 7*i);
    }
    return size;
}

int sz(unsigned char arr[]) {
    int size = 0;
    for (int i = 0; i < 4; ++i) {
        size += ((arr[4 - i - 1]) << 8*i);
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

    for (i = 0; i < 4; ++i) {
        header->size[i] = fgetc(fin);
    }

    TAG_SIZE = getSize(header->size);    
}

Frame * getFrame() {
    Frame* frame = initFrame();
    int i;
    for (i = 0; i < 4; ++i)
        frame->id[i] = read_();
        
    for (i = 0; i < 4; ++i) {
        frame->size[i] = read_();
        printf("%d ", frame->size[i]);
    }
    for (i = 0; i < 2; ++i)
        frame->flags[i] = read_();

    int size = sz(frame->size);
    printf("%d\n", size);
    unsigned char *string = malloc(size * sizeof( unsigned char));
    for (i = 0; i < size; ++i) {
        string[i] = read_();
    }
    frame->content = string;

    return frame;
}

// void printHeader() {
//     int i;
//     for (i = 0; i < 3; ++i)
//         //forintf(fout, "%c ", header->id[i]);

//     for (i = 0; i < 2; ++i)
//         //forintf(fout, "%c ", header->version[i]);


//     //forintf(fout, "%c ", header->flags);

//     for (i = 0; i < 4; ++i)
//         //forintf(fout, "%c ", header->size[i]);
// }

// void printFrame(Frame* frame) {
//     //forintf(fout, "\n\n\n---------------------------------\n\n\n");
//     int i;
//     for (i = 0; i < 4; ++i)
//         //forintf(fout, "%c", frame->id[i]);

//     for (i = 0; i < 4; ++i) {
//         //forintf(fout, "%d", frame->size[i]);
//     }
//     for (i = 0; i < 2; ++i)
//         //forintf(fout, "%d", frame->flags[i]);

//     printf("%d  \n ", getSize(frame->size));
//     for (i = 0; i < getSize(frame->size); ++i)
//         //forintf(fout, "%c", frame->content[i]);
// }

