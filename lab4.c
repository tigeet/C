#include <stdio.h>
#include <stdlib.h>

int BYTE_SIZE  = 8;
int COLUMNS = 10;


int HEADER_SIZE = 10; 
int EXT_HEADER_SIZE;
int FRAME_HEADER_SIZE = 10;
int PADDING_SIZE;
int FOOTER_SIZE = 10;


typedef struct Header{
    char id[3];
    char version[2];
    char flags[1];
    char size[4];
} Header;

typedef struct Frame {
    char id[4];
    char size[4];
    char flags[2];
    char *content;
} Frame;


Header *header;
Frame *frames[100]; int fp = 0;
int pointer;


char read(FILE* fin) {
    ++pointer;
    return fgetc(fin);
}

void toBin(int c, int d, FILE* fout) {
    if (d < BYTE_SIZE) {
        toBin(c / 2, d + 1, fout);
        fprintf(fout, "%d", c % 2);
    }
}

void print(FILE* fin, FILE*fout) {
    int c, p = 0;
    while ((c = fgetc(fin)) != EOF) {
        if (p % COLUMNS == 0)
            fprintf(fout, "\n");
        //toBin(c, 0, fout);
        //fprintf(fout, " ");
        fprintf(fout, "%3d ", c);
        p++;
    }
}

int getSize(const char sizearr[], int fieldSize) {
    int size = 0;
    for (int i = 0; i < fieldSize; ++i) {
        size += (sizearr[fieldSize - i - 1] << 7*i);
    }
    return size;
}


void getHeader(FILE*fin) {
    char *headerBuff = malloc(10 * sizeof(char));
    for (int i = 0; i < HEADER_SIZE; ++i) {
        headerBuff[i] = fgetc(fin);
    }
    header = (Header*)headerBuff;
}


void getFrame(FILE* fin, FILE*fout) {
    char *frameHeaderStr = malloc(10 * sizeof(char));
    Frame* frameHeaderBuff = malloc(sizeof(Frame));

    for (int i = 0; i < FRAME_HEADER_SIZE; ++i) {
        frameHeaderStr[i] = read(fin);
    }
    frameHeaderBuff = (Frame*)frameHeaderStr;

    int size = getSize(frameHeaderBuff->size, 4);
    char *frameContentStr = malloc(size* sizeof(char));
    for (int i = 0; i < size; ++i) {
       frameContentStr[i] = read(fin);
    }  
    frameHeaderBuff->content = frameContentStr;

    frames[fp++] = frameHeaderBuff;
    return;
}

int main() {
    FILE *fin =  fopen("file.mp3", "rb");
    FILE *fout = fopen("t.txt", "w");
    getHeader(fin);


    while (pointer < getSize(header->size, 4)) {
        getFrame(fin, fout);
    }


    for (int i = 0; i < fp; ++i) {
        fprintf(fout, "%s ", frames[i]->id);

        for (int j = 1; j < getSize(frames[i]->size, 4); ++j) {
            fprintf(fout, "%c", frames[i]->content[j]);
        }
        fprintf(fout, "\n");
    }
    // print(fin, fout);
    
}
