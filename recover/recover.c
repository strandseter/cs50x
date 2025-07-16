#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Failed to open card!\n");
        return 1;
    }

    uint8_t buffer[512];

    int file_count = 0;

    FILE *file = NULL;

    while (fread(buffer, 1, 512, card) == 512)
    {
        bool is_start_of_jpeg = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                                (buffer[3] & 0xf0) == 0xe0;

        if (is_start_of_jpeg)
        {
            if (file != NULL)
            {
                fclose(file);
            }

            char filename[8];
            sprintf(filename, "%03i.jpg", file_count);

            file = fopen(filename, "w");

            file_count++;
        }

        if (file != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), 512, file);
        }
    }

    printf("Recovered %i images!\n", file_count);

    fclose(file);
    fclose(card);
}
