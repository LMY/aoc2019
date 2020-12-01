#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(int input[])
{
  FILE* fp = fopen("aoc02.txt", "r");

  int idx = 0;

  while (fscanf(fp, "%d", &input[idx]) != EOF)
    {
      idx++;
      fscanf(fp, "%*c");
    }

  fclose(fp);

  return idx;
}

void go(int array[], int len)
{
  int idx = 0;

  while (idx < len)
    if (array[idx] == 1) {
      // printf("%d = %d   ->  %d = %d + %d \n", idx, array[idx], array[idx + 3], array[idx + 1], array[idx + 2]);

      array[array[idx + 3]] = array[array[idx + 1]] + array[array[idx + 2]];
      idx += 4;
    } else if (array[idx] == 2) {
      // printf("%d = %d   ->  %d = %d * %d \n", idx, array[idx], array[idx + 3], array[idx + 1], array[idx + 2]);

      array[array[idx + 3]] = array[array[idx + 1]] * array[array[idx + 2]];
      idx += 4;
    } else if (array[idx] == 99) {
      // printf("%d = %d\n", idx, array[idx]);

      return;
    } else {
      printf("Unknown opcode @%d = %d\n", idx, array[idx]);
      return;
    }

  printf("end reach\n");
}

int try(int array[], int len, int pos1, int pos2)
{
  int newone[len];
  memcpy(newone, array, len*sizeof(int));

  newone[1] = pos1;
  newone[2] = pos2;

  go(newone, len);

  return newone[0];
}

int es1()
{
  int array[10000];
  int len = readinput(array);
  return try(array, len, 12, 2);
}

int es2()
{
  int array[10000];
  int len = readinput(array);

  for (int y=0; y<100; y++)
    for (int x=0; x<100; x++)
        if (try(array, len, y, x) == 19690720)
            return 100*y +x;

  return -1;
}


int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
