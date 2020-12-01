#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

long long es1()
{
  long long code[100000] = { 0 };
  int len = intcode_read("aoc13.txt", code);

  int ip = 0;
  int retcode = -1;
  int base = 0;

  long long count = 0;

  while (retcode != 0)
    {
      /*long long x =*/ intcode_exec_simple(code, len, NULL, 0, &ip, &retcode, &base);
      /*long long y =*/ intcode_exec_simple(code, len, NULL, 0, &ip, &retcode, &base);
      long long t = intcode_exec_simple(code, len, NULL, 0, &ip, &retcode, &base);

      if (t == 2)
        count++;
    }

  return count;
}

long long es2()
{
  long long code[100000] = { 0 };
  int len = intcode_read("aoc13.txt", code);

  int ip = 0;
  int retcode = -1;
  int base = 0;

  long long score = 0;

  code[0] = 2;
  long long inputs[1] = { 0 };

  long long ballx = 0;
  long long ladx = 0;

  while (retcode != 0)
    {
      inputs[0] = ballx>ladx?+1 : ballx<ladx?-1 : 0;

      long long x = intcode_exec_simple(code, len, inputs, 1, &ip, &retcode, &base);
      long long y = intcode_exec_simple(code, len, inputs, 1, &ip, &retcode, &base);
      long long t = intcode_exec_simple(code, len, inputs, 1, &ip, &retcode, &base);

      if (x == -1 && y == 0)
        score = t;
      else
        {
          if (t == 3)
            ladx = x;
          else if (t == 4)
            ballx = x;
        }
    }

  return score;
}

int main()
{
  printf("1 -> %lld\n", es1());
  printf("2 -> %lld\n", es2());
  return 0;
}
