#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(long long input[])
{
  FILE* fp = fopen("aoc11.txt", "r");

  int idx = 0;

  while (fscanf(fp, "%lld", &input[idx]) != EOF)
    {
      idx++;
      fscanf(fp, "%*c");
    }

  fclose(fp);

  return idx;
}

#include "intcode.h"

int es1()
{
  long long code[100000] = { 0 };
  int len = readinput(code);

  long long inputs[] = { 0 };
  int inputs_len = sizeof(inputs)/sizeof(inputs[0]);

  int ip = 0;
  int base = 0;
  int retcode = INT_MIN;

  int count = 0;

  int grid[101*101];
  int posx = 50;
  int posy = 50;
  int dir = 0; // UP

  for (int i=0; i<101*101; i++)
    grid[i] = -1;


  while (retcode != 0)
    {
      inputs[0] = grid[101*posx + posy];
      if (inputs[0] < 0)
        inputs[0] = 0;

      // g_print("(%d, %d, %d) ", posx, posy, dir);

      long long newv = intcode_exec_simple(code, len, inputs, inputs_len, &ip, &retcode, &base);
      long long newdir = intcode_exec_simple(code, len, inputs, 0, &ip, &retcode, &base);

      // g_print(" %lld %lld ", newv, newdir);

      if (grid[101*posx + posy] < 0)
        {
          count++;
          // g_print("NEW (%d, %d) -> %lld\n", posx, posy, newv);
        }
      // else
      //   g_print("(%d, %d) -> %lld\n", posx, posy, newv);

      grid[101*posx + posy] = newv;

      dir = newdir ?
              (dir == 3 ? 0 : dir + 1) :
              (dir == 0 ? 3 : dir - 1);

      if (dir == 0)
        posy--;
      else if (dir == 2)
        posy++;
      else if (dir == 1)
        posx++;
      else //if (dir == 3)
        posx--;
    }

  return count;
}

char* es2()
{
long long code[100000] = { 0 };
  int len = readinput(code);

  long long inputs[] = { 0 };
  int inputs_len = sizeof(inputs)/sizeof(inputs[0]);

  int ip = 0;
  int base = 0;
  int retcode = INT_MIN;

  int grid[101*101];
  int posx = 50;
  int posy = 50;
  int dir = 0; // UP

  for (int i=0; i<101*101; i++)
    grid[i] = -1;
  
  grid[101*posx + posy] = 1;  // start from a white

  while (retcode != 0)
    {
      inputs[0] = grid[101*posx + posy];
      if (inputs[0] < 0)
        inputs[0] = 0;

      long long newv = intcode_exec_simple(code, len, inputs, inputs_len, &ip, &retcode, &base);
      long long newdir = intcode_exec_simple(code, len, inputs, 0, &ip, &retcode, &base);

      grid[101*posx + posy] = newv;

      dir = newdir ?
              (dir == 3 ? 0 : dir + 1) :
              (dir == 0 ? 3 : dir - 1);

      if (dir == 0)
        posy--;
      else if (dir == 2)
        posy++;
      else if (dir == 1)
        posx++;
      else //if (dir == 3)
        posx--;
    }

  char* ret = g_new0(char, 102*101);
  int idx = 0;

  for (int y=0; y<101; y++)
    {
      gboolean skip = TRUE;

      for (int x=0; x<101; x++)
        if (grid[101*x + y] != -1)
          {
            skip = FALSE;
            break;
          }

      if (skip)
        continue;

      for (int x=0; x<101; x++)
        ret[idx++] = grid[101*x + y] == 1 ? '*' : ' ';

      ret[idx++] = '\n';
    }

  return ret;
}

int main()
{
  printf("1 -> %d\n", es1());
  {
    char* code = es2();
    printf("2 ->\n%s\n", code);
    g_free(code);
  }
  return 0;
}
