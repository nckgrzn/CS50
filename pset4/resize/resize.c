/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // declare multiplier as a variable
    int n = atoi(argv[1]);

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // declare resized structs
    BITMAPINFOHEADER newbi = bi;
    BITMAPFILEHEADER newbf = bf;
    
    // new width
    newbi.biWidth = bi.biWidth * n;
    
    // new height
    newbi.biHeight = bi.biHeight * n;
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newpadding = (4 - (newbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // new size of the image
    newbi.biSizeImage = ((sizeof(RGBTRIPLE) * newbi.biWidth) + newpadding) * abs(newbi.biHeight);
    
    // new size of the bitmap file
    newbf.bfSize = newbi.biSizeImage + sizeof(newbi) + sizeof(newbf);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // repeat n times 
        for (int m = 0; m < n; m++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile n times
                for (int k = 0; k < n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }    
            }
            
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int l = 0; l < newpadding; l++)
            {
                fputc(0x00, outptr);
            }
            
            // send infile cursor back to beginning of line
            if (m < n - 1)
            {
                int line = (bi.biWidth * sizeof(RGBTRIPLE)) + padding;
                fseek(inptr, - line, SEEK_CUR);
            }
        }
    }
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
