#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  SAMPLE_LENGTH 18
#define LENGTH_DIGIT 256

int Input(unsigned char* sample, FILE* file);
void TableOfShifts(unsigned char* sample, int* arrayOfShift, int length);
void Switch(unsigned char* buffer, int current, int indexBuffer);
int Fseek(unsigned char* buffer, int length, int currentShift, FILE* file);
int ReadingBuffer(unsigned char* buffer, int lengthSample, int currentShift, FILE* file);
void BoyerMoure(unsigned char* sample, int* arrayOfShift, int lengthSample, FILE* file);

int main() {
    unsigned char  sample[SAMPLE_LENGTH] = { '\0' };
    FILE* file = fopen("in.txt", "r");
    if (file == NULL) {
        return 0;
    }
    int length = Input(sample, file);
    if (length == -1) {
        fclose(file);
        return 0;
    }
    int arrayOfShift[LENGTH_DIGIT];
    TableOfShifts(sample, arrayOfShift, length);
    BoyerMoure(sample, arrayOfShift, length, file);
    fclose(file);
    return 0;
}

int Input(unsigned char* sample, FILE* file) {
    int length = 0;
    while (1) {
        int c = fgetc(file);
        if (c == EOF) {
            return -1;
        }
        if (c == '\n') {
            break;
        }
        sample[length] = c;
        length++;
    }
    sample[length] = '\0';
    return length;
}

void TableOfShifts(unsigned char* sample, int* arrayOfShift, int length) {
    for (int i = 0; i < LENGTH_DIGIT; ++i) {
        arrayOfShift[i] = length;
    }
    for (int j = length - 2; j >= 0; --j) {
        int index = (int)sample[j];
        if (arrayOfShift[index] == length) {
            arrayOfShift[index] = length - j - 1;
        }
    }
}

void Switch(unsigned char* buffer, int current, int indexBuffer) {
    for (int i = 0; i < indexBuffer; ++i) {
        buffer[i] = buffer[current];
        current++;
    }
}

int Fseek(unsigned char* buffer, int length, int currentShift, FILE* file) {
    int indexBuffer = length - currentShift;
    Switch(buffer, currentShift, indexBuffer);
    int readLength = fread(buffer + length - currentShift, sizeof(char), currentShift, file);
    return (readLength + length - currentShift);
}

int ReadingBuffer(unsigned char* buffer, int lengthSample, int currentShift, FILE* file) {
    if (lengthSample > currentShift) {
        return  Fseek(buffer, lengthSample, currentShift, file);
    }
    else {
        return fread(buffer, sizeof(char), lengthSample, file);
    }
}

void BoyerMoure(unsigned char* sample, int* arrayOfShift, int lengthSample, FILE* file) {
    unsigned char buffer[SAMPLE_LENGTH];
    int readLength = fread(buffer, sizeof(char), lengthSample, file);
    buffer[readLength] = '\0';
    int count = 0;
    int shift = 0;
    int currentShift = 0;
    int maxIndexSample = lengthSample - 1;
    while (readLength == lengthSample) {
        for (int i = lengthSample - 1; i >= 0; --i) {
            printf("%d ", i + shift + 1);
            if (buffer[i] != sample[i]) {
                currentShift = arrayOfShift[(int)buffer[maxIndexSample]];
                break;
            }
            else {
                count++;
            }
        }
        if (count == lengthSample) {
            currentShift = lengthSample;

        }
        readLength = ReadingBuffer(buffer, lengthSample, currentShift, file);
        shift += currentShift;
        count = 0;
    }
}

