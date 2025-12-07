//
// Created by Marchesini Alessandro on 05/12/25.
//

#include "day_three.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUMBER_OF_BATTERIES 12

/* ========== Utility and Conversion Methods ========== */
double convert_Joltage_String_To_Double(const char joltage[NUMBER_OF_BATTERIES]) {
    double joltageValue = 0;
    for (int i = 0; i < NUMBER_OF_BATTERIES; i++) {
        joltageValue = joltageValue * 10 + joltage[i] - '0';
    }
    return joltageValue;
}

/* Reads a single line (a bank of batteries) from the input file 'f' and stores it into the 'bank' buffer
 * Reading stops when a newline character ('\n') is found or the end of the file (EOF) is reached
 * A null terminator ('\0') is added at the end of the bank string */
bool read_Next_Battery_Bank(FILE *f, char *bank, const int bankSize) {
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

int get_Bank_Length(FILE *f) {
    int bankLength = 0;
    while (fgetc(f) != '\n') {
        bankLength++;
    }
    fseek(f, 0, SEEK_SET);
    return bankLength;
}

/* ========== Joltage Calculation Method (Part 1 - Greedy) ========== */
/* Finds the largest possible two-digit joltage from the battery bank
 * It uses a greedy approach:
 * 1. Finds the largest digit available for the tens place (firstMax).
 * 2. Finds the largest digit that appears after it for the ones place (secondMax) */
int find_Max_Two_Digit_Joltage(const char *bank) {
    char firstMax = '0', secondMax = '0';
    int indexFirstMax = 0;
    for (int i = 0; i < strlen(bank) - 1; i++) {
        if (bank[i] > firstMax) {
            firstMax = bank[i];
            indexFirstMax = i;
        }
    }

    for (int i = indexFirstMax + 1; i < strlen(bank); i++) {
        if (bank[i] >= secondMax) secondMax = bank[i];
    }

    return (firstMax - '0') * 10 + (secondMax - '0');
}

/* ========== Joltage Calculation Method (Part 2 - Recursive) ========== */
/* Recursively finds the largest possible joltage string (a sequence of 12 digits)
 * by greedily selecting the largest available digit at each level */

/*
 * This is a single-path greedy traversal, NOT a multi-branching DFS.
 * Example Path (Bank: 7 9 1 5 3 4, N=3):
 *                  O (treeLevel=0)
 * Range: [0 to 3] -> (7915) -> **Chosen: 9** (at index 1)
 *                         |
 *                  O (treeLevel=1)
 * Range: [2 to 4] -> (153) -> **Chosen: 5** (at index 3)
 *                         |
 *                  O (treeLevel=2)
 * Range: [4 to 5] -> (34) -> **Chosen: 4** (at index 5)
 *                         |
 *                  O (treeLevel=3)
 *              Base Case -> Result: 954
 */
void find_Max_Joltage_Recursive(int firstPossibleIndex, int treeLevel, char *bank, char currentJoltage[NUMBER_OF_BATTERIES], double *maxJoltage) {
    if (treeLevel == NUMBER_OF_BATTERIES) {
        double joltageValue = convert_Joltage_String_To_Double(currentJoltage);
        if (joltageValue > *maxJoltage) *maxJoltage = joltageValue;
        return;
    }

    char maxBattery = '0';
    int maxBatteryIndex = 0;
    int lastPossibleIndex = (int) strlen(bank) - NUMBER_OF_BATTERIES + treeLevel;
    for (int i = firstPossibleIndex; i <= lastPossibleIndex; i++) {
        if (bank[i] > maxBattery) {
            maxBattery = bank[i];
            maxBatteryIndex = i;
        }
    }

    currentJoltage[treeLevel] = bank[maxBatteryIndex];
    find_Max_Joltage_Recursive(maxBatteryIndex + 1, treeLevel + 1, bank, currentJoltage, maxJoltage);
}

double calculate_Max_Joltage(char *bank) {
    double maxJoltage = 0;
    char currentJoltage[NUMBER_OF_BATTERIES];
    find_Max_Joltage_Recursive(0, 0, bank, currentJoltage, &maxJoltage);
    return maxJoltage;
}

/* ========== Main Program Execution ========== */
int day_three_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error opening file %s\n", fileName);
        return 1;
    }

    double totalJoltage = 0;
    int bankSize = get_Bank_Length(f);
    char *bank = malloc(sizeof(char) * bankSize + 1);
    if (bank == NULL) {
        printf("Error allocating memory for bank\n");
        return 1;
    }

    while (read_Next_Battery_Bank(f, bank, bankSize)) {
        // totalJoltage += find_Max_Two_Digit_Joltage(bank);
        totalJoltage += calculate_Max_Joltage(bank);
    }

    printf("%.0f\n", totalJoltage);
    free(bank);
    fclose(f);
    return 0;
}
