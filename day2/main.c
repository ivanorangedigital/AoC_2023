#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define LEN 265
#define RED 12
#define BLUE 14
#define GREEN 13
#define COLORS ((char const *[]){"red", "blue", "green"})

int findGameId(char *line, int *index) {
  // max 100
  // numbers strat from 5 to max 7
  char *numbers = (char *)malloc(1 * sizeof(char));
  int i = 0;

  for (char *ptr = line; ptr < line + strlen(line); ptr++) {
    if (*ptr == ':') {
      *index = (ptr - line) + 2;
      break;
    }

    if (*ptr >= '0' && *ptr <= '9') {
      numbers[i++] = *ptr;
      numbers = realloc(numbers, (i + 1) * sizeof(char));
    }
  }

  numbers = realloc(numbers, (i + 1) * sizeof(char));
  numbers[i] = '\0';

  return atoi(numbers);
}

// index 0 == red, 1 == blue, 2 == green
int compareString(char *ptr) {
  for (int row = 0; row < 3; row++) {
    for (int i = 3; i <= 5; i++) {
      int isValid = strncmp(ptr, COLORS[row], i);
      if (!isValid) {
        return row;
      }
    }
  }
  return -1;
}

// function for 2nd part
int findMax(int *ptr, int *mainData, char *color) {
  int ln = ptr - mainData;
  int max = mainData[0];
  for (int *pt = mainData + 1; pt < mainData + ln; pt++) {

    if (*pt > max)
      max = *pt;
  }
  // printf("%s max: %d\n", color, max);
  return max;
}

int verifyIfPlay(int gameId, char *line, int k) {
  char sets[10][64] = {'\0'};
  int setIndex = 0, subIndex = 0;
  for (char *ptr = line + k; ptr < line + strlen(line); ptr++) {
    // we strart from line + k (partenza dalla selezione colore)
    if (*ptr != ' ' && *ptr != ';')
      sets[setIndex][subIndex++] = *ptr;

    if (*ptr == ';' || *ptr == '\n') {
      sets[setIndex][subIndex] = '\0';
      // printf("%s\n", sets[setIndex]);
      subIndex = 0;
      setIndex++;
    }
  }
  // part 2
  //   [0][0]          [1][0]           [2][0]
  // [ [red, 0, ...9], [blue, 0, ...9], [green, 0, ...9] ]
  static int colors[3][10] = {{0}, {1}, {2}};
  // need 3 pointer
  static int *_red, *_blue, *_green;
  _red = colors[0];
  _blue = colors[1];
  _green = colors[2];
  // -----

  // check input validity
  // some no need for part two
  int red = 0, green = 0, blue = 0, strIndex = 0, isValid = 1;
  char str[64];

  for (int row = 0; row < 10; row++) {
    // toggle comments for part 1
    if (sets[row][0] == '\0')
      break;
    // if (sets[row][0] == '\0' || !isValid)
    //   return isValid;

    // part 1
    // red = green = blue = 0;
    for (int i = 0; i < 64; i++) {
      if (sets[row][i] == '\0')
        break;

      char n = sets[row][i];
      if (n >= '0' && n <= '9') {
        str[strIndex++] = n;
      } else if (n != ',') {
        str[strIndex] = '\0';
        strIndex = 0;
        int colorType = compareString(&sets[row][i]);

        switch (colorType) {
        case 0:
          // 2nd part
          *_red++ = atoi(str);
          // -------
          // part 1
          // red += atoi(str);
          break;
        case 1:
          // 2nd part
          *_blue++ = atoi(str);
          // -------
          // part 1
          // blue += atoi(str);
          break;
        case 2:
          // 2nd part
          *_green++ = atoi(str);
          // -------
          // part 1
          // green += atoi(str);
          break;
        }

        strcpy(str, "");
        // uncomment for part 1
        //   if (red <= RED && green <= GREEN && blue <= BLUE) {
        //     isValid = 1;
        //   } else {
        //     return 0;
        //   }
      }
    }
  }

  // part 2
  // printf("%d\n", findMax(_red, colors[0]));
  int maxRed = findMax(_red, colors[0], "rosso");
  int maxBlue = findMax(_blue, colors[1], "blue");
  int maxGreen = findMax(_green, colors[2], "verde");

  int prd = maxRed * maxBlue * maxGreen;

  return prd;
  // -----

  // part 1
  // return isValid;
}

int main() {
  FILE *ptr = fopen("input.txt", "r");

  int sum = 0;

  char line[LEN];
  while (fgets(line, LEN, ptr)) {
    int indexStart;
    int gameId = findGameId(line, &indexStart);
    // part 1
    // int isValid = verifyIfPlay(gameId, line, indexStart);
    // if (isValid)
    //   sum += gameId;

    // part 2
    int prd = verifyIfPlay(gameId, line, indexStart);
    sum += prd;
  }
  printf("total: %d\n", sum);

  fclose(ptr);

  return 0;
}
