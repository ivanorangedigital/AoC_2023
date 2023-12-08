#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 200
#define COL 200

void allocateMatrix(char ***matrix) {
  *matrix = (char **)malloc(ROW * sizeof(char *));
  for (char **ptr = *matrix; ptr < *matrix + ROW; ptr++)
    *ptr = (char *)malloc(COL * sizeof(char));
}

int readFile(char **matrix) {
  FILE *fptr = fopen("input.txt", "r");
  char **ptr = matrix;

  char line[COL];
  while (fgets(line, COL, fptr))
    strncpy(*ptr++, line, COL);

  return ptr - matrix;
}

int countGamePoints(char *line, int index, int isLast) {
  static int multipler[ROW] = {-1};
  if (multipler[0] == -1) {
    for (int r = 0; r < ROW; r++) {
      multipler[r] = 1;
    }
  }

  int winnerNumbers[64] = {-1}, myNumbers[64] = {-1};
  int *winner = winnerNumbers, *my = myNumbers;
  // how many numbers are same of winnerNumbers
  int count = 0, breakPoint = 0, start = 0;

  char str[9 + 1];
  char *st = str;
  for (char *ptr = line; *ptr != '\0'; ptr++) {
    if (*ptr == ':')
      start = 1;

    if (start) {
      if (*ptr == '|')
        breakPoint = 1;

      if (!breakPoint) {
        if (*ptr >= '0' && *ptr <= '9') {
          *st++ = *ptr;
        } else {
          *st = '\0';
          const int ln = strlen(str);
          if (ln > 0) {
            int n = atoi(str);
            *winner++ = n;

            // reset
            strcpy(str, "");
            st = str;
          }
        }
      } else {
        if (*ptr >= '0' && *ptr <= '9') {
          *st++ = *ptr;
        } else {
          *st = '\0';
          const int ln = strlen(str);
          if (ln > 0) {

            int n = atoi(str);
            *my++ = n;

            // reset
            strcpy(str, "");
            st = str;
          }
        }
      }
    }
  }

  int winnerLn = winner - winnerNumbers;
  int myLn = my - myNumbers;

  winner = winnerNumbers;
  my = myNumbers;

  while (winner < winnerNumbers + winnerLn) {

    while (my < myNumbers + myLn) {
      if (*my == *winner)
        count++;
      my++;
    }

    my = myNumbers;
    winner++;
  }

  int value = multipler[index];
  while (value--) {
    for (int i = index + 1; i <= index + count; i++) {
      multipler[i] += 1;
    }
  }

  if (isLast) {
    int sum = 0;
    for (int i = 0; i <= index; i++) {
      sum += multipler[i];
    }
    return sum;
  }
  // return count > 0 ? 1 << (count - 1) : -1;
  return 0;
}

int main() {
  char **matrix;
  // allocate data to matrix
  allocateMatrix(&matrix);

  // get filw rows
  const int rows = readFile(matrix);

  int sum = 0;
  for (int row = 0; row < rows; row++) {
    int points = countGamePoints(matrix[row], row, row == rows - 1);
    if (points != -1)
      sum += points;
  }

  printf("total: %d\n", sum);

  return 0;
}
