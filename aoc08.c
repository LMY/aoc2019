#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readinput(int input[])
{
  FILE* fp = fopen("aoc08.txt", "r");

  int idx = 0;
  char c;

  while (fscanf(fp, "%c", &c) != EOF)
    input[idx++] = c - '0';

  fclose(fp);

  return idx;
}

int es1()
{
  int LENX = 25;
  int LENY = 6;

  int image[1000000];
  int len = readinput(image);

  int nlayers = len/LENX/LENY;

  int best0count = INT_MAX;
  int n1pn2 = -1;

  for (int i=0; i<nlayers; i++)
    {
      int count0 = 0;
      int count1 = 0;
      int count2 = 0;

      for (int idx=i*LENX*LENY; idx<(i+1)*LENX*LENY; idx++)
        if (image[idx] == 0)
          count0++;
        else if (image[idx] == 1)
          count1++;
        else if (image[idx] == 2)
          count2++;

      if (count0 < best0count)
        {
          best0count = count0;
          n1pn2 = count1 * count2;
        }
    }


  return n1pn2;
}

char* es2()
{
  int LENX = 25;
  int LENY = 6;

  int image[1000000];
  int len = readinput(image);

  int nlayers = len/LENX/LENY;

  char* ret = g_malloc(LENX*LENY);

  for (int y=0; y<LENY; y++)
    for (int x=0; x<LENX; x++)
      ret[y*LENX+x] = -1;
    
  for (int i=0; i<nlayers; i++)
    for (int y=0; y<LENY; y++)
      for (int x=0; x<LENX; x++)
      {
        int base = i*LENX*LENY + y*LENX + x;

        if (ret[y*LENX + x] == -1)
          {
            if (image[base] == 0 || image[base] == 1)
              ret[y*LENX + x] = image[base];
          }
      }
      

  return ret;
}

int main()
{
  printf("1 -> %d\n", es1());
  {
    char* img = es2();
    printf("2 ->\n");
    for (int y=0; y<6; y++)
    {
      for (int x=0; x<25; x++)
        {
          g_print("%c", img[y*25 + x] ? '*' : ' ');
        }
      g_print("\n");
    }
    g_free(img);
  }
  return 0;
}
