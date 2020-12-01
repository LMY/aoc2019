#include <glib-2.0/glib.h>
#include <stdio.h>

GHashTable* readinput()
{
  FILE* fp = fopen("aoc06.txt", "r");

  int idx = 0;
  char line[128];
  GHashTable* ret = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

  while (fscanf(fp, "%s", line) != EOF)
    {
      idx++;

      char* pos = strchr(line, ')');
      *pos = '\0';

      char* a = g_strdup(line);
      char* b = g_strdup(pos+1);

      // if (g_hash_table_lookup(ret, b) != NULL)
      //   g_print("%d Already present: [%s]\n", idx, b);
      // else
      //   {
          g_hash_table_insert(ret, b, a);
        // }
    }

  fclose(fp);

  return ret;
}

int count_element(GHashTable* table, char* who)
{
  int cnt = 0;
  char* cur = who;

  while ( cur )
    {
      cur = g_hash_table_lookup(table, cur);
      cnt++;
    }

  return cnt - 1;
}

GSList* path(GHashTable* table, char* who)
{
  GSList* ret = NULL;

  char* cur = who;

  while ( cur )
    {
      cur = g_hash_table_lookup(table, cur);

      if (cur != NULL)
        ret = g_slist_prepend(ret, cur);
    }


  return g_slist_reverse(ret);
}

// void echo_path(GSList* list)
// {
//   int i = 0;

//   for (GSList* iter = list; iter; iter = iter->next)
//     g_print("%d %s\n", i++, (char*)iter->data);
// }

int es1()
{
  GHashTable* table = readinput();
  
  int count = 0;
  GList* keys =  g_hash_table_get_keys(table);

  for (GList* iter = keys; iter; iter = iter->next)
    {
      char* key = iter->data;
      count += count_element(table, key);
    }

  g_list_free(keys);
  g_hash_table_destroy(table);

  return count;
}

int es2()
{
  GHashTable* table = readinput();

  GSList* san = path(table, "SAN");
  GSList* you = path(table, "YOU");

  int ret = -1;

  int cy = 0;
  for (GSList* y=you; y && ret < 0; y=y->next)
    {
      int cx = 0;
      char* elementy = (char*) y->data;

      for (GSList* x=san; x && ret < 0; x=x->next)
        {
          char* elementx = (char*) x->data;

          if (strcmp(elementx, elementy) == 0)  // found common ancestor
              ret = cx + cy;

          cx++;
        }

      cy++;
    }

  g_slist_free(san);
  g_slist_free(you);
  g_hash_table_destroy(table);

  return ret;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
