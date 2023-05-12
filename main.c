#include <stdlib.h>
#include <stdio.h>

unsigned int bitsToInt(char * bits);
void rotate(char * path, char  * newPath, int mode);
int pixelRotation(int x, int y, int width, int height, int mode);

unsigned int bitsToInt(char * bits)
{ 
    return (unsigned char)bits[0] | ((unsigned char)bits[1] << 8) | ((unsigned char)bits[2] << 16) | ((unsigned char)bits[3] << 24);
}

void rotate(char * path, char  * newPath, int mode)
{
    FILE * inputStream, * outputStream;
    inputStream = fopen(path, "rb");
    outputStream = fopen(newPath, "wb");
    char intBuf[4];
    unsigned int size;
    unsigned int height;
    unsigned int width;
    int sizePos;
    int heightPos;
    int widthPos;

    sizePos = 2;
    fseek(inputStream, sizePos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    size = bitsToInt(intBuf);

    heightPos = 22;
    fseek(inputStream, heightPos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    height = bitsToInt(intBuf);

    widthPos = 18;
    fseek(inputStream, widthPos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    width = bitsToInt(intBuf);

    char * readBuffer;
    char * writeBuffer;

    readBuffer = malloc(size);
    writeBuffer = malloc(size);

    fseek(inputStream, 0, SEEK_SET);
    fread(readBuffer, size, 1, inputStream);

    for(int i = 0; i < 54; i++)
    {
        writeBuffer[i] = readBuffer[i];
    }

    if (mode == 1 || mode == 3)
    {
        for(int i = 0; i < 4; i++)
        {
            writeBuffer[widthPos + i] = readBuffer[heightPos + i];
            writeBuffer[heightPos + i] = readBuffer[widthPos + i];
        }
    }
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int opix = y * width + x;
            int npix = pixelRotation(x, y, width, height, mode);

            for (int i = 0; i < 3; i++)
            {
                int oindex = 54 + opix * 3 + i;
                int nindex = 54 + npix * 3 + i;
                
                writeBuffer[nindex] = readBuffer[oindex];
            }
        }
    }
    fwrite(writeBuffer, size, 1, outputStream);
    fclose(inputStream);
    fclose(outputStream);
    free(readBuffer);
    free(writeBuffer);
}

int pixelRotation(int x, int y, int width, int height, int mode){
    switch (mode)
    {
        default: //0
        return y * width + x;
        case 1: //90
        return height - y - 1 + x * height;
        case 2: //180
        return (height - y - 1) * width + (width - x - 1);
        case 3: //270 
        return y + (width - x -1) * height;
    }
}

int main(){
    char * filePath = "darthvador.bmp";
    char * file0 = "darthvadorcopy.bmp";
    rotate(filePath, file0, 0);
    char * file1 = "90degree.bmp";
    rotate(filePath, file1, 1);
    char * file2 = "180degree.bmp";
    rotate(filePath, file2, 2);
    char * file3 = "270degree.bmp";
    rotate(filePath, file3, 3);
    return 0;
}
