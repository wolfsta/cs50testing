#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // if the user doesnt specify the memory card file
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover imagefile\n");
        return 1;
    }

    //open file for reading
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return 2;
    }

    //initialize needed variables and file for writing
    int jpgcount = 0;
    char *outfile = malloc((sizeof(char)) * 7);
    BYTE buffer[512];
    FILE *img = NULL;

    // read file to buffer one byte at a time so can see when it reads less than 512bytes at the end
    while((fread(buffer, 1, 512, infile)) == 512)  {


        // check if its a jpg file
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // it is a jpg so check if no jpg exists yet, write buffer and increment jpgcount
            if(jpgcount == 0)
            {
                jpgcount++;
                sprintf (outfile, "%03i.jpg", (jpgcount - 1));
                img = fopen(outfile, "w");
                if (img == NULL)
                {
                    fprintf(stderr, "Cannot open file\n");
                    return 3;
                }
                fwrite(buffer, 1, 512, img);


            // else if it is a new jpg but not the first one, close the open file, create a new filename, open it for writing, write the buffer and increment jpgcount
            } else {
                jpgcount++;
                fclose(img);
                sprintf (outfile, "%03i.jpg", (jpgcount - 1));
                img = fopen(outfile, "w");
                if (img == NULL)
                {
                    fprintf(stderr, "Cannot open file\n");
                    return 4;
                }
                fwrite(buffer, 1, 512, img);
            }


        // its not a new jpg so else if its not the first jpg found then close the writing file, reopen it in append mode and and write the buffer
        } else if (jpgcount >= 1)
        {
            fclose(img);
            img = fopen(outfile, "a");
            if (img == NULL)
                {
                    fprintf(stderr, "Cannot open file\n");
                    return 5;
                }

            fwrite(buffer, 1, 512, img);


        // else would be to do nothing as its not a new jpg and its not for an existing file, so can I leave this blank and it will go back to top of loop?
        } else {

        }

    }
    fclose(img);
    fclose(infile);
    free(outfile);

}
