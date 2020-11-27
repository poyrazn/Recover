#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;
#define SIZE 512


int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Open input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    // allocate memory for buffer to write onto
    // BYTE buffer[SIZE];
    BYTE *buffer = malloc(SIZE * sizeof(BYTE));

    // keep track of number of found images for file naming
    int imgcount = 0;

    // flag to check the beginning of a jpeg file
    bool jpeg = false;

    FILE *image = NULL;

    // read from file, write into buffer until no blocks left
    while (fread(buffer, SIZE, 1, file) == 1)
    {

        //check if read bytes match the jpeg header signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            // check if there was another jpeg file found before this current byte stream
            if (jpeg)
            {
                // close previously written image file
                fclose(image);
            }
            else
            {
                // mark as jpeg found
                jpeg = true;

            }

            // allocate memory for filename
            string filename = malloc(7 * sizeof(char));
            // assign imagenumber to filename
            sprintf(filename, "%03d.jpg", imgcount);
            // open new image file with filename
            image = fopen(filename, "a");
            imgcount++;
        }
        if (jpeg)
        {
            // if jpeg is found, write buffer content to the current image file
            fwrite(buffer, SIZE, 1, image);
        }
    }

    // close the input file
    fclose(file);
    // close the image file
    fclose(image);
    return 0;
}
