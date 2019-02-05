#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


int main(int argc, char * argv [])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 2;
    }
    FILE * readFile = fopen(argv[1], "r");
    if(readFile == NULL)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 2;
    }
    FILE * writeFile = fopen(argv[2], "w");
    if(writeFile == NULL)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        fclose(readFile);
        return 2;
    }
    BITMAPFILEHEADER BF;
    BITMAPINFOHEADER BI;
    fread(&BF, sizeof(BITMAPFILEHEADER), 1, readFile);
    fread(&BI, sizeof(BITMAPINFOHEADER), 1, readFile);
    if((BF.bfType != 0x4d42) || (BF.bfOffBits != 54) || (BI.biSize != 40) || (BI.biBitCount != 24)
        || (BI.biCompression != 0))
    {
        fclose(readFile);
        fclose(writeFile);
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 2;
    }
    fwrite(&BF, sizeof(BITMAPFILEHEADER), 1, writeFile);
    fwrite(&BI, sizeof(BITMAPINFOHEADER), 1, writeFile);
    int padding = (4 - (BI.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    for(int i = 0, n = abs(BI.biHeight); i < n; i++)
    {
        for(int j = 0; j < BI.biWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, readFile);
            if(triple.rgbtGreen == 0x00 && triple.rgbtBlue == 0x00 && triple.rgbtRed == 0xff)
            {
                triple.rgbtGreen = 0xff;
                triple.rgbtBlue = 0xff;
            }
            fwrite(&triple, sizeof(RGBTRIPLE), 1, writeFile);
        }
        fseek(readFile, padding, SEEK_CUR);
        for(int x = 0; x < padding; x++)
        {
            fputc(0x00, writeFile);
        }
    }
    fclose(readFile);
    fclose(writeFile);
    return 0;
}
