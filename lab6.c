#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *getFileName(char *path) {
    char *name;
    int len = strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '/' && path[i] != '\\')
        i--;

    name = (char *) malloc(len - i);
    name[len - i - 1] = '\0';
    memcpy(name, path + i + 1, len - 1 - i);
    return name;    
}

int getSize(FILE *f){
    fseek(f, 0L, SEEK_END);
     int size= ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}


void getArchive(char *name) {
    FILE *fin, *fout;
    char n, nsize;
    int fsize;
    char *tname, *text;

    fin = fopen(name, "rb");
    fread(&n, 1, 1, fin);

    for (int i = 0; i < n; i++) {
        fread(&nsize, 1, 1, fin);
        fread(&fsize, 4, 1, fin);

        tname = (char *) malloc(nsize + 1);
        tname[nsize] = '\0';
        fread(tname, 1, nsize, fin);

        fout = fopen(tname, "wb");

        text = (char *) malloc(fsize);
        fread(text, 1, fsize, fin);

        fwrite(text, 1, fsize, fout);
        fclose(fout);
        free(tname);
        free(text);
    }
}

void List(char *name) {
    FILE *fin;
    char n, nsize;
    int fsize;
    char *tname, *text;

    fin = fopen(name, "rb");
    fread(&n, 1, 1, fin);

    for (int i = 0; i < n; i++) {
        fread(&nsize, 1, 1, fin);
        fread(&fsize, 4, 1, fin);

        tname = (char *) malloc(nsize + 1);
        tname[nsize] = '\0';
        fread(tname, 1, nsize, fin);

        fseek(fin, fsize, SEEK_CUR);
        printf("%s ", tname);
        free(tname);
    }
}

void Pack(int argc, char *argv[], char *name) {
    char n, nsize;
    int fsize;
    char *tname, *text;
    FILE *fout, *fin;

    fout = fopen(name, "wb");
    n = argc - 4;
    fputc(n, fout);

    for (int i = 4; i < argc; i++) {
        fin = fopen(argv[i], "rb");
        tname = getFileName(argv[i]);

        nsize = strlen(tname);
        fsize = getSize(fin);
        fputc(nsize, fout);
        fwrite(&fsize, sizeof ( int), 1, fout);
        fwrite(tname, sizeof(char), nsize, fout);

        text = (char *) malloc(fsize);
        fread(text, 1, fsize, fin);
        fwrite(text, 1, fsize, fout);
        fclose(fin);
        free(text);
    }
    fclose(fout);
}



int main(int argc, char *argv[]) {
    char filename[100];

    strcpy(filename, argv[2]);

    if (!strcmp("--create", argv[3])) {
        Pack(argc, argv, filename);
    } else if (!strcmp("--extract", argv[3])) {
        getArchive(filename);
    } else if (!strcmp("--list", argv[3])) {
        List(filename);
    }
    return 0;
}
