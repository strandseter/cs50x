#include <cs50.h>
#include <stdio.h>

int main()
{
    int height = get_int("Height: ");

    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height + i + 1; j++)
        {
            if (j + 1 < height - i)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }

            if (j == height - 1)
            {
                for (int k = 0; k < 2; k++)
                {
                    printf(" ");
                }
            }
        }

        printf("\n");
    }
}
