#include <glib-2.0/glib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
  char what[128];
  long long quantity;
} req;

typedef struct
{
  req from[128];
  int from_len;

  req to;
} recipe;

int read_recipes(recipe recipes[])
{
  FILE* fp = fopen("aoc14.txt", "r");

  int idx = 0;
  char line[1024];

  recipes[idx].from_len = 0;

  while (fscanf(fp, "%s", line) != EOF)
    {
      if (strcmp(line, "=>") == 0)
        {
          fscanf(fp, "%lld %s", &recipes[idx].to.quantity, recipes[idx].to.what);
          idx++;
          recipes[idx].from_len = 0;
        }
      else
        {
          recipes[idx].from[recipes[idx].from_len].quantity = atoi(line);
          fscanf(fp, "%s", recipes[idx].from[recipes[idx].from_len].what);

          if (recipes[idx].from[recipes[idx].from_len].what[strlen(recipes[idx].from[recipes[idx].from_len].what)-1] == ',')
            recipes[idx].from[recipes[idx].from_len].what[strlen(recipes[idx].from[recipes[idx].from_len].what)-1] = '\0';

          recipes[idx].from_len++;
        }
    }

  fclose(fp);

  return idx;
}

int res_find(req need[], int len, char* what)
{
  for (int i=0; i<len; i++)
    if (strcmp(need[i].what, what) == 0)
      return i;

  return -1;
}

void res_add(req need[], int* len, char* what, long long howmany)
{
  int idx = res_find(need, *len, what);

  if (idx >= 0)
    need[idx].quantity += howmany;
  else
    {
      strcpy(need[*len].what, what);
      need[*len].quantity = howmany;
      (*len)++;
    }
}

long long howmuchfor(recipe recipes[], int len, long long howmuch)
{
  req need[1000];
  int need_len = 0;

  strcpy(need[need_len].what, "FUEL");
  need[need_len].quantity = howmuch;
  need_len++;

  while (1)
    {
      // for (int i=0; i<need_len; i++)
      //   if (need[i].quantity != 0)
      //     g_print("%s %lld\n", need[i].what, need[i].quantity);
      // g_print("\n");

      int idx = -1;
      for (int i=0; i<need_len; i++)
        if (strcmp(need[i].what, "ORE") != 0 && need[i].quantity > 0)
          {
            idx = i;
            break;
          }

      if (idx < 0)
        break;

      for (int i=0; i<len; i++)
        if (strcmp(recipes[i].to.what, need[idx].what) == 0)
          {
            long long howmany = ceil((double) need[idx].quantity / recipes[i].to.quantity);
            need[idx].quantity -= howmany*recipes[i].to.quantity;

            for (int k=0; k<recipes[i].from_len; k++)
              res_add(need, &need_len, recipes[i].from[k].what, recipes[i].from[k].quantity * howmany);
          }
    }

  return need[res_find(need, need_len, "ORE")].quantity;
}

long long es1()
{
  recipe recipes[100];
  int len = read_recipes(recipes);

  return howmuchfor(recipes, len, 1);
}

long long es2()
{
  recipe recipes[100];
  int len = read_recipes(recipes);

  long long amount = 1000000000000;

  long long min = 1;
  long long max = amount;

  while (min < max-1)
    {
      long long mean = (min+max)/2;
      long long qmean = howmuchfor(recipes, len, mean);

      // g_print("%lld; %lld]  f(%lld) = %lld\n", min, max, mean, qmean);

      if (qmean > amount)
        max = mean;
      else if (qmean < amount)
        min = mean;
    }

  return min;
}

int main()
{
  printf("1 -> %lld\n", es1());
  printf("2 -> %lld\n", es2());
  return 0;
}
