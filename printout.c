#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char c;
    FILE *file = fopen(argv[3], "r");
    c = fgetc(file);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(file);
    }
    return 0;
}