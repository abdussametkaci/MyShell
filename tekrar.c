#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// print arguments
void printArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    int i = 0;
    int repeat = atoi(argv[argc - 1]); // the last argument

    while (i < repeat)
    {
        printArgs(argc, argv);
        printf("\n");
        i++;
    }

    return 0;
}

void printArgs(int argc, char *argv[])
{
    // argv[0] is program name
    int i = 1; 
    // except the last argument, all arguments are printed
    while (i < argc - 1)
    {
        printf("%s", argv[i]);
        printf(" ");
        i++;
    }
}