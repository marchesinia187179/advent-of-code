//
// Created by Marchesini Alessandro on 05/12/25.
//

#include "day_three.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Finds the largest possible two-digit joltage from the battery bank
 * It uses a greedy approach:
 * 1. Finds the largest digit available for the tens place (firstMax).
 * 2. Finds the largest digit that appears after it for the ones place (secondMax) */
int getJoltage(const char *bank, const int bankSize) {
    char firstMax = '0', secondMax = '0';
    int indexFirstMax = 0;
    for (int i = 0; i < bankSize - 1; i++) {
        if (bank[i] > firstMax) {
            firstMax = bank[i];
            indexFirstMax = i;
        }
    }

    for (int i = indexFirstMax + 1; i < bankSize; i++) {
        if (bank[i] >= secondMax) secondMax = bank[i];
    }

    return (firstMax - '0') * 10 + (secondMax - '0');
}

/* Reads a single line (a bank of batteries) from the input file 'f' and stores it into the 'bank' buffer
 * Reading stops when a newline character ('\n') is found or the end of the file (EOF) is reached
 * A null terminator ('\0') is added at the end of the bank string */
bool setBank(FILE *f, char *bank, const int bankSize) {
    char c;
    int i = 0;
    while ((c = (char) fgetc(f)) != EOF) {
        if (c == '\n') break;

        bank[i] = c;
        i++;
    }
    bank[bankSize] = '\0';

    if (c == EOF && i == 0) return false;
    return true;
}

int getBankLength(FILE *f) {
    int bankLength = 0;
    while (fgetc(f) != '\n') {
        bankLength++;
    }
    fseek(f, 0, SEEK_SET);
    return bankLength;
}

int day_three_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error opening file %s\n", fileName);
        return 1;
    }

    int totalJoltages = 0, bankSize = getBankLength(f);
    char *bank = malloc(sizeof(char) * bankSize + 1);
    if (bank == NULL) {
        printf("Error allocating memory for bank\n");
        return 1;
    }

    while (setBank(f, bank, bankSize)) {
        totalJoltages += getJoltage(bank, bankSize);
    }

    printf("Total Joltages: %d\n", totalJoltages);
    free(bank);
    fclose(f);
    return 0;
}
