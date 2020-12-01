#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(long long input[])
{
  FILE* fp = fopen("aoc09.txt", "r");

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

long long es1()
{
  // long long code[] = { 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 };
  // int len = sizeof(code)/sizeof(code[0]);
  
  long long code[100000] = { 0 };
  int len = readinput(code);

  long long inputs[] = { 1 };
  int inputs_len = sizeof(inputs)/sizeof(inputs[0]);

  int ip = 0;
  int base = 0;
  int retcode = INT_MIN;

  long long outv = 0;
  while (retcode != 0)
    {
      long long newv = intcode_exec_simple(code, len, inputs, inputs_len, &ip, &retcode, &base);

      if (retcode != 0)
        outv = newv;
    }

  return outv;
}

int es2()
{
  long long code[100000] = { 0 };
  int len = readinput(code);

  long long inputs[] = { 2 };
  int inputs_len = sizeof(inputs)/sizeof(inputs[0]);

  int ip = 0;
  int base = 0;
  int retcode = INT_MIN;

  long long outv = 0;
  while (retcode != 0)
    {
      long long newv = intcode_exec_simple(code, len, inputs, inputs_len, &ip, &retcode, &base);

      if (retcode != 0)
        outv = newv;
    }

  return outv;
}

int main()
{
  printf("1 -> %lld\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
