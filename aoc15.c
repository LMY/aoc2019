#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

gboolean inverses(long long a, long long b)
{
  return (a == 1 && b == 2) || (a == 2 && b == 1) ||
         (a == 3 && b == 4) || (a == 4 && b == 3);
}

int explore(long long ocode[], int code_len, int oip, int obase, long long seq[], int seq_len)
{
  int ret = -1;

  for (int i=1; i<=4; i++)
    if (seq_len == 0 || !inverses(seq[seq_len-1], i))
      {
        int ip = oip;
        int retcode = -1;
        int base = obase;
        long long inputs[1] = { i };

        long long code[2000];
        memcpy(code, ocode, code_len*sizeof(long long));

        long long try = intcode_exec_simple(code, code_len, inputs, 1, &ip, &retcode, &base);

        if (try == 0)
          continue;
        else if (try == 1)
          {
            // g_print("len %d, found %d\n", seq_len, i);
            seq[seq_len] = i;
            int newt = explore(code, code_len, ip, base, seq, seq_len+1);

            if (newt >= 0)
              ret = newt;
          }
        else if (try == 2)
          {
            // g_print("FOUND");
            // seq_out(seq, seq_len);
            return seq_len;
          }
      }

  return ret;
}

int es1()
{
  long long code[2000] = { 0 };
  int len = intcode_read("aoc15.txt", code);

  long long seq[2000] = { 0 };

  return explore(code, len, 0, 0, seq, 0) + 1;
}

int es2()
{
  long long code[2000] = { 0 };
  int code_len = intcode_read("aoc15.txt", code);

  int matrix[1000*1000] = { 1 };
  int posx = 500;
  int posy = 500;

  for (int i=0; i<1000*1000; i++)
    matrix[i] = 1;

  int ip = 0;
  int base = 0;

  for (int steps = 0; steps<1000000; steps++)
    {
      long long inputs[1] = { (rand()%4)+1 };
      int retcode;

      long long try = intcode_exec_simple(code, code_len, inputs, 1, &ip, &retcode, &base);

      if (try != 0)
        {
          if (inputs[0] == 1)
            posy--;
          else if (inputs[0] == 2)
            posy++;
          else if (inputs[0] == 3)
            posx--;
          else if (inputs[0] == 4)
            posx++;

          matrix[1000*(posy)+(posx)] = (try == 2 ? 2 : 0);
        }
    }

  // FILE* fp = fopen("maxe.txt", "w");

  // for (int y=1; y<999; y++)
  //   {
  //     for (int x=1; x<999; x++)
  //       fprintf(fp, "%c", matrix[1000*y+x] == 0 ? ' ' : matrix[1000*y+x] == 1 ? '#' : 'O');
      
  //     fprintf(fp, "\n");
  //   }

  // fclose(fp);

  for (int t = 0;; t++)
    {
      int colored = 0;

      int newmat[1000*1000];

      for (int y=1; y<999; y++)
        for (int x=1; x<999; x++)
          if (matrix[1000*y+x] == 1)
            newmat[1000*y+x] = 1;
          else
            {
              newmat[1000*y+x] =  matrix[1000*(y)+(x)] == 2 || matrix[1000*(y)+(x+1)] == 2 || matrix[1000*(y)+(x-1)] == 2 || 
                                matrix[1000*(y+1)+(x)] == 2 || matrix[1000*(y-1)+(x)] == 2 ? 2 : 0;

              if (newmat[1000*y+x] == 2 && matrix[1000*y+x] != 2)
                colored++;
            }

      if (colored == 0)
        return t;

      memcpy(matrix, newmat, sizeof(newmat));
    }

  return 0;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
