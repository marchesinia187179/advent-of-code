//
// Created by Marchesini Alessandro on 02/12/25.
//

#include <math.h>
#include <stdio.h>

#define BUFFER_SIZE 5
#define DIAL_MIN_NUMBER 0
#define DIAL_MAX_NUMBER 99

/* Rewinds the file pointer to the character immediately following the newline ('\n')
 * found in the buffer, ensuring the next read starts at the next instruction.
 * If no '\n' is found, the file pointer remains advanced by BUFFER_SIZE */
void restoreFilePointerIndicator(FILE *f, char const buf[BUFFER_SIZE]) {
    if (fgetc(f) == EOF) return;
    fseek(f, -1L, SEEK_CUR);

    for (int i = 1; i < BUFFER_SIZE; i++) {
        if (buf[i] == '\n') {
            fseek(f, -(long)(BUFFER_SIZE - 1 - i), SEEK_CUR);
            return ;
        }
    }
}

/* Decrements the dialPointer
 * If the pointer falls below the start of the cycle (DIAL_MIN_NUMBER),
 * it wraps around and resets to the end of the cycle (DIAL_MAX_NUMBER) */
void leftCase(int *dialPointer, int rotation) {
    while (rotation) {
        (*dialPointer)--;
        if (*dialPointer < DIAL_MIN_NUMBER) *dialPointer = DIAL_MAX_NUMBER;
        rotation--;
    }
}

/* Increments the dialPointer
 * If the pointer goes past the end of the cycle (DIAL_MAX_NUMBER),
 * it wraps around and resets to the beginning (DIAL_MIN_NUMBER) */
void rightCase(int *dialPointer, int rotation) {
    while (rotation) {
        (*dialPointer)++;
        if (*dialPointer > DIAL_MAX_NUMBER) *dialPointer = 0;
        rotation--;
    }
}

int getRotation(const char buf[BUFFER_SIZE]) {
    int rotation = 0;

    // The for calculates the value of the rotation
    // If the scan finds a no-number it breaks
    for (int i = 1; i < BUFFER_SIZE; i++) {
        int c = buf[i] - '0';

        if (c >= 0 && c <= 9) rotation = rotation * 10 + c;
        else break;
    }

    // The dial operates on a cycle of 100 (0-99)
    // We use the modulus to determine the effective rotation
    rotation = rotation % 100;
    return rotation;
}

int day_one_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening %s\n", fileName);
        return 1;
    }

    int dialPointer = 50;
    int zeroCount = 0;
    char buf[BUFFER_SIZE];
    while (fread(buf, sizeof(char), BUFFER_SIZE, f)) {
        // Get the rotation and set the dialPointer
        int rotation = getRotation(buf);
        if (buf[0] == 'L') leftCase(&dialPointer, rotation);
        else rightCase(&dialPointer, rotation);

        // Counting the zeros
        if (dialPointer == 0) zeroCount++;

        restoreFilePointerIndicator(f, buf);
    }

    printf("%d\n", zeroCount);
    fclose(f);
    return 0;
}