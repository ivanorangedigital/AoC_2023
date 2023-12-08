#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 4
#define ROW 100
#define COL 255
#define FILE_NAME "input.txt"

typedef struct {
  int time;
  int distance;
} Data;

int readFile(char matrix[ROW][COL]) {
  FILE *fptr = fopen(FILE_NAME, "r");
  char(*ptr)[COL] = matrix;

  char line[COL];
  while (fgets(line, COL, fptr))
    strncpy(*ptr++, line, COL);

  return ptr - matrix;
}

int getNumbersFromRow(int *arr, char *row) {
  char n[LEN];
  char *ptr = row, *str = n;
  int *arrPtr = arr;

  while (*ptr) {
    if (*ptr >= '0' && *ptr <= '9')
      *str++ = *ptr;
    else {
      *str = '\0';
      const int len = strlen(n);
      if (len > 0) {
        int tmp = atoi(n);

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

int main() {
  char matrix[ROW][COL];
  int rows = readFile(matrix);

  Data map[LEN];
  /*
   * <time> 7 15 30
   * <distance> 9 40 200
   */
  for (int row = 0; row < rows; row++) {
    int tmp[LEN];
    getNumbersFromRow(tmp, matrix[row]);
    if (row == 0)
      for (int i = 0; i < LEN; i++)
        map[i].time = tmp[i];
    else
      for (int i = 0; i < LEN; i++)
        map[i].distance = tmp[i];
  }

  int prd = 1;
  for (Data *ptr = map; ptr < map + LEN; ptr++) {
    float timeToPress = (float)ptr->distance / ptr->time;
    if (timeToPress - (int)timeToPress > 0)
      timeToPress = (int)timeToPress + 1;
    else
      timeToPress = (int)timeToPress;

    int minTimeToPress = timeToPress, counter = 0;
    while (minTimeToPress < ptr->time) {
      if ((ptr->time - minTimeToPress) * minTimeToPress > ptr->distance) {
        counter++;
      }
      minTimeToPress++;
    }
    prd *= counter;
  }

  printf("prodotto: %d\n", prd);
  return 0;
}
