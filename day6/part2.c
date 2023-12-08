#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 4
#define ROW 100
#define COL 255
#define FILE_NAME "input.txt"

typedef unsigned long long ull;
typedef struct {
  ull time;
  ull distance;
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

  Data map[1];
  Data *ptr = map;
  ptr->time = 0;
  ptr->distance = 0;
  /*
   * <time> 7 15 30
   * <distance> 9 40 200
   */
  for (int row = 0; row < rows; row++) {
    int tmp[LEN];
    getNumbersFromRow(tmp, matrix[row]);
    if (row == 0)
      for (int i = 0; i < LEN; i++) {
        int time = tmp[i];
        if (ptr->time == 0) {
          ptr->time = time;
          continue;
        }

        int multipler = 1;
        while (multipler < time) {
          multipler *= 10;
        };
        ptr->time *= multipler;
        ptr->time += time;
        printf("%llu\n", ptr->time);
      }
    else
      for (int i = 0; i < LEN; i++) {
        int distance = tmp[i];
        if (ptr->distance == 0) {
          ptr->distance = distance;
          continue;
        }

        int multipler = 1;
        while (multipler < distance) {
          multipler *= 10;
        };
        ptr->distance *= multipler;
        ptr->distance += distance;
        printf("%llu\n", ptr->distance);
      }
  }

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

  printf("counter %d\n", counter);
  return 0;
}
