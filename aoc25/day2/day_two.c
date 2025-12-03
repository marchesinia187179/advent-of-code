//
// Created by Marchesini Alessandro on 03/12/25.
//

#include "day_two.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

// Return the length of the ID
int getLength(size_t ID) {
    size_t base = 10;
    int length = 0;
    while (ID / base) {
        length++;
        base *= 10;
    }
    length++;
    return length;
}

/* If the ID length is odd, then the ID cannot be invalid according to the puzzle
 * Instead, if the ID length is even, it calculates half the ID length
 * and checks if the result of the division and the remainder are equal */
bool isInvalid(size_t ID) {
    int IDLength = getLength(ID);
    if (IDLength % 2L != 0) return false;

    size_t base = (size_t) pow(10, (double) IDLength / 2);
    if (ID / base == ID % base) return true;
    return false;
}

void checkInvalidID(size_t firstID, size_t lastID, size_t *sumInvalidID) {
    for (size_t i = firstID; i <= lastID; i++) {
        if (isInvalid(i)) (*sumInvalidID) += i;
    }
}

int day_two_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error opening file %s\n", fileName);
        return 1;
    }

    size_t firstID = 0, lastID = 0, sumInvalidID = 0;
    bool isReadingSecondNuber = false;
    char c;

    while ((c = (char) fgetc(f)) != EOF) {
        if (c == ',') {     // Case the lastID is read (ID range is read)
            checkInvalidID(firstID, lastID, &sumInvalidID);

            // Restore each variable to check a new ID range
            firstID = lastID = 0;
            isReadingSecondNuber = false;
            continue;
        }

        if (c == '-') {     // Case the firstID is read
            isReadingSecondNuber = true;
            continue;
        }

        // Save firstID and lastID of the current ID range
        if (c >= '0' && c <= '9') {
            if (!isReadingSecondNuber) firstID = firstID * 10 + c - '0';
            else lastID = lastID * 10 + c - '0';
        }
    }
    checkInvalidID(firstID, lastID, &sumInvalidID);     // Check the last ID range

    printf("%lu\n", sumInvalidID);
    fclose(f);
    return 0;
}