#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned int getSize(FILE *f){
    fseek(f, 0L, SEEK_END);
    unsigned int size= ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}

char *getFileName(char *path) {
    char *name = path;
    int len = strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '/' && path[i] != '\\')
        i--;
    
    name = (char *) malloc(len - i);
    memcpy(name, path + i + 1, len - 1 - i);
    name[len - i - 1] = '\0';
    return name;    
    // for (int i = len - 1; i >= 0; i--)
    //     if (path[i] == '/' || path[i] == '\\') {
    //         name = (char *) malloc(len - i);
    //         memcpy(name, path + i + 1, len - 1 - i);
    //         name[len - i - 1] = '\0';
    //         break;
    //     }
    // return name;
}

int Pack(char *argv[], int files, char *filename) {
    unsigned char files_number, byte;
    char *temp_filename, *text;
    FILE *output, *f;

    output = fopen(filename, "wb");
    files_number = files;
    fwrite(&files_number, 1, 1, output);

    for (int i = 0; i < files; i++) {
        unsigned int file_size;

        f = fopen(argv[i + 4], "rb");
        temp_filename = getFileName(argv[i + 4]);
        if (f == NULL)
            continue;

        file_size = getSize(f);
        byte = strlen(temp_filename);
        fwrite(&byte, 1, 1, output);
        fwrite(&file_size, sizeof (unsigned int), 1, output);

        for (int j = 0; j < byte; j++)
            fwrite(&temp_filename[j], 1, 1, output);

        text = (char *) malloc(file_size);
        fread(text, 1, file_size, f);
        fwrite(text, 1, file_size, output);
        fclose(f);
        free(text);
    }
    fclose(output);
    return 1;
}

int getArchive(char *filename) {
    FILE *input;
    char files;

    input = fopen(filename, "rb");

    if (input == NULL)
        return 0;

    fread(&files, 1, 1, input);

    for (int i = 0; i < files; i++) {
        FILE *f;
        char filename_size;
        int file_size;
        char *temp_filename, *text;

        fread(&filename_size, 1, 1, input);
        fread(&file_size, 4, 1, input);

        temp_filename = (char *) malloc(filename_size + 1);
        fread(temp_filename, 1, filename_size, input);
        temp_filename[filename_size] = '\0';

        f = fopen(temp_filename, "wb");
        if (f == NULL)
            continue;

        text = (char *) malloc(file_size);
        fread(text, 1, file_size, input);

        fwrite(text, 1, file_size, f);
        fclose(f);
        free(temp_filename);
        free(text);
    }
    return 1;
}

void List(char *filename) {
    FILE *input;
    char files;

    input = fopen(filename, "rb");

    if (input == NULL)
        return;

    fread(&files, 1, 1, input);

    for (int i = 0; i < files; i++) {
        char filename_size;
        int file_size;
        char *temp_filename, *text;

        fread(&filename_size, 1, 1, input);
        fread(&file_size, 4, 1, input);

        temp_filename = (char *) malloc(filename_size + 1);
        fread(temp_filename, 1, filename_size, input);
        temp_filename[filename_size] = '\0';

        fseek(input, file_size, SEEK_CUR);

        printf("Filename %i: %s\n", i, temp_filename);
        free(temp_filename);
    }
}


int main(int argc, char *argv[]) {
    FILE *f;
    unsigned char byte;
    char filename[100];
    unsigned int sz;
    int files = 0, operation = 0;


    for (int i = 1; i < argc; i += 2) {
        if (!strcmp("--file", argv[i]))
            strcpy(filename, argv[i + 1]);
        else if (!strcmp("--create", argv[i])) {
            files = argc - 4, operation = 1;
            break;
        }
        else if (!strcmp("--extract", argv[i])) {
            operation = 2;
            break;
        }
        else if (!strcmp("--list", argv[i])) {
            operation = 3;
            break;
        }
    }


    switch (operation) {
        case 1:
            Pack(argv, files, filename);
            break;
        case 2:
            getArchive(filename);
            break;
        case 3:
            List(filename);
            break;
    }

    return 0;
}
