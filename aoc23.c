#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"


long long es1()
{
  intcode_machine m[50] = { 0 };

  for (int i=0; i<50; i++)
    {
      m[i] = intcode_machine_create("aoc23.txt");
      intcode_machine_enqueue(&m[i], i);
    }

  while (1)
    {
      for (int i=0; i<50; i++)
        if (m[i].output_len >= 3)
          {
            long long addr = m[i].output[0];
            long long x = m[i].output[1];
            long long y = m[i].output[2];

            for (int k=0; k<INPUT_LEN-4; k++)
              m[i].output[k] = m[i].output[k+3];
            
            m[i].output_len -= 3;

            // g_print("MSG %2d -> %2lld:   (%lld, %lld)\n", i, addr, x, y);

            if (addr == 255)
              return y;
            else if (addr < 50)
              {
                intcode_machine_enqueue(&m[addr], x);
                intcode_machine_enqueue(&m[addr], y);
              }
            else
              g_print("Warning, output to %lld:   (%lld, %lld)\n", addr, x, y);
          }

      for (int i=0; i<50; i++)
        {
          if (m[i].input_idx == m[i].input_len)
            {
              m[i].input_idx = 0;
              m[i].input_len = 1;
              m[i].inputs[0] = -1;
            }

          intcode_machine_execute(&m[i]);
        }
    }

  return 0;
}

int es2()
{
  intcode_machine m[50] = { 0 };

  for (int i=0; i<50; i++)
    {
      m[i] = intcode_machine_create("aoc23.txt");
      intcode_machine_enqueue(&m[i], i);
    }

  long long lastx = INT_MIN;
  long long lasty[2] = { INT_MIN, INT_MIN };

  while (1)
    {
      for (int i=0; i<50; i++)
        if (m[i].output_len >= 3)
          {
            long long addr = m[i].output[0];
            long long x = m[i].output[1];
            long long y = m[i].output[2];

            for (int k=0; k<INPUT_LEN-4; k++)
              m[i].output[k] = m[i].output[k+3];
            
            m[i].output_len -= 3;

            // g_print("MSG %2d -> %2lld:   (%lld, %lld)\n", i, addr, x, y);

            if (addr == 255)
              {
                lastx = x;
                lasty[0] = y;
              }
            else if (addr < 50)
              {
                intcode_machine_enqueue(&m[addr], x);
                intcode_machine_enqueue(&m[addr], y);
              }
            else
              g_print("Warning, output to %lld:   (%lld, %lld)\n", addr, x, y);
          }
        
      gboolean isidle = TRUE;

      for (int i=0; i<50; i++)
        if (m[i].retcode != -1)
          {
            isidle = FALSE;
            break;
          }

      if (isidle && lastx != INT_MIN && lasty[0] != INT_MIN)
        {
          intcode_machine_enqueue(&m[0], lastx);
          intcode_machine_enqueue(&m[0], lasty[0]);
          // g_print("idle send %lld\n", lasty[0]);

          if (lasty[0] == lasty[1])
            return lasty[0];

          lasty[1] = lasty[0];
        }

      for (int i=0; i<50; i++)
        {
          if (m[i].input_idx == m[i].input_len)
            {
              m[i].input_idx = 0;
              m[i].input_len = 1;
              m[i].inputs[0] = -1;
            }

          intcode_machine_execute(&m[i]);
        }
    }

  return 0;
}

int main()
{
  printf("1 -> %lld\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
