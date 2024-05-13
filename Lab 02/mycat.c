#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char ch;

    if (argc != 2) {
        printf("Usage: mycat filename\n");
        return 1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Cannot open file \n");
        return 0;
    }

    ch = fgetc(file);
    while (ch != EOF) {
        putchar(ch);
        ch = fgetc(file);
    }

    fclose(file);
    return 0;
}
