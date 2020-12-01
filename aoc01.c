#include <stdio.h>
#include <stdlib.h>

int req(int x)
{
  int val = (x/3) - 2;
  return val >= 0 ? val : 0;
}

int reqn(int x[], int n)
{
  int ret = 0;
  int this = 0;

  for (int i=0; i<n; i++)
  {
    this = req(x[i]);
    // printf("%d) f(%d) = %d\n", i, x[i], this);
    ret += this;
  }

  return ret;
}

int readinput(int input[])
{
  FILE* fp = fopen("aoc01.txt", "r");

  int idx = 0;

  while (fscanf(fp, "%d", &input[idx]) != EOF)
    idx++;

  fclose(fp);

  return idx;
}

int es1()
{
  int array[10000];
  int len = readinput(array);
  return reqn(array, len);
}

int es2()
{
  int x[10000];
  int n = readinput(x);

  int ret = 0;
  int this = 0;

  for (int i=0; i<n; i++)
  {
    this = x[i];
    while ((this = req(this)) > 0)
      ret += this;
  }

  return ret;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
