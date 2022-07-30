#include <stdio.h>
#include <string.h>

#define LENGTH_SAMPLE 18

int Input(unsigned char* sample, FILE* file);
int Degree(int value, int degree);
void ReplacingInArray(unsigned char* text, char futureValue, int lengthSample);
int HashFunction(unsigned char* array, int LengthSample);
int ScanNewValue(unsigned char* buffer, int hashBuffer, int lengthSample, int degree, FILE* file);
void RabinKarp(unsigned char* sample, int lengthSample, FILE* file);

int main() {
    unsigned char sample[LENGTH_SAMPLE];
    FILE* file = fopen("in.txt", "r");
    if (file == NULL) {
        return 0;
    }
    int length = Input(sample, file);
    if (length != -1) {
        RabinKarp(sample, length, file);
    }
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

int Degree(int value, int degree) {
    int result = 1;
    for (int i = 1; i <= degree; ++i) {
        result = result * value;
    }
    return result;
}

void ReplacingInArray(unsigned char* text, char futureValue, int lengthSample) {
    for (int i = 0; i < lengthSample - 1; ++i) {
        text[i] = text[i + 1];
    }
    text[lengthSample - 1] = futureValue;
}
int HashFunction(unsigned char* array, int LengthSample) {
    int futureValue = 1;
    int hash = 0;
    for (int j = 0; j < LengthSample; ++j) {
        hash += (array[j] % 3) * futureValue;
        futureValue = futureValue * 3;
    }
    return hash;
}

int ScanNewValue(unsigned char* text, int hashBuffer, int lengthSample, int degree, FILE* file) {
    unsigned char futureValue;
    int c = fgetc(file);
    if (c == EOF) {
        return -1;
    }
    futureValue = c;
    hashBuffer = (hashBuffer - text[0] % 3) / 3 + (futureValue % 3) * degree;
    ReplacingInArray(text, futureValue, lengthSample);
    text[lengthSample - 1] = futureValue;
    return hashBuffer;
}

void RabinKarp(unsigned char* sample, int lengthSample, FILE* file) {
    int shift = 1;
    unsigned char buffer[LENGTH_SAMPLE];
    buffer[lengthSample] = '\0';
    int lengthBuffer = fread(buffer, sizeof(unsigned char), lengthSample, file);
    int hashBuffer = HashFunction(buffer, lengthBuffer);
    int hashSample = HashFunction(sample, lengthSample);
    printf("%d ", hashSample);
    int degree = Degree(3, lengthSample - 1);
    while (lengthBuffer == lengthSample) {
        if (hashBuffer == hashSample) {
            for (int i = 0; i < lengthSample; ++i) {
                printf("%d ", shift + i);
                if (buffer[i] != sample[i]) {
                    break;
                }
            }
        }
        hashBuffer = ScanNewValue(buffer, hashBuffer, lengthSample, degree, file);
        if (hashBuffer == -1) {
            return;
        }
        shift += 1;
    }
}

