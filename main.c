#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const char *rshaped = "┴";
const char *cross = "┼";
const char *shaped = "┬";
const char *line = "│";
const char *dash = "─";

void printsymbol(uint8_t column, const char* symbol);
void printfile(char *name, char *argv[]);

int main(int argc, char *argv[])
{
    if (argc == 1) {
        perror("Not args!");
        exit(1);
    } else if (argc == 2) {
        printfile(argv[1], argv);
    } else {
        for (uint8_t i = 1; i < argc; i++)
            printfile(argv[i], argv);
    }
    return 0;
}

void printsymbol(uint8_t column, const char* symbol)
{
    for (uint8_t i = 0; i < column; i++)
        if (i == 5)
            printf("%s", symbol);
        else
            printf("%s", dash);
}

void printfile(char *name, char *argv[])
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    uint8_t column = w.ws_col, newline = 8;
    uint64_t number = 1;
    FILE *fp;

    char *prog = argv[0];
    char buffer;

    if ((fp = fopen(name, "r")) == NULL) {
            fprintf(stderr, "%s: can't open %s\n", prog, name);
            exit(1);
    }

    printsymbol(column, shaped);
    printf("\n%8s\tFile: %s\n", line, name);
    printsymbol(column, cross);

    while (1) {
        buffer = fgetc(fp);

        if (buffer == EOF)
            if (feof(fp) != 0) {  
                putchar('\n');
                break;
            } else {
                perror("\nError read file!\n");
                break;
            }

        if (newline == column) {
            printf("\n%8s ", line);
            newline = 8;
        }

        if (number == 1)
            printf("%4ld%4s\t", number++, line);
        if (buffer == '\n') {
            printf("%c%4ld%4s\t", buffer, number++, line);
            newline = 8;
        } else {
            putchar(buffer);
            newline++;
        }
    }

    printsymbol(column, rshaped);
    fclose(fp);
}
