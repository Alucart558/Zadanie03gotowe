#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void standartTask();
void commandLineTask(int, char *[]);
long fileSize(char *fileName);
int numberOfBytesInHex(long number);

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("File not found.");
    }
    else
    {
        commandLineTask(argc, argv);
    }
    return 0;
}

void standartTask()
{
    FILE *outputFile = fopen("output.txt", "wb");
    FILE *inputFile = fopen("input.txt", "rb");
    if (inputFile == NULL)
    {
        printf("File not found.");
        /* return 1; */
    }

    unsigned char buffer[16];

    size_t currentAddress = 0;
    size_t bytesRead;
    size_t bytesWritten;
    size_t i;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
    {
        bytesWritten = fwrite(buffer, 1, sizeof(buffer), outputFile);

        for (i = 0; i < bytesRead; i++)
        {
            fprintf(outputFile, "%2X ", buffer[i]);
            printf("%2X ", buffer[i]);
        }
        fprintf(outputFile, "%04X-%04X\n", currentAddress, currentAddress + bytesRead - 1);
        printf("%04X-%04X\n", currentAddress, currentAddress + bytesRead - 1);
        currentAddress += bytesRead;
    }
    if (ferror(inputFile))
    {
        printf("File read error.");
    }

    fclose(inputFile);
    fclose(outputFile);
}

void commandLineTask(int argc, char *argv[])
{
    char *fileName = argv[1];

    FILE *inputFile = fopen(fileName, "rb");
    if (inputFile == NULL)
    {
        printf("File not found.");
    }

    long fileLength = fileSize(fileName);
    int numberOfBytesForAddress = MAX(numberOfBytesInHex(fileLength), 4);

    unsigned char buffer[16];

    size_t currentAddress = 0;
    size_t bytesRead;
    size_t bytesWritten;
    size_t i;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
    {

        for (i = 0; i < sizeof(buffer); i++)
        {
            if (i < bytesRead)
            {
                printf("%02X ", buffer[i]);
            }
            else
            {
                printf("   ");
            }
        }

        printf("%0*X-%0*X\n", numberOfBytesForAddress, currentAddress, numberOfBytesForAddress, currentAddress + bytesRead - 1);
        currentAddress += bytesRead;
    }
    if (ferror(inputFile))
    {
        printf("File read error.");
    }

    fclose(inputFile);
}

long fileSize(char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    fseek(file, 0, SEEK_END);
    long position = ftell(file);
    fclose(file);
    return position;
}

int numberOfBytesInHex(long number)
{
    char hex[10];
    sprintf(hex, "%x", number);
    return strlen(hex);
}