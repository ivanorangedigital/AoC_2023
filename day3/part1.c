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

void deallocateMatrix(char **matrix) {
  for (int i = 0; i < ROW; i++)
    free(matrix[i]);
  free(matrix);
}

int readFile(char **matrix) {
  FILE *fptr = fopen("input.txt", "r");
  char **ptr = matrix;

  char line[COL];
  while (fgets(line, COL, fptr))
    strncpy(*ptr++, line, COL);

  return ptr - matrix;
}

void printData(char **matrix, int k) {
  for (char **ptr = matrix; ptr < matrix + k; ptr++)
    printf("%s", *ptr);
}

int findNumber(int row, int sum, int count, int init, char **initMatrix) {
  /*
   * data structur
   * [
   * 	<row>[[N], [<sum_indexes>, <count>, <effective_number>], ... N - 1 ]
   * ]
   */

  static int matrix[ROW][COL][3];

  if (init) {
    // cycle rows
    for (int i = 0; i < row; i++) {
      // cycle cols
      char number[9 + 1];
      int indexNumber = 0, indexMatrix = 1, sum = 0, count = 0;

      for (int j = 0; j < strlen(initMatrix[i]); j++) {
        char n = initMatrix[i][j];

        // check if n is number
        if (n >= '0' && n <= '9') {
          number[indexNumber++] = n;
        } else {
          number[indexNumber] = '\0';
          const int ln = strlen(number);
          // printf("%d\n", ln);
          // se è piu grande abbiamo il primo numero

          if (ln > 0) {
            int sum = 0;
            for (int _i = j - ln; _i < j; _i++) {
              // printf("_i %d\n", _i);
              sum += _i;
            }
            // printf("somma %d, ln %d", sum, ln);
            matrix[i][0][0] = ++count;
            matrix[i][indexMatrix][0] = sum;
            matrix[i][indexMatrix][1] = ln;
            matrix[i][indexMatrix++][2] = atoi(number);

            // reset
            indexNumber = 0;
            strcpy(number, "");
            // printf("count: %d %d %d %d\n", matrix[row][0][0],
            //        matrix[row][indexMatrix - 1][0],
            //        matrix[row][indexMatrix - 1][1],
            //        matrix[row][indexMatrix - 1][2]);
          }
        }
      }
    }
    return 0;
  }

  for (int i = 1; i <= matrix[row][0][0]; i++) {
    if (matrix[row][i][0] == sum && matrix[row][i][1] == count)
      return matrix[row][i][2];
  }
  return -1;
}

int *getSymbolsIndex(int *ptr, char **matrix, int k) {
  for (int row = 0; row < k; row++) {
    for (int i = 0; i < strlen(matrix[row]); i++) {
      char n = matrix[row][i];
      if (n != '.' && !(n >= '0' && n <= '9') && n != '\n') {
        *ptr++ = (row * 1000) + i;
      }
    }
  }
  return ptr;
}

