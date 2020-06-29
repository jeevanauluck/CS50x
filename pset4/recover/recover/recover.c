#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2) //makes sure user inputted single command line argument
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) //if file cannot be opened for reading by any chance
    {
        printf("Error: cannot open for reading\n");
        return 1;
    }

    char filename[8];
    uint8_t buffer[512];
    FILE *img = NULL;
    int counter = 0; //counter for amt of imgs
    bool is_img = false; //determines if img or not

    while (fread(buffer, 512, 1, file) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (is_img == true)
            {
                fclose(img);
            }
            else
            {
                is_img = true;
            }
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "a");
            counter++;
        }

        if (is_img == true)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }
    fclose(file);
    fclose(img);
    return 0;
}