#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 100
#define DIGITS                                                                 \
  ((char const *[]){"zero", "one", "two", "three", "four", "five", "six",      \
                    "seven", "eight", "nine"})
#define ROW 10

char getChar(int index) {
  switch (index) {
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  case 3:
    return '3';
  case 4:
    return '4';
  case 5:
    return '5';
  case 6:
    return '6';
  case 7:
    return '7';
  case 8:
    return '8';
  default:
    return '9';
  }
}

int compareString(char *ptr) {
  for (int row = 0; row < ROW; row++) {
    for (int i = 3; i <= 6; i++) {
      int isValid = strncmp(ptr, DIGITS[row], i);
      if (!isValid) {
        return row;
      }
    }
  }
  return -1;
}

int findFirstAndLastNumber(char *line) {
  char first = '\0', last = '\0', n;

  int i = 0;
  while ((n = line[i++]) != '\0') {

    // remove else if you want res of part 1
    if (n >= '0' && n <= '9') {
      // assegnazione first and last
      if (!first && !last) {
        first = last = n;
      } else
        last = n;
    } else {
      int compared = compareString(&line[i - 1]);

      if (compared == -1)
        continue;

      if (!first && !last) {
        first = last = getChar(compared);
      } else {
        last = getChar(compared);
      }
    }
  }

  char res[2 + 1];
  res[0] = first;
  res[1] = last;
  res[2] = '\0';

  return atoi(res);
}

int main() {
  FILE *ptr;
  ptr = fopen("input.txt", "r");

  char line[LEN];
  int total = 0;

  while (fgets(line, LEN, ptr)) {
    total += findFirstAndLastNumber(line);
  }

  // close file
  fclose(ptr);

  printf("%d\n", total);

  return 0;
}
