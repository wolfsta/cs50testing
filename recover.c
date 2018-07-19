#include <stdio.h>
#include <stdint.h>

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
    char *outfile = NULL;
    BYTE buffer[512];
    FILE *img = NULL;

    do  {
        // read file to buffer one byte at a time so can see when it reads less than 512bytws at the end
        fread(buffer, 1, 512, infile);

        // check if its a jpg file
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // it is a jpg so check if no jpg exists yet, write buffer and increment jpgcount
            if(jpgcount == 0)
            {
                img = fopen(outfile, "w");
                fwrite(buffer, 1, 512, img);
                jpgcount++;

            // else if it is a new jpg but not the first one, close the open file, create a new filename, open it for writing, write the buffer and increment jpgcount
            } else {
                fclose(img);
                sprintf (outfile, "%03i.jpg", jpgcount);
                img = fopen(outfile, "w");
                if (img == NULL)
                {
                    fprintf(stderr, "Cannot open file\n");
                    return 2;
                }
                fwrite(buffer, 1, 512, img);
                jpgcount++;
            }


        // its not a new jpg so else if its not the first jpg found then close the writing file, reopen it in append mode and and write the buffer
        } else if (jpgcount != 0)
        {
            fclose(img);
            img = fopen(outfile, "a");
            if (img == NULL)
                {
                    fprintf(stderr, "Cannot open file\n");
                    return 2;
                }
            fwrite(buffer, 1, 512, img);

        // else would be to do nothing as its not a new jpg and its not for an existing file, so can I leave this blank and it will go back to top of loop?
        }

    //while not end of file, if it is then close the infile and img files
    } while (!(feof(img)));

    fclose(infile);
    fclose(img);

}
