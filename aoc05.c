#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(int input[])
{
  FILE* fp = fopen("aoc05.txt", "r");

  int idx = 0;

  while (fscanf(fp, "%d", &input[idx]) != EOF)
    {
      idx++;
      fscanf(fp, "%*c");
    }

  fclose(fp);

  return idx;
}

int get(int array[], int idx, int mode)
{
  if (mode == 0)
    return array[idx];
  else
      return idx;
}

void set(int array[], int idx, int value, int mode)
{
  if (mode == 0)
    array[array[idx]] = value;
  else
    g_print("ERROR cant set in immediate mode\n");
}

int go(int array[], int len, int input)
{
  int idx = 0;
  int out = 0;

  while (idx < len)
    {
      // g_print("IPX=%d op=%d\t", idx, array[idx]);
      int mode1 = 0;
      int mode2 = 0;
      int mode3 = 0;
      int opcode = array[idx];

      if (opcode > 99)
        {
          mode1 = opcode / 100;
          mode2 = opcode / 1000;
          mode3 = opcode / 10000;

          opcode %= 100;
          mode1 %= 10;
          mode2 %= 10;
          mode3 %= 10;
        }
      // g_print("modes= %d %d %d (%d)\t", mode3, mode2, mode1, opcode);
      // if (opcode == 1 || opcode == 2 || opcode == 7 || opcode == 8)
      //   g_print("parms %d %d %d\n", array[idx+1], array[idx+2], array[idx+3]);
      // else if (opcode == 3 || opcode == 4)
      //   g_print("parms %d\n", array[idx+1]);
      // else if (opcode == 5 || opcode == 6)
      //   g_print("parms %d %d\n", array[idx+1], array[idx+2]);
      // else
      //   g_print("parms\n" );

      if (opcode == 1) {
        set(array, idx+3, get(array, array[idx + 1], mode1) + get(array, array[idx + 2], mode2), mode3);
        idx += 4;
      } else if (opcode == 2) {
        set(array, idx+3, get(array, array[idx + 1], mode1) * get(array, array[idx + 2], mode2), mode3);
        idx += 4;
      } 
      else if (opcode == 3) {
        // g_print("// executing set at %d\n", idx);
        set(array, idx + 1, input, mode1);
        idx += 2;
      }
      else if (opcode == 4) {
        out = get(array, array[idx + 1], mode1);
        idx += 2;
      }

      else if (opcode == 5) {
        if (get(array, array[idx + 1], mode1) != 0)
          idx = get(array, array[idx + 2], mode2);
        else
          idx += 3;
      }
      else if (opcode == 6) {
        if (get(array, array[idx + 1], mode1) == 0)
          idx = get(array, array[idx + 2], mode2);
        else
          idx += 3;
      }
      else if (opcode == 7) {
        set(array, idx+3, get(array, array[idx + 1], mode1) < get(array, array[idx + 2], mode2) ? 1 : 0, mode3);
        idx += 4;
      }
      else if (opcode == 8) {
        set(array, idx+3, get(array, array[idx + 1], mode1) == get(array, array[idx + 2], mode2) ? 1 : 0, mode3);
        idx += 4;
      }

      else if (opcode == 99) {
        return out;
      } else {
        g_print("Unknown opcode @%d = %d\n", idx, array[idx]);
        return out;
      }
    }

  g_print("end reach\n");
  return -1;
}

int try(int array[], int len, int input)
{
  int newone[len];
  memcpy(newone, array, len*sizeof(int));

  return go(newone, len, input);
}

int es1()
{
  int array[10000];
  int len = readinput(array);

  return try(array, len, 1);
}

int es2()
{
  int array[10000];
  int len = readinput(array);

  return try(array, len, 5);
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