int findNearNumbers(int *symbols, int k, char **matrix, int rows) {
  // se trova una corrispondenza a row - 1 escludiamo le diagonali sopra
  // cerchiamo a sinistra e a destra
  // se trova una corripondenza a row + 1 escludiamo le diagonali sotto
  int mainSum = 0;

  for (int *sym = symbols; sym < symbols + k; sym++) {
    int row = *sym / 1000;
    int i = *sym % 1000;

    // check top
    if (row - 1 >= 0) {
      if (matrix[row - 1][i] >= '0' && matrix[row - 1][i] <= '9') {
        char *ptrLeft, *ptrRight;
        int sum = 0, count = 0;

        ptrLeft = &matrix[row - 1][i];
        ptrRight = &matrix[row - 1][i + 1];

        int index1 = i;
        while (*ptrLeft >= '0' && *ptrLeft <= '9') {
          count++;
          sum += index1--;
          ptrLeft--;
        }

        int index2 = i + 1;
        while (*ptrRight >= '0' && *ptrRight <= '9') {
          count++;
          sum += index2++;
          ptrRight++;
        }

        int findedNumber = findNumber(row - 1, sum, count, 0, matrix);
        if (findedNumber != -1)
          mainSum += findedNumber;
      } else {
        // search in diagonals if no number on top and row - 1 > 0
        char *ptrLeft = &matrix[row - 1][i - 1];

        char *ptrRight = &matrix[row - 1][i + 1];

        int index1 = i - 1, sum1 = 0, count1 = 0;
        while (*ptrLeft >= '0' && *ptrLeft <= '9') {
          count1++;
          sum1 += index1--;
          ptrLeft--;
        }

        int index2 = i + 1, sum2 = 0, count2 = 0;
        while (*ptrRight >= '0' && *ptrRight <= '9') {
          count2++;
          sum2 += index2++;
          ptrRight++;
        }

        int findedNumber1 = findNumber(row - 1, sum1, count1, 0, matrix);
        if (findedNumber1 != -1)
          mainSum += findedNumber1;

        int findedNumber2 = findNumber(row - 1, sum2, count2, 0, matrix);
        if (findedNumber2 != -1)
          mainSum += findedNumber2;
      }
    }

    // find in same row
    if (i - 1 >= 0) {
      char *ptrLeft = &matrix[row][i - 1];
      int sum = 0, count = 0;

      int index = i - 1;
      while (*ptrLeft >= '0' && *ptrLeft <= '9') {
        count++;
        sum += index--;
        ptrLeft--;
      }

      int findedNumber = findNumber(row, sum, count, 0, matrix);
      if (findedNumber != -1)
        mainSum += findedNumber;
    }

    if (i + 1 < strlen(matrix[row])) {
      char *ptrRight = &matrix[row][i + 1];
      int sum = 0, count = 0;

      int index = i + 1;
      while (*ptrRight >= '0' && *ptrRight <= '9') {
        count++;
        sum += index++;
        ptrRight++;
      }

      int findedNumber = findNumber(row, sum, count, 0, matrix);
      if (findedNumber != -1)
        mainSum += findedNumber;
    }

    // bottom
    if (row + 1 < rows) {
      if (matrix[row + 1][i] >= '0' && matrix[row + 1][i] <= '9') {
        char *ptrLeft, *ptrRight;
        int sum = 0, count = 0;

        ptrLeft = &matrix[row + 1][i];
        ptrRight = &matrix[row + 1][i + 1];

        int index1 = i;
        while (*ptrLeft >= '0' && *ptrLeft <= '9') {
          count++;
          sum += index1--;
          ptrLeft--;
        }

        int index2 = i + 1;
        while (*ptrRight >= '0' && *ptrRight <= '9') {
          count++;
          sum += index2++;
          ptrRight++;
        }

        int findedNumber = findNumber(row + 1, sum, count, 0, matrix);
        if (findedNumber != -1)
          mainSum += findedNumber;
      } else {
        // search in diagonals if no number on top and row - 1 > 0
        char *ptrLeft = &matrix[row + 1][i - 1];

        char *ptrRight = &matrix[row + 1][i + 1];

        int index1 = i - 1, sum1 = 0, count1 = 0;
        while (*ptrLeft >= '0' && *ptrLeft <= '9') {
          count1++;
          sum1 += index1--;
          ptrLeft--;
        }

        int index2 = i + 1, sum2 = 0, count2 = 0;
        while (*ptrRight >= '0' && *ptrRight <= '9') {
          count2++;
          sum2 += index2++;
          ptrRight++;
        }

        int findedNumber1 = findNumber(row + 1, sum1, count1, 0, matrix);
        if (findedNumber1 != -1)
          mainSum += findedNumber1;

        int findedNumber2 = findNumber(row + 1, sum2, count2, 0, matrix);
        if (findedNumber2 != -1)
          mainSum += findedNumber2;
      }
    }
  }

  return mainSum;
}

// alghoritm
int main() {
  char **matrix;
  // allocateMemory
  allocateMatrix(&matrix);

  // readFile
  int rows = readFile(matrix);

  // printData
  // printData(matrix, rows);

  // initialize data
  findNumber(rows, 0, 0, 1, matrix);

  int symbols[1000];
  // find symbols
  const int ln = getSymbolsIndex(symbols, matrix, rows) - symbols;

  // start find numbers
  int sum = findNearNumbers(symbols, ln, matrix, rows);
  printf("total: %d\n", sum);

  // for (int i = 0; i < ln; i++) {
  //   printf("row: %d, col: %d\n", symbols[i] / 10, symbols[i] % 10);
  // }

  // printf("%d", findNumber(0, 3, 3, 0, matrix));

  // deallocate memory
  deallocateMatrix(matrix);
}
