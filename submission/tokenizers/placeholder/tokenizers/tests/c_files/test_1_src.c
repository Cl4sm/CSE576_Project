#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Welcome to the fun house, enter your name: ");
    char name[32];
    scanf("%30s", name);

    for (int i = 0; i < 10; i++)
    {
        printf("Counting: %d\n", i);
    }
    printf("Done!\n");
    printf("Goodbye %s\n", name);
}