/** recovers JPEG images from a data file **/
       
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }

    // remember file name
    char *datafile = argv[1];

    // open memory card file 
    FILE *fileptr = fopen(datafile, "r");
    if (fileptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", datafile);
        return 2;
    }
    
    // create buffer to read bytes into
    unsigned char buffer[512];
        
    // create file for storing recovered images
    FILE *img;
    
    // create array for file names 
    char filename[8];
    
    // create bool for tracking
    int count = 0;
    
    // repeat until end of file
    while (fread(buffer, 1, 512, fileptr) == 512)
    {
        // find beginning of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (count == 0)
            {
                // open first JPEG
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                count++;
            }
            
            else
            {
                // close previous file
                fclose(img);
                
                // open new JPEG
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                count++;
            }
        }
        
        // write 512 bytes until new JPEG is found
        if (count > 0)
            fwrite(buffer, 1, 512, img);
    }
    
    // success, close any open files
    fclose(img);
    return 0;
}
