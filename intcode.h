#include <glib-2.0/glib.h>

int intcode_read(char* filename, long long input[])
{
  FILE* fp = fopen(filename, "r");

  int idx = 0;

  while (fscanf(fp, "%lld", &input[idx]) != EOF)
    {
      idx++;
      fscanf(fp, "%*c");
    }

  fclose(fp);

  return idx;
}

static long long intcode_get(long long array[], long long idx, int mode, int curr)
{
  if (mode == 0)
    return array[idx];
  else if (mode == 1)
    return idx;
  else if (mode == 2)
    return array[idx + curr];
  else
    {
      g_print("ERROR GET invalid mode: %d\n", mode);
      return 0;
    }    
}

static gboolean intcode_set(long long array[], long long idx, long long value, int mode, int curr)
{
  if (mode == 0)
    {
      array[array[idx]] = value;
      return TRUE;
    }
  else if (mode == 1)
    {
      g_print("ERROR cant SET in immediate mode\n");
      return FALSE;
    }
  else if (mode == 2)
    {
      array[array[idx] + curr] = value;
      return TRUE;
    }
  else
    {
      g_print("ERROR SET invalid mode: %d\n", mode);
      return FALSE;
    }    
}

long long intcode_exec(long long array[], int len, long long input[], int input_len, int* input_idx, int* ip, int* retcode, int *base)
{
  long long out = 0;


  while (*ip < len)
    {
      int mode1 = 0;
      int mode2 = 0;
      int mode3 = 0;
      int opcode = array[*ip];
//  g_print("IP=%d, opcode=%d\n", *ip, opcode);
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
        if (!intcode_set(array, *ip+3, intcode_get(array, array[*ip + 1], mode1, *base) + intcode_get(array, array[*ip + 2], mode2, *base), mode3, *base))
          {
            *retcode = -3;
            return -3;
          }
        *ip += 4;
      } else if (opcode == 2) {
        if (!intcode_set(array, *ip+3, intcode_get(array, array[*ip + 1], mode1, *base) * intcode_get(array, array[*ip + 2], mode2, *base), mode3, *base))
          {
            *retcode = -3;
            return -3;
          }

        *ip += 4;
      } 
      else if (opcode == 3) {
        if (*input_idx >= input_len)
          {
            // g_print("Too many intcode_sets: %d\n", *input_idx);
            *retcode = -1;
            return -1;
          }

        if (!intcode_set(array, *ip + 1, input[(*input_idx)++], mode1, *base))
          {
            *retcode = -3;
            return -3;
          }

        *ip += 2;
      }
      else if (opcode == 4) {
        out = intcode_get(array, array[*ip + 1], mode1, *base);
        *ip += 2;
        *retcode = 1;
        return out;
      }

      else if (opcode == 5) {
        if (intcode_get(array, array[*ip + 1], mode1, *base) != 0)
          *ip = intcode_get(array, array[*ip + 2], mode2, *base);
        else
          *ip += 3;
      }
      else if (opcode == 6) {
        if (intcode_get(array, array[*ip + 1], mode1, *base) == 0)
          *ip = intcode_get(array, array[*ip + 2], mode2, *base);
        else
          *ip += 3;
      }
      else if (opcode == 7) {
        if (!intcode_set(array, *ip+3, intcode_get(array, array[*ip + 1], mode1, *base) < intcode_get(array, array[*ip + 2], mode2, *base) ? 1 : 0, mode3, *base))
          {
            *retcode = -3;
            return -3;
          }

        *ip += 4;
      }
      else if (opcode == 8) {
        if (!intcode_set(array, *ip+3, intcode_get(array, array[*ip + 1], mode1, *base) == intcode_get(array, array[*ip + 2], mode2, *base) ? 1 : 0, mode3, *base))
          {
            *retcode = -3;
            return -3;
          }

        *ip += 4;
      }
      else if (opcode == 9) {
        *base += intcode_get(array, array[*ip + 1], mode1, *base);
        *ip += 2;
      }

      else if (opcode == 99) {
        *retcode = 0;
        return out;
      } else {
        g_print("Unknown opcode @%d = %lld\n", *ip, array[*ip]);
        *retcode = -3;
        return out;
      }
    }

  g_print("end reach\n");
  *retcode = -2;
  return -1;
}

long long intcode_exec_simple(long long array[], int len, long long input[], int input_len, int* ip, int* retcode, int *base)
{
  int input_idx = 0;
  return intcode_exec(array, len, input, input_len, &input_idx, ip, retcode, base);
}

typedef struct 
{
  #define INTCODE_CODE_LEN 1024*8
  long long code[INTCODE_CODE_LEN];
  int code_len;

  #define INPUT_LEN 128
  long long inputs[INPUT_LEN];
  int input_len;
  int input_idx;

  int ip;
  int base;

  int retcode;
  long long output[INPUT_LEN];
  int output_len;

} intcode_machine;

intcode_machine intcode_machine_create(char* filename)
{
  intcode_machine mac;
  memset(&mac, '\0', sizeof(intcode_machine));

  mac.code_len = intcode_read(filename, mac.code);

  if (mac.code_len >= INTCODE_CODE_LEN)
    g_print("ERROR: code is too large: %d\n", mac.code_len);

  return mac;
}

intcode_machine intcode_machine_clone(intcode_machine* mac)
{
  intcode_machine ret;
  memcpy(&ret, mac, sizeof(ret));
  return ret;
}

long long intcode_machine_execute(intcode_machine* mac)
{
  long long ret = intcode_exec(mac->code, mac->code_len, mac->inputs, mac->input_len, &mac->input_idx, &mac->ip, &mac->retcode, &mac->base);

  if (mac->output_len == INPUT_LEN - 1)
    g_print("ERROR: Output queue exausted!\n");

  if (mac->retcode >= 0)
    mac->output[mac->output_len++] = ret;
  return ret;
}

void intcode_machine_enqueue(intcode_machine* mac, long long x)
{
  if (mac->input_idx == mac->input_len)
    mac->input_idx = 0;

  if (mac->input_len >= INPUT_LEN - 1)
    {
      long long inputs[INPUT_LEN];
      memcpy(inputs, &mac[mac->input_idx], sizeof(long long)*(INPUT_LEN-mac->input_idx));
      memcpy(mac->inputs, inputs, sizeof(long long)*(INPUT_LEN-mac->input_idx));
      mac->input_len -= mac->input_idx;
      mac->input_idx = 0;
    }
  
  mac->inputs[mac->input_len++] = x;

  if (mac->input_idx >= INPUT_LEN - 1)
    g_print("ERROR: Input queue exausted!\n");
}
