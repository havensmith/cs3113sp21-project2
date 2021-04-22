#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *file;
    file = fopen(argv[3], "r");
    char c = fgetc(file);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(file);
    }
    //printBlocks();
    return 0;
}