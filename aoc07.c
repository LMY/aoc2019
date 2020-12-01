#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(int input[])
{
  FILE* fp = fopen("aoc07.txt", "r");

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

int intcode_exec(int array[], int len, int input[], int input_len, int* ip, int* retcode)
{
  int out = 0;

  int input_idx = 0;

  while (*ip < len)
    {
      int mode1 = 0;
      int mode2 = 0;
      int mode3 = 0;
      int opcode = array[*ip];

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

      if (opcode == 1) {
        set(array, *ip+3, get(array, array[*ip + 1], mode1) + get(array, array[*ip + 2], mode2), mode3);
        *ip += 4;
      } else if (opcode == 2) {
        set(array, *ip+3, get(array, array[*ip + 1], mode1) * get(array, array[*ip + 2], mode2), mode3);
        *ip += 4;
      } 
      else if (opcode == 3) {
        if (input_idx >= input_len)
          {
            g_print("Too many sets: %d\n", input_idx);
            *retcode = -1;
            return -1;
          }

        set(array, *ip + 1, input[input_idx++], mode1);
        *ip += 2;
      }
      else if (opcode == 4) {
        out = get(array, array[*ip + 1], mode1);
        *ip += 2;
        *retcode = 1;
        return out;
      }

      else if (opcode == 5) {
        if (get(array, array[*ip + 1], mode1) != 0)
          *ip = get(array, array[*ip + 2], mode2);
        else
          *ip += 3;
      }
      else if (opcode == 6) {
        if (get(array, array[*ip + 1], mode1) == 0)
          *ip = get(array, array[*ip + 2], mode2);
        else
          *ip += 3;
      }
      else if (opcode == 7) {
        set(array, *ip+3, get(array, array[*ip + 1], mode1) < get(array, array[*ip + 2], mode2) ? 1 : 0, mode3);
        *ip += 4;
      }
      else if (opcode == 8) {
        set(array, *ip+3, get(array, array[*ip + 1], mode1) == get(array, array[*ip + 2], mode2) ? 1 : 0, mode3);
        *ip += 4;
      }

      else if (opcode == 99) {
        *retcode = 0;
        return out;
      } else {
        g_print("Unknown opcode @%d = %d\n", *ip, array[*ip]);
        *retcode = -3;
        return out;
      }
    }

  g_print("end reach\n");
  *retcode = -2;
  return -1;
}

int intcode(int array[], int len, int input[], int input_len)
{
  int newone[len];
  memcpy(newone, array, len*sizeof(int));

  int idx = 0;
  int retcode;
  return intcode_exec(newone, len, input, input_len, &idx, &retcode);
}


int intcode_ampl(int code[], int code_len, int input, int phases[], int phases_len)
{
  int inputs[2] = { 0, input };

  for (int i=0; i<phases_len; i++)
    {
      inputs[0] = phases[i];
      inputs[1] = intcode(code, code_len, inputs, 2);
    }

  return inputs[1];
}

void permutation(int vals[], int len, int k, int using[])
{
  for (int i=0; i<len; i++)
    vals[i] = INT_MIN;

  int rem_len = len;

  for (int i=0; i<len; i++)
    {
      int pos = k % rem_len;
      k /= rem_len;
      rem_len--;

      int cx = 0;
      for (int a=0; a<len; a++)
        if (vals[a] == INT_MIN)
            if (cx++ == pos)
              {
                vals[a] = using[i];
                break;
              }
    }
}


int es1()
{
  int code[10000];
  int code_len = readinput(code);

  int max = INT_MIN;

  int perm[5];
  int using[] = { 0, 1, 2, 3, 4 };

  for (int i=0; i<5*4*3*2*1; i++)
    {
      permutation(perm, 5, i, using);

      int value = intcode_ampl(code, code_len, 0, perm, 5);
      
      if (value > max)
        max = value;
    }

  return max;
}

int es2()
{
  int code[10000];
  int code_len = readinput(code);

  // int code[] = { 3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10 };
  // int code_len = sizeof(code)/sizeof(code[0]);

  int max = INT_MIN;

  int perm[5];
  int using[] = { 5, 6, 7, 8, 9 };

  for (int i=0; i<5*4*3*2*1; i++)
    {
      int mcode[5][10000];
      for (int k=0; k<5; k++)
        memcpy(mcode[k], code, sizeof(int)*code_len);

      int ips[5] = { 0 };
      permutation(perm, 5, i, using);

      int ilen = 2;
      int inputs[2] = { 0, 0 };
      int last_iter = INT_MIN;

      while (1)
        {
          int retcode[5];

          for (int j = 0; j < 5; j++)
            {
              inputs[0] = ilen == 2 ? perm[j] : inputs[1];
              inputs[1] = intcode_exec(mcode[j], code_len, inputs, ilen, &ips[j], &retcode[j]);
            }
  
          ilen = 1;

          gboolean interrupt = TRUE;
          for (size_t j=0; j<sizeof(retcode)/sizeof(retcode[0]); j++)
            if (retcode[j] != 0)
              interrupt = FALSE;

          if (interrupt)
            break;

          last_iter = inputs[1];
        }
      
      if (last_iter > max)
        max = last_iter;
    }

  return max;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
