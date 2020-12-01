#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_input(char input[])
{
  FILE* fp = fopen("aoc24.txt", "r");

  char line[128];
  int idx = 0;

  while (fscanf(fp, "%s", line) != EOF)
    for (int i=0; line[i]; i++)
      input[idx++] = line[i];

  input[idx] = '\0';

  fclose(fp);
}

void show_config(char input[])
{
  for (int y=0; y<5; y++)
    {
      for (int x=0; x<5; x++)
        g_print("%c", input[5*y+x]);

      g_print("\n");
    }
}

void evolve(char input[], char output[])
{
  for (int y=0; y<5; y++)
    for (int x=0; x<5; x++)
      {
        int num = (x>0 && input[5*y+(x-1)] == '#') + (x<4 && input[5*y+(x+1)] == '#') +
                  (y>0 && input[5*(y-1)+x] == '#') + (y<4 && input[5*(y+1)+x] == '#');
        
        if (input[5*y+x] == '#')
          output[5*y+x] = num == 1 ? '#' : '.';
        else
          output[5*y+x] = num == 1 || num == 2 ? '#' : '.'; 
      }
}

unsigned biodiversity(char config[])
{
  unsigned ret = 0;

  for (int i=0; i<25; i++)
    if (config[i] == '#')
      ret |= (1<<i);

  return ret;
}

unsigned es1()
{
  char config[26] = { 0 };
  read_input(config);
  
  char other[26] = { 0 };

  GHashTable* table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

  g_hash_table_insert(table, g_strdup(config), GINT_TO_POINTER(0));

  for (int i=1; ; i++)
    {     
      evolve(config, other);
      memcpy(config, other, sizeof(config));

      // g_print("%d\n", i);
      // show_config(config);
      // g_print("\n\n");      

      if (g_hash_table_lookup(table, config) != NULL)
        {
          // g_print("Found at %d, same as %d\n", i, GPOINTER_TO_INT(g_hash_table_lookup(table, config)));
          break;
        }
      
      g_hash_table_insert(table, g_strdup(config), GINT_TO_POINTER(i));
    }

  g_hash_table_destroy(table);

  return biodiversity(config);
}

#define LEVELS 411

gboolean are_valid_coords(int x, int y)
{
  return x >= 0 && x < 5 && y >= 0 && y < 5 && (x != 2 || y != 2);
}

void evolve2(char input[], char output[])
{
  for (int level = 0; level<LEVELS; level++)
    for (int y=0; y<5; y++)
      for (int x=0; x<5; x++)
        if (x != 2 || y != 2)
          {
            int num = 0;

            if (are_valid_coords(x-1, y))
              num += input[25*level + 5*y+(x-1)] == '#';
            if (are_valid_coords(x+1, y))
              num += input[25*level + 5*y+(x+1)] == '#';

            if (are_valid_coords(x, y-1))
              num += input[25*level + 5*(y-1)+x] == '#';
            if (are_valid_coords(x, y+1))
              num += input[25*level + 5*(y+1)+x] == '#';

            if (level < LEVELS-1)
              {
                if (y == 1 && x == 2)
                  num += (input[25*(level+1) + 5*0+(0)] == '#') + (input[25*(level+1) + 5*0+(1)] == '#') + (input[25*(level+1) + 5*0+(2)] == '#') +
                          (input[25*(level+1) + 5*0+(3)] == '#') + (input[25*(level+1) + 5*0+(4)] == '#');      

                if (y == 3 && x == 2)
                  num += (input[25*(level+1) + 5*4+(0)] == '#') + (input[25*(level+1) + 5*4+(1)] == '#') + (input[25*(level+1) + 5*4+(2)] == '#') +
                          (input[25*(level+1) + 5*4+(3)] == '#') + (input[25*(level+1) + 5*4+(4)] == '#');

                if (y == 2 && x == 1)
                  num += (input[25*(level+1) + 5*(0)+(0)] == '#') + (input[25*(level+1) + 5*(1)+(0)] == '#') + (input[25*(level+1) + 5*(2)+(0)] == '#') +
                        (input[25*(level+1) + 5*(3)+(0)] == '#') + (input[25*(level+1) + 5*(4)+(0)] == '#');
                  
                if (y == 2 && x == 3)
                  num += (input[25*(level+1) + 5*(0)+(4)] == '#') + (input[25*(level+1) + 5*(1)+(4)] == '#') + (input[25*(level+1) + 5*(2)+(4)] == '#') +
                        (input[25*(level+1) + 5*(3)+(4)] == '#') + (input[25*(level+1) + 5*(4)+(4)] == '#');
                }

            if (level > 0)
              {
                if (y == 0)
                  num += (input[25*(level-1) + 5*(1)+(2)] == '#');

                if (y == 4)
                  num += (input[25*(level-1) + 5*(3)+(2)] == '#');

                if (x == 0)
                  num += (input[25*(level-1) + 5*(2)+(1)] == '#');

                if (x == 4)
                  num += (input[25*(level-1) + 5*(2)+(3)] == '#');
              }

            if (input[25*level + 5*y+x] == '#')
              output[25*level + 5*y+x] = num == 1 ? '#' : '.';
            else
              output[25*level + 5*y+x] = num == 1 || num == 2 ? '#' : '.'; 
          }
}

unsigned alive(char config[], size_t config_len)
{
  unsigned count = 0;

  for (size_t i=0; i<config_len; i++)
    if (config[i] == '#')
      count++;

  return count;
}

unsigned es2()
{
  char config[25*LEVELS + 1] = { 0 };
  read_input(&config[25*(LEVELS/2)]);
  
  char other[25*LEVELS + 1] = { 0 };

  for (int i=0; i<200; i++)
    {     
      evolve2(config, other);
      memcpy(config, other, sizeof(config));

      // g_print("%d\t%u\n", i+1, alive(config, sizeof(config)/sizeof(config[0])));

      // g_print("%d\n", i);
      // show_config(&config[25 * (LEVELS / 2)]);
      // g_print("\n\n");
    }

  return alive(config, sizeof(config)/sizeof(config[0]));
}

int main()
{
  printf("1 -> %u\n", es1());
  printf("2 -> %u\n", es2());
  return 0;
}
