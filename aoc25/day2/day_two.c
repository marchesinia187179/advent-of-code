//
// Created by Marchesini Alessandro on 03/12/25.
//

#include "day_two.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ID_LENGTH 20    // I hypothesize the max ID length is 20

int getIDLength(size_t ID) {
    size_t base = 10;
    while (ID / base) {
        base *= 10;
    }
    return (int) log10((double) base);
}

/* Check for section repetition
 * The possible size of the section is between 1 and IDLength / 2,
 * so sections greater than IDLength / 2 don't matter.
 * The checking works like this:
 *  - Take the i-th digit of the ID.
 *  - Each of its repetitions is distant by a multiple of 'sec' (section size 'j').
 *  - Check these repetitions without exceeding IDLength.
 *  - Increase 'i' and check the other repetitions only if
 *      - each check was successful, AND
 *      - the number of repetition checks performed was equal to (IDLength - 1) / sec. */
bool isInvalidSecondPartRec(int sec, int IDLength, char *ID) {
    if (sec > IDLength / 2) return false;

    bool ret = true;
    for (int i = 0; i < sec; i++) {
        int check = 0;
        for (int j = sec + i; j < IDLength; j += sec) {
            if (ID[j] != ID[i]) {
                ret = false;
                break;
            }
            check++;
        }
        if (check != (IDLength - 1) / sec) ret = false;
        if (ret == false) break;
    }

    return ret || isInvalidSecondPartRec(sec + 1, IDLength, ID);
}

bool isInvalidSecondPart(size_t ID) {
    char IDString[MAX_ID_LENGTH];
    snprintf(IDString, sizeof(IDString), "%zd", ID);
    return isInvalidSecondPartRec(1, getIDLength(ID), IDString);
}

/* If the ID length is odd, then the ID cannot be invalid according to the puzzle
 * Instead, if the ID length is even, it calculates half the ID length
 * and checks if the result of the division and the remainder are equal */
bool isInvalidFirstPart(size_t ID) {
    int IDLength = getIDLength(ID);
    if (IDLength % 2L != 0) return false;
    
    size_t base = (size_t) pow(10, (double) IDLength / 2);
    if (ID / base == ID % base) return true;
    return false;
}

void checkInvalidID(size_t firstID, size_t lastID, size_t *sumInvalidID) {
    for (size_t i = firstID; i <= lastID; i++) {
        // The FOR check only this IF, if we want to verify the first part of the puzzle
        // if (isInvalidFirstPart(i)) (*sumInvalidID) += i;

        // The FOR check only this IF, if we want to verify the second part of the puzzle
        if (isInvalidSecondPart(i)) (*sumInvalidID) += i;
    }
}

int day_two_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error opening file %s\n", fileName);
        return 1;
    }

    size_t firstID = 0, lastID = 0, sumInvalidID = 0;
    bool isReadingSecondNumber = false;
    char c;

    while ((c = (char) fgetc(f)) != EOF) {
        if (c == ',') {     // Case the lastID is read (ID range is read)
            checkInvalidID(firstID, lastID, &sumInvalidID);

            // Restore each variable to check a new ID range
            firstID = lastID = 0;
            isReadingSecondNumber = false;
            continue;
        }

        if (c == '-') {     // Case the firstID is read
            isReadingSecondNumber = true;
            continue;
        }

        // Save firstID and lastID of the current ID range
        if (c >= '0' && c <= '9') {
            if (!isReadingSecondNumber) firstID = firstID * 10 + c - '0';
            else lastID = lastID * 10 + c - '0';
        }
    }
    checkInvalidID(firstID, lastID, &sumInvalidID);     // Check the last ID range

    printf("%lu\n", sumInvalidID);
    fclose(f);
    return 0;
}
