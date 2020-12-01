#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

gboolean is_inside(int px, int py, long long origcode[], int len)
{
  int ip = 0;
  int base = 0;
  int retcode = -1;
  int input_idx = 0;
  long long inputs[] = { px, py };

  long long code[4000] = { 0 };
  memcpy(code, origcode, sizeof(code));
  
  long long ret = intcode_exec(code, len, inputs, 2, &input_idx, &ip, &retcode, &base);
  return (ret == 1);
}

int es1()
{
  long long origcode[4000] = { 0 };
  int len = intcode_read("aoc19.txt", origcode);

  int total = 0;

  for (int y=0; y<50; y++)
    for (int x=0; x<50; x++)
      if (is_inside(x, y, origcode, len) == 1)
        total++;

  return total;
}

int es2()
{
  long long origcode[4000] = { 0 };
  int len = intcode_read("aoc19.txt", origcode);

  for (int y=0;; y++)
    for (int x=0; x<=y; x++)
      {
        // g_print("%d, %d\n", x, y);

        gboolean okx = TRUE;

        for (int dx=0; dx<100; dx++)
          if (!is_inside(x+dx, y, origcode, len) || !is_inside(x, y+dx, origcode, len))
            {
              okx = FALSE;
              break;
            }

        if (okx)
          return x*10000+y;
      }

  // not reachable
  return 0;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
