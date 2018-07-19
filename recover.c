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

    int jpgcount = 0;
    char *outfile = NULL;
    BYTE buffer[512];
    FILE *img = NULL;

    do  {
        fread(buffer, 1, 512, infile);

        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if(jpgcount == 0)
            {
                img = fopen(outfile, "w");
                fwrite(buffer, 1, 512, img);
                jpgcount++;

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
        } else
        {
            printf ("hello");
        }


    } while (!(feof(img)));

    fclose(infile);
    fclose(img);

}
