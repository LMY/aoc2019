#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_input(char line[])
{
  FILE* fp = fopen("aoc16.txt", "r");

  fscanf(fp, "%s", line);

  fclose(fp);
}

void step(char input[], char output[])
{
  int len = strlen(input);

  for (int i=0; i<len; i++)
    input[i] -= '0';

  for (int i = 0; i < len; i++)
  {
    int total = 0;
    gboolean active = TRUE;

    for (int base=i; active && base<len; base+=4*(i+1))
      for (int k=0; k<=i; k++)
        {
          if (base+k >= len)
            {
              active = FALSE;
              break;
            }
          total += input[base + k];

          if (base+k+2*i+2 < len)
            total -= input[base+k+2*i+2];
        }

    output[i] = (total > 0 ? total : -total) % 10;
  }

  for (int i=0; i<len; i++)
    output[i] += '0';

  output[len] = '\0';
}

void step_from(char input[], int offset)
{
  int len = strlen(input);

  for (int i=offset; i<len; i++)
    input[i] -= '0';

  for (int i=len-2; i>=offset; i--)
    {
      input[i] += input[i+1];
      input[i] %= 10;
    }

  for (int i=offset; i<len; i++)
    input[i] += '0';
}

int calc_offset(char line[])
{
  int ret = 0;

  for (int i=0; i<7; i++)
    ret = (ret*10) + line[i]-'0';

  return ret;
}

char* es1()
{
  char line[1024];
  read_input(line);

  char res[1024];

  for (int i=1; i<=100; i++)
  {
    step(line, res);
    strcpy(line, res);
  }

  line[8] = '\0';

  return g_strdup(line);
}

char* es2()
{
  char line[1024];
  read_input(line);

  int offset = calc_offset(line);
  int len = strlen(line);

  char* signal = g_malloc0(10000*len + 1);

  for (int i=0; i<10000; i++)
    for (int k=0; k<len; k++)
      signal[i*len + k] = line[k];

  for (int i=1; i<=100; i++)
    step_from(signal, offset);

  signal[offset+8] = '\0';
  char* ret = g_strdup(&signal[offset]);

  g_free(signal);

  return ret;
}

int main()
{
  {
    char* res = es1();
    printf("1 -> %s\n", res);
    g_free(res);
  }
  {
    char* res = es2();
    printf("2 -> %s\n", res);
    g_free(res);
  }
  return 0;
}
