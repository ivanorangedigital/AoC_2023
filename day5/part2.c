#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macro definition
#define ROW 1000
#define COL 255
#define MAX_ULL_LN 20 + 1
#define FILE_NAME "input.txt"

// type defintion
typedef unsigned long long ull;
typedef struct {
  ull min;
  ull max;
  ull range;
  int founded;
} Number;

// global data
char matrix[ROW][COL];
ull map[ROW][COL][3];

// usefule function
int readFile(char matrix[ROW][COL]) {
  FILE *fptr = fopen(FILE_NAME, "r");
  char(*ptr)[COL] = matrix;

  char line[COL];
  while (fgets(line, COL, fptr))
    strncpy(*ptr++, line, COL);

  return ptr - matrix;
}

void printData(char matrix[ROW][COL], int k) {
  for (char(*ptr)[COL] = matrix; ptr < matrix + k; ptr++)
    printf("%s", *ptr);
}

void printNumbers(Number *arr, int k) {
  for (Number *ptr = arr; ptr < arr + k; ptr++) {
    printf("%llu - %llu, ", ptr->min, ptr->max);
  }

  printf("\n\n");
}

int getNumbersFromRow(ull *arr, char *row) {
  char n[MAX_ULL_LN];
  char *ptr = row, *str = n;
  ull *arrPtr = arr;

  while (*ptr) {
    if (*ptr >= '0' && *ptr <= '9')
      *str++ = *ptr;
    else {
      *str = '\0';
      const int len = strlen(n);
      if (len > 0) {
        char *endPtr;
        ull tmp = strtoull(n, &endPtr, 10);

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
int createMap(ull map[ROW][COL][3], char matrix[ROW][COL], int rows) {
  // we can start from row 2
  int mapRow = 0, mapCol = 1;
  for (int row = 2; row <= rows; row++) {
    if (row < rows) {
      if (matrix[row][0] >= '0' && matrix[row][0] <= '9') {
        getNumbersFromRow(map[mapRow][mapCol++], &matrix[row][0]);
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

int processNumberForEachMap(Number *seeds, int seedsLen, ull map[ROW][COL][3],
                            int mapLen) {
  for (int row = 0; row < mapLen; row++) {
    // reset all founded to 0
    for (Number *tmpSeeds = seeds; tmpSeeds < seeds + seedsLen; tmpSeeds++) {
      tmpSeeds->founded = 0;
    }

    for (int j = 1; j < map[row][0][0]; j++) {

      ull range = map[row][j][2];
      ull destination = map[row][j][0];
      ull origin = map[row][j][1];
      ull maxOrigin = origin + range - 1;

      int tmpLen = seedsLen;
      for (Number *ptr = seeds; ptr < seeds + tmpLen; ptr++) {
        if (ptr->max < origin || ptr->min > maxOrigin || ptr->min > maxOrigin ||
            ptr->founded)
          continue;
        // range uguali funzione
        if (ptr->min >= origin && ptr->max <= maxOrigin) {
          ptr->min = (ptr->min - origin) + destination;
          ptr->max = ptr->min + ptr->range - 1;
          ptr->range = ptr->max - ptr->min + 1;
          ptr->founded = 1;
        } else if (ptr->min < origin && ptr->max <= maxOrigin) {
          Number tmp;
          tmp.min = ptr->min;
          tmp.max = origin - 1;
          tmp.range = tmp.max - tmp.min + 1;
          tmp.founded = 0;

          *(seeds + seedsLen++) = tmp;

          ptr->min = destination;
          ptr->range = ptr->range > tmp.range ? ptr->range - tmp.range
                                              : tmp.range - ptr->range;
          ptr->max = ptr->min + ptr->range - 1;
          ptr->founded = 1;
        } else if (ptr->min < origin && ptr->max > maxOrigin) {
          Number tmp1;
          tmp1.min = ptr->min;
          tmp1.max = origin - 1;
          tmp1.range = tmp1.max - tmp1.min + 1;
          tmp1.founded = 0;

          *(seeds + seedsLen++) = tmp1;

          Number tmp2;
          tmp2.min = maxOrigin + 1;
          tmp2.max = ptr->max;
          tmp2.range = tmp2.max - tmp2.min + 1;
          tmp2.founded = 0;

          *(seeds + seedsLen++) = tmp2;

          ull rangeTmps = tmp1.range + tmp2.range;
          ptr->min = destination;
          ptr->range = ptr->range - rangeTmps;
          ptr->max = destination + ptr->range - 1;
          ptr->founded = 1;
        } else if (ptr->min >= origin && ptr->max > maxOrigin) {
          // caso errato
          Number tmp;
          tmp.min = maxOrigin + 1;
          tmp.max = ptr->max;
          tmp.range = tmp.max - tmp.min + 1;
          tmp.founded = 0;

          *(seeds + seedsLen++) = tmp;

          ptr->min = (ptr->min - origin) + destination;
          ptr->max = destination + range - 1;
          ptr->range = ptr->max - ptr->min + 1;
          ptr->founded = 1;
        }
      }
    }
  }

  return seedsLen;
}

int main() {
  int rows = readFile(matrix);
  // printData(matrix, rows);

  ull seeds[64];
  int seedsLen = getNumbersFromRow(seeds, matrix[0]);
  if (seedsLen == -1)
    return -1;

  // create struct, seeds len always even
  Number newSeeds[ROW], *numPtr = newSeeds;
  int newSeedsLen = 0;
  for (ull *ptr = seeds; ptr < seeds + seedsLen; ptr += 2, numPtr++) {
    Number number;
    number.min = *ptr;
    number.max = *ptr + *(ptr + 1) - 1;
    number.range = *(ptr + 1);
    number.founded = 0;
    *numPtr = number;
    newSeedsLen++;
  }

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

  ull mainMin = newSeeds[0].min;
  printf("%llu %llu %llu\n", newSeeds[0].min, newSeeds[0].max,
         newSeeds[0].range);
  int len = processNumberForEachMap(newSeeds, newSeedsLen, map, mapLen);
  for (int j = 1; j < len; j++) {
    printf("%llu %llu %llu\n", newSeeds[j].min, newSeeds[j].max,
           newSeeds[0].range);
    if (newSeeds[j].min < mainMin) {
      mainMin = newSeeds[j].min;
    }
  }

  printf("\nminimo %llu\n", mainMin);

  return 0;
}
