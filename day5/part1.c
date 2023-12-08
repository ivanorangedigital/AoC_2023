#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 1000
#define COL 255
#define M_DIGITS 20 + 1

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

void printData(char **matrix, int k) {
  for (char **ptr = matrix; ptr < matrix + k; ptr++)
    printf("%s", *ptr);
}

int readFile(char **matrix) {
  FILE *fptr = fopen("input3.txt", "r");
  char **ptr = matrix;

  char line[COL];
  while (fgets(line, COL, fptr))
    strncpy(*ptr++, line, COL);

  return ptr - matrix;
}

int setIntsToArr(unsigned long long *arr, char *row) {
  char n[M_DIGITS];
  char *ptr = row, *str = n;
  unsigned long long *arrPtr = arr;

  while (*ptr) {
    if (*ptr >= '0' && *ptr <= '9')
      *str++ = *ptr;
    else {
      *str = '\0';
      const int len = strlen(n);
      if (len > 0) {
        char *endPtr;
        unsigned long long tmp = strtoull(n, &endPtr, 10);

        if (*endPtr) {
          printf("errore di conversione\n");
          return -1;
        }

        *arrPtr++ = tmp;
        // clear
        strcpy(n, "");
        str = n;
      }
    }
    ptr++;
  }

  return arrPtr - arr;
}

// return rows of map
int createMap(unsigned long long map[ROW][COL][3], char **matrix, int rows) {
  // we can start from row 2
  int mapRow = 0, mapCol = 1;
  for (int row = 2; row <= rows; row++) {
    if (row < rows) {
      if (matrix[row][0] >= '0' && matrix[row][0] <= '9') {
        setIntsToArr(map[mapRow][mapCol++], &matrix[row][0]);
      } else if (strlen(matrix[row]) == 1) {
        map[mapRow++][0][0] = mapCol;
        mapCol = 1;
      }
    } else {
      map[mapRow++][0][0] = mapCol;
      mapCol = 1;
    }
  }

  return mapRow;
}

int findDestination(unsigned long long *numbers, int numbersLen,
                    unsigned long long map[COL][3]) {

  for (unsigned long long *numPtr = numbers; numPtr < numbers + numbersLen;
       numPtr++) {
    int foundedDestination = 0;
    for (int j = 1; j < map[0][0] && !foundedDestination; j++) {
      unsigned long long range = map[j][2];
      unsigned long long dest = map[j][0];
      unsigned long long origin = map[j][1];

      if ((*numPtr - origin) < range) {
        *numPtr = (*numPtr - origin) + dest;
        foundedDestination = 1;
      }
    }
  }

  return 0;
}

int main() {
  char **matrix;
  allocateMatrix(&matrix);

  // get rows of input
  int rows = readFile(matrix);
  // printData(matrix, rows);

  // create array of seeds
  unsigned long long intsToMap[64];
  // the first row contains our numbers
  int intsLen = setIntsToArr(intsToMap, &matrix[0][0]);
  // error on number conversion
  if (intsLen == -1)
    return -1;

  // create all avaialable maps
  unsigned long long map[ROW][COL][3];
  /*
   * <row 0>
   *         0  1  2
   * <col 0> N (numbers of cols, included himself) ex. 3
   * <col 1> 50 98 2
   * <col 2> 52 50 48
   *
   * <row 1>
   *         0  1  2
   * <col 0> N (numbers of cols, included himself) ex. 4
   * <col 1> 0 15 37
   * <col 2> 37 52 2
   * <col 3> 39 0 15
   *
   * for each map we need to return intsLen numbers
   */

  int mapLen = createMap(map, matrix, rows);

  // find final destination
  for (int i = 0; i < mapLen; i++) {
    findDestination(intsToMap, intsLen, map[i]);
  }

  // find min
  int min = intsToMap[0];
  for (int i = 1; i < intsLen; i++) {
    if (intsToMap[i] < min)
      min = intsToMap[i];
  }

  printf("total: %d\n", min);
  //
  // for (int i = 0; i < mapLen; i++) {
  //   for (int j = 1; j < map[i][0][0]; j++) {
  //     printf("%llu %llu %llu\n", map[i][j][0], map[i][j][1], map[i][j][2]);
  //   }
  //   printf("\n");
  // }

  // before close script
  deallocateMatrix(matrix);

  return 0;
}
