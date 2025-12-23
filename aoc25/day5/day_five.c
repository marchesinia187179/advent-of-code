//
// Created by Marchesini Alessandro on 23/12/25.
//

#include "day_five.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/errno.h>

#define UPDATE_RANGE_LIMIT(limit, c) {limit = limit * 10 + (c - '0');}

struct range {
    size_t start, end;
}; typedef struct range range;

struct rangesArray {
    int rangesCount;
    range *ranges;
}; typedef struct rangesArray ranges;

int initialize_ranges_array(ranges *ranges, int rangesCount) {
    ranges->rangesCount = rangesCount;
    ranges->ranges = malloc(sizeof(ranges) * ranges->rangesCount);
    if (ranges->ranges == NULL) return 1;

    for (int i = 0; i < ranges->rangesCount; i++) {
        ranges->ranges[i].start = 0;
        ranges->ranges[i].end = 0;
    }

    return 0;
}

/**
 * @brief Counts the number of fresh ingredients.
 * * An ingredient is fresh if it is contained within a range.
 */
int count_fresh_ingredients(FILE *f, ranges *ranges, int *freshIngredientsCount) {
    char c;
    size_t ingredient = 0;
    int ingredientsCount = 0;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            for (int i = 0; i < ranges->rangesCount; i++) {
                if (ingredient >= ranges->ranges[i].start && ingredient <= ranges->ranges[i].end) {
                    (*freshIngredientsCount)++;
                    break;
                }
            }
            if (ingredient != 0) ingredientsCount++;
            ingredient = 0;
        } else {
            ingredient = ingredient * 10 + (c - '0');
        }
    }

    return ingredientsCount == 0;
}

/**
 * @brief Reads the ranges from the file and stores them into the rangesArray.
 * * A range is written on a line with the bottom limit separated from the top limit with a '-'.
 * * There are no spaces in the lines.
 */
int get_ranges(FILE *f, ranges *ranges) {
    char c;
    int currRangesCount = 0;
    bool isReadingFirstRange = true;

    while (currRangesCount != ranges->rangesCount) {
        switch (c = fgetc(f)) {
            case '-': isReadingFirstRange = false; break;
            case '\n':
                isReadingFirstRange = true;
                currRangesCount++;
                break;
            default:
                if (isReadingFirstRange) { UPDATE_RANGE_LIMIT(ranges->ranges[currRangesCount].start, c); }
                else { UPDATE_RANGE_LIMIT(ranges->ranges[currRangesCount].end, c); }
                break;
        }
    }

    return currRangesCount != ranges->rangesCount;
}

/**
 * @brief Counts the number of ranges.
 * * Each range is written on its own line.
 * * Ranges are located at the beginning of the file and separated from the ingredients by a blank line.
 */
int count_ranges(FILE *f) {
    char c;
    int rangesCount = 0;
    bool isBlankLine = true;

    while ((c = getc(f)) != EOF) {
        if (c == '\n') {
            if (isBlankLine) break;

            isBlankLine = true;
            rangesCount++;
        } else {
            isBlankLine = false;
        }
    }
    rewind(f);
    return rangesCount;
}

int day_five_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) {
        errno = ENOENT;
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    ranges ranges;
    int freshIngredientsCount = 0;

    int rangesCount = count_ranges(f);
    if (rangesCount == 0) {
        errno = EDOM;
        perror("Ranges not found");
        exit(EXIT_FAILURE);
    }

    if (initialize_ranges_array(&ranges, rangesCount)) {
        errno = ENOMEM;
        perror("Error creating ranges");
        exit(EXIT_FAILURE);
    }

    if (get_ranges(f, &ranges)) {
        errno = EDOM;
        perror("Error reading ranges");
        exit(EXIT_FAILURE);
    }

    if (count_fresh_ingredients(f, &ranges, &freshIngredientsCount)) {
        errno = EDOM;
        perror("Ingredients not found");
        exit(EXIT_FAILURE);
    }

    printf("Fresh ingredients: %d\n", freshIngredientsCount);

    free(ranges.ranges);
    fclose(f);
    return 0;
}
