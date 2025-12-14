//
// Created by Marchesini Alessandro on 14/12/25.
//

#include "day_four.h"

#include <stdbool.h>
#include <stdio.h>

int close_rolls(const char *ptr, int rows, int cols, int currRow, int currCol) {
    int counter = 0;
    for (int i = currRow - 1; i <= currRow + 1; i++) {
        for (int j = currCol - 1; j <= currCol + 1; j++) {
            if (i < 0 || i >= rows || j < 0 || j >= cols) continue;
            if (i == currRow && j == currCol) continue;

            if (*(ptr + (i * cols + j)) == '@') counter++;
        }
    }
    return counter;
}

int get_accessible_rolls(char *ptr, int rows, int cols) {
    int accessible_rolls = 0;
    bool emptyRow = false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            switch (*(ptr + (i * cols + j))) {
                case '@':
                    if (close_rolls(ptr, rows, cols, i, j) < 4) accessible_rolls++;
                    break;
                case '.':
                    break;
                default:
                    emptyRow = true;
                    break;
            }
        }

        if (emptyRow) break;
    }

    return accessible_rolls;
}

void get_lines(FILE *f, char *ptr, int rows, int cols) {
    char c;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c = fgetc(f);
            *(ptr + (i * cols + j)) = c;
        }
        fgetc(f);   // to avoid the '\n'
    }
}

int get_dimensions(FILE *f, int *rows, int *cols) {
    *rows = 0, *cols = 0;

    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') (*rows)++;
        else (*cols)++;
    }
    *cols = *cols / *rows;

    rewind(f);
    return (*cols == 0) * -1;   // Return -1 if the file is empty
}

int day_four_main(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Error opening file %s\n", fileName);
        return 1;
    }

    int cols, rows;
    if (get_dimensions(f, &rows, &cols) != 0) {
        printf("Error getting dimensions\n");
        fclose(f);
        return 1;
    }

    char lines[rows][cols];
    get_lines(f, &lines[0][0], rows, cols);

    printf("Total rolls: %d\n", get_accessible_rolls(&lines[0][0], rows, cols));

    fclose(f);
    return 0;
}
