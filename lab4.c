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
void getheaderflag(unsigned char flags);
Frame *getframe();
Frame *initframe();
void print(char *p, int n);
void show();
int null(char *p, int n);
Frame *get(const char *q);
Frame *search(const char *q);
Header *header;
Frame *frames[100];
int fp = 0;
long pointer;
FILE *fin;
FILE *fout;

char *separate(char *str);
void parsearg(int argc, char *argv[]);

void copyf(char buff[], char origin[]);
void set(const char id[], const char val[], const char fpath[]);

void printHeader();
void printFrame(Frame *);
int main(int argc, char *argv[])
{
    //filepath
    fin = fopen("assets/a.mp3", "rb");
    fout = fopen("t.txt", "w");
    set("TIT2", "ABC", "text.txt");
}

void set(const char id[], const char val[], const char fpath[])
{
    //getheader();
    Frame *frame = get(id);
    if (frame == 0)
    {
        printf("tag not found");
        return;
    }
    //printf("%d\n", headersize(header->size));
    long size1 = framesize(frame->size);
    long size2 = strlen(val);
    long dsize = size2 - size1;
    long size = headersize(header->size) + dsize;
    freopen("assets/a.mp3", "r", fin);
    // fclose(fin);
    // fin = fopen("assets/a.mp3", "r");
    FILE *buff = fopen(fpath, "w");
    getheader();
    for (int i = 0; i < 4; ++i)
    {
        header->size[3 - i] = size % (long)pow(2, 7);
        size /= pow(2, 7);
    }
    printHeader();
    //printf("%d", headersize(header->size));
    while (pointer < headersize(header->size) && (frame = getframe())) {   
        fprintf(buff, "%s\n", frame->id);
    }
}

// long framesize(char id[]) {
//     Frame * frame = search(id);
//     return (frame) ? framesize(frame->size) : 0;
// }

void copyf(char origin[], char buff[])
{
    FILE *bf = fopen(buff, "w");
    int c;
    while ((c = fgetc(fin)) != EOF)
    {
        //if (c != 13)
        fputc(c, bf);
    }

    fclose(bf);
    //delete origin
    rename(buff, origin); //origin
}

void parsearg(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(fout, "not enough armuments");
    }
    else if (argc == 3)
    {
        char *val = separate(argv[2]);
        if (strcmp(argv[2], "--show") == 0)
            show();
        else if (strcmp(argv[2], "--get") == 0)
            get(val);
        else
            fprintf(fout, "1 arg, not supported");
    }
    else if (argc == 4)
    {
        //
    }
    else
    {
        fprintf(fout, "too many arguments");
    }
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

Frame *initframe()
{   
    printf("init\n");
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

Frame *getframe()
{
    Frame *frame = initframe();
    read_(frame->id, 4);
    if (null(frame->id, 4))
    {
        return 0;
    }
    read_(frame->size, 4);
    read_(frame->flags, 2);

    int size = framesize(frame->size);
    //fseek(fin, size, SEEK_CUR);
    //pointer += size;
    unsigned char *str = malloc((size + 1L) * sizeof(unsigned char));
    read_(str, size);
    frame->content = str;

    return frame;
}

int null(char *p, int n)
{
    int f = 1;
    while (n-- > 0)
        if (*p++ != 0)
            f = 0;
    return f;
}

void show()
{
    getheader();
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = getframe()))
    {
        print(frame->id, 4);
        fprintf(fout, "  %d  ", framesize(frame->size));

        print(frame->content, framesize(frame->size));
        fprintf(fout, "\n");
        clear(frame);
    }
}

void print(char *p, int n)
{
    while (n-- > 0)
    {
        if (*p)
            fprintf(fout, "%c", *p);
        *p++;
    }
}

Frame *get(const char *q)
{
    getheader();
    Frame *frame;
    while (pointer < headersize(header->size) && (frame = search(q)))
    {
        if (strcmp(frame->id, q) == 0)
            return frame;
        else
            clear(frame);
    }
    return 0;
}

Frame *search(const char q[])
{
    int tp;
    Frame *frame = initframe();

    read_(frame->id, 4);
    if (null(frame->id, 4))
    {
        return 0;
    }
    if (strcmp(q, frame->id) == 0)
        tp = GET;
    else
        tp = READ;

    read_(frame->size, 4);
    read_(frame->flags, 2);

    long size = framesize(frame->size);
    if (tp == READ)
    {
        pointer += size;
        fseek(fin, size, SEEK_CUR);
    }
    else if (tp == GET)
    {
        unsigned char *str = malloc((size + 1) * sizeof(unsigned char));
        read_(str, size);
        frame->content = str;
    }
    return frame;
}

void printHeader()
{
    int i;
    for (i = 0; i < 3; ++i)
        fprintf(fout, "%c ", header->id[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fout, "%c ", header->version[i]);

    fprintf(fout, "%c ", header->flags);

    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c ", header->size[i]);
}

void printFrame(Frame *frame)
{
    fprintf(fout, "\n\n\n---------------------------------\n\n\n");
    int i;
    for (i = 0; i < 4; ++i)
        fprintf(fout, "%c", frame->id[i]);

    for (i = 0; i < 4; ++i)
    {
        fprintf(fout, "%d", frame->size[i]);
    }
    for (i = 0; i < 2; ++i)
        fprintf(fout, "%d", frame->flags[i]);

    //printf("%d  \n ", framesize(frame->size));
    for (i = 0; i < framesize(frame->size); ++i)
        fprintf(fout, "%c", frame->content[i]);
}