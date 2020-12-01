#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXK 35

void read_input(char matrix[], int* lenx, int* leny)
{
  FILE* fp = fopen("aoc18.txt", "r");

  *lenx = 0;
  *leny = 0;
  int idx = 0;

  char line[1024];

  while (fscanf(fp, "%s", line) != EOF)
    {
      for (int i=0; line[i]; i++)
        matrix[idx++] = line[i];
      
      int newlen = strlen(line);
      if (newlen > *lenx)
        *lenx = newlen;
      
      (*leny)++;
    }

  fclose(fp);
}

typedef struct
{
  int x;
  int y;
} point;

point INVALID = { -1, -1 };


point matrix_find(char* matrix, int lenx, int leny, char who)
{
  for (int y=0; y<leny; y++)
      for (int x=0; x<lenx; x++)
        if (matrix[lenx*y+x] == who)
          {
            point p = { x, y };
            return p;
          }
  
  return INVALID;
}

gboolean simple_has_key(char* keys, int key_len, char key)
{
  for (int i=0; i<key_len; i++)
    if (keys[i] == key)
      return TRUE;

  return FALSE;
}

int count_keys(char* matrix, int lenx, int leny)
{
  int cnt = 0;

  for (int y=0; y<leny; y++)
    for (int x=0; x<lenx; x++)
      if (matrix[lenx*y+x] >= 'a' && matrix[lenx*y+x] <= 'z')
        cnt++;

  return cnt;
}

typedef struct
{
  int len;

  char needed[MAXK];
  int needed_len;
} reach;

void reach_matrix_rec(char* matrix, int lenx, int leny, point from, reach* reachmatrix)
{
  reach* this = &reachmatrix[lenx * from.y + from.x];

  for (int dir=1; dir<=4; dir++)
    {
      int dx = dir == 2 ? +1 : dir == 4 ? -1 : 0;
      int dy = dir == 3 ? +1 : dir == 1 ? -1 : 0;

      if (from.y + dy < 0 || from.y + dy >= leny || from.x + dx < 0 || from.x + dx >= lenx)
        continue;
        
      char xxx = matrix[lenx*(from.y + dy)+(from.x + dx)];
      if (xxx == '#')
        continue;

      point newfrom = { from.x + dx, from.y + dy };

      reach* that = &reachmatrix[lenx * (from.y + dy) + (from.x + dx)];

      if (that->len < this->len + 1)
        continue;

      if (that->len == this->len + 1 &&
       that->needed_len < this->needed_len)
        {
          char now[128] = { 0 };
          char then[128] = { 0 };

          strncpy(now, this->needed, this->needed_len);
          strncpy(then, that->needed, that->needed_len);

          g_print("OMG.  %s  |   %s |\n", now, then);
          continue;
        }

      that->len = this->len+1;
      that->needed_len = this->needed_len;
      strncpy(that->needed, this->needed, this->needed_len);

      if (xxx >= 'A' && xxx <= 'Z')
        {
          char key = xxx-'A'+'a';

          if (!simple_has_key(that->needed, that->needed_len, key))
            that->needed[that->needed_len++] = key;
        }

      reach_matrix_rec(matrix, lenx, leny, newfrom, reachmatrix);
    }
}

void reach_matrix(char* matrix, int lenx, int leny, point from, reach* reachmatrix)
{
  reachmatrix[lenx* from.y + from.x].len = 0;
  reachmatrix[lenx* from.y + from.x].needed_len = 0;

  reach_matrix_rec(matrix, lenx, leny, from, reachmatrix);
}

gboolean have_required(char reqs[], char reqs_len, gboolean visited[])
{
  for (int r=0; r<reqs_len; r++)
    if (!visited[reqs[r]-'a'+1])
      return FALSE;

  return TRUE;
}

typedef struct 
{
  gboolean visited[MAXK];
  int from;
  int cost_till_here;
  
  char sequence[MAXK];
  int sequence_len;

} data;

guint func_hash(gconstpointer key)
{
  gboolean* array = (gboolean*) key;

  return (((array[0] & 1) << 0) |
              ((array[1] & 1) << 1) |
              ((array[2] & 1) << 2) |
              ((array[3] & 1) << 3) |
              ((array[4] & 1) << 4) |
              ((array[5] & 1) << 5) |
              ((array[6] & 1) << 6) |
              ((array[7] & 1) << 7) |
              ((array[8] & 1) << 8) |
              ((array[9] & 1) << 9) |
              ((array[9] & 1) << 10) |
              ((array[10] & 1) << 11) |
              ((array[11] & 1) << 12) |
              ((array[12] & 1) << 13) |
              ((array[13] & 1) << 14) |
              ((array[14] & 1) << 15) |
              ((array[15] & 1) << 16) |
              ((array[16] & 1) << 17) |
              ((array[17] & 1) << 18) |
              ((array[18] & 1) << 19) |
              ((array[19] & 1) << 20) |
              ((array[20] & 1) << 21) |
              ((array[21] & 1) << 22) |
              ((array[22] & 1) << 23) |
              ((array[23] & 1) << 24) |
              ((array[24] & 1) << 25) |
              ((array[25] & 1) << 26) |
              ((array[26] & 1) << 27)) ^ array[MAXK-1];
}

gboolean func_equal(gconstpointer a, gconstpointer b)
{
  return memcmp(a, b, MAXK*sizeof(gboolean)) == 0;
}

void add_entry(GHashTable* table, gboolean state[], int value, int from)
{
  gboolean* ret = g_malloc(MAXK*sizeof(gboolean));
  memcpy(ret, state, MAXK*sizeof(gboolean));

  ret[MAXK-1] = from;

  g_hash_table_insert(table, ret, GINT_TO_POINTER(value));
}

void update_entry(GHashTable* table, gboolean state[], int value, int from)
{
  state[MAXK-1] = from;
  gpointer xxx = g_hash_table_lookup(table, state);
  if (xxx == NULL || value < GPOINTER_TO_INT(xxx))
    add_entry(table, state, value, from);
}

gboolean should_skip(GHashTable* table, gboolean state[], int from, int cost_till_here)
{
  state[MAXK-1] = from;

  gpointer xxx = g_hash_table_lookup(table, state);
  if (xxx != NULL && cost_till_here > GPOINTER_TO_INT(xxx))
    return TRUE;

  return FALSE;
}

int visit(reach mat[], int keyn)
{
  data stack[10240] = { 0 };
  int stack_len = 0;

  stack[0].visited[0] = TRUE;
  stack[0].from = 0;
  stack[0].cost_till_here = 0;
  stack[0].sequence[0] = '\0';
  stack_len++;

  char bestseq[MAXK];
  int best = INT_MAX;

  GHashTable* table = g_hash_table_new_full(func_hash, func_equal, g_free, NULL);

  while (stack_len > 0)
    {
      data this = stack[stack_len-1];
      stack_len--;

      if (this.cost_till_here > best)
        {
          update_entry(table, this.visited, this.cost_till_here, this.from);
          continue;
        }

      if (this.sequence_len == keyn)
        {
          if (this.cost_till_here < best)
            {
              best = this.cost_till_here;
              strcpy(bestseq, this.sequence);
            }

          update_entry(table, this.visited, this.cost_till_here, this.from);
          continue;
        }
      
      if (should_skip(table, this.visited, this.from, this.cost_till_here))
        continue;

      for (int i=1; i<keyn + 1; i++)
        if (!this.visited[i])
          {
            reach* target = &mat[(keyn+1)*i + this.from];

            if (!have_required(target->needed, target->needed_len, this.visited))
              continue;

            stack[stack_len] = this;
            stack[stack_len].from = i;
            stack[stack_len].cost_till_here += target->len;
            stack[stack_len].visited[i] = TRUE;
            stack[stack_len].sequence[stack[stack_len].sequence_len++] = 'a'+(i-1);
            stack[stack_len].sequence[stack[stack_len].sequence_len] = '\0';

            stack_len++;
          }

      update_entry(table, this.visited, this.cost_till_here, this.from);
    }

  g_hash_table_destroy(table);
  // g_print("sequence : [%s]\n", bestseq);
  return best;
}

typedef struct 
{
  gboolean visited[MAXK];
  int from[4];
  int cost_till_here;
  
  char sequence[MAXK];
  int sequence_len;

} data2;

guint func_hash2(gconstpointer key)
{
  gboolean* array = (gboolean*) key;

  return (((array[0] & 1) << 0) |
              ((array[1] & 1) << 1) |
              ((array[2] & 1) << 2) |
              ((array[3] & 1) << 3) |
              ((array[4] & 1) << 4) |
              ((array[5] & 1) << 5) |
              ((array[6] & 1) << 6) |
              ((array[7] & 1) << 7) |
              ((array[8] & 1) << 8) |
              ((array[9] & 1) << 9) |
              ((array[9] & 1) << 10) |
              ((array[10] & 1) << 11) |
              ((array[11] & 1) << 12) |
              ((array[12] & 1) << 13) |
              ((array[13] & 1) << 14) |
              ((array[14] & 1) << 15) |
              ((array[15] & 1) << 16) |
              ((array[16] & 1) << 17) |
              ((array[17] & 1) << 18) |
              ((array[18] & 1) << 19) |
              ((array[19] & 1) << 20) |
              ((array[20] & 1) << 21) |
              ((array[21] & 1) << 22) |
              ((array[22] & 1) << 23) |
              ((array[23] & 1) << 24) |
              ((array[24] & 1) << 25) |
              ((array[25] & 1) << 26) |
              ((array[26] & 1) << 27)) ^ array[MAXK-1] ^ array[MAXK-2] ^ array[MAXK-3] ^ array[MAXK-4];
}


void update_entry2(GHashTable* table, gboolean state[], int value, int from[])
{
  state[MAXK-1] = from[0];
  state[MAXK-2] = from[1];
  state[MAXK-3] = from[2];
  state[MAXK-4] = from[3];

  gpointer xxx = g_hash_table_lookup(table, state);
  if (xxx == NULL || value < GPOINTER_TO_INT(xxx))
    {
      gboolean* ret = g_malloc(MAXK*sizeof(gboolean));
      memcpy(ret, state, MAXK*sizeof(gboolean));
      g_hash_table_insert(table, ret, GINT_TO_POINTER(value));
    }
}

gboolean should_skip2(GHashTable* table, gboolean state[], int from[], int cost_till_here)
{
  state[MAXK-1] = from[0];
  state[MAXK-2] = from[1];
  state[MAXK-3] = from[2];
  state[MAXK-4] = from[3];

  gpointer xxx = g_hash_table_lookup(table, state);
  if (xxx != NULL && cost_till_here > GPOINTER_TO_INT(xxx))
    return TRUE;

  return FALSE;
}

gboolean have_required2(char reqs[], char reqs_len, gboolean visited[])
{
  for (int r=0; r<reqs_len; r++)
    if (!visited[reqs[r]-'a'+4])
      return FALSE;

  return TRUE;
}

int visit2(reach mat[], int keyn)
{
  data2 stack[10240] = { 0 };
  int stack_len = 0;

  stack[0].visited[0] = TRUE;
  stack[0].visited[1] = TRUE;
  stack[0].visited[2] = TRUE;
  stack[0].visited[3] = TRUE;
  stack[0].from[0] = 0;
  stack[0].from[1] = 1;
  stack[0].from[2] = 2;
  stack[0].from[3] = 3;
  stack[0].cost_till_here = 0;
  stack[0].sequence[0] = '\0';
  stack_len++;

  char bestseq[MAXK];
  int best = INT_MAX;

  GHashTable* table = g_hash_table_new_full(func_hash2, func_equal, g_free, NULL);

  while (stack_len > 0)
    {
      data2 this = stack[stack_len-1];
      stack_len--;

      if (this.cost_till_here > best)
        {
          update_entry2(table, this.visited, this.cost_till_here, this.from);
          continue;
        }

      if (this.sequence_len == keyn)
        {
          if (this.cost_till_here < best)
            {
              best = this.cost_till_here;
              strcpy(bestseq, this.sequence);
            }

          update_entry2(table, this.visited, this.cost_till_here, this.from);
          continue;
        }
      
      if (should_skip2(table, this.visited, this.from, this.cost_till_here))
        continue;

      if (this.sequence_len < 4)
        g_print("%s\n", this.sequence);

      for (int who=0; who<4; who++)
        {
          for (int i=4; i<keyn + 4; i++)
            if (!this.visited[i])
              {
                reach* target = &mat[(keyn+4)*i + this.from[who]];

                if (target->len == INT_MAX)
                  continue;

                if (!have_required2(target->needed, target->needed_len, this.visited))
                  continue;

                stack[stack_len] = this;
                stack[stack_len].from[who] = i;
                stack[stack_len].cost_till_here += target->len;
                stack[stack_len].visited[i] = TRUE;
                stack[stack_len].sequence[stack[stack_len].sequence_len++] = 'a'+(i-4);
                stack[stack_len].sequence[stack[stack_len].sequence_len] = '\0';

                stack_len++;
              }

          update_entry2(table, this.visited, this.cost_till_here, this.from);
        }
    }

  g_hash_table_destroy(table);
  g_print("sequence : [%s]\n", bestseq);
  return best;
}



int es1()
{
  char matrix[10240];
  int lenx;
  int leny;

  read_input(matrix, &lenx, &leny);

  point pos = matrix_find(matrix, lenx, leny, '@');
  matrix[lenx * pos.y + pos.x] = '.';
  
  int keyn = count_keys(matrix, lenx, leny);

  reach mat[MAXK*MAXK] = { 0 };

  for (int from=0; from<keyn+1; from++)
    {
      reach reachmat[10000];
      for (int i = 0; i < 10000; i++)
        reachmat[i].len = INT_MAX;
      
      point p0 = from == 0 ? pos : matrix_find(matrix, lenx, leny, 'a' + from -1);
      reach_matrix(matrix, lenx, leny, p0, reachmat);

      for (int to=0; to<keyn+1; to++)
        {
          point p1 = to == 0 ? pos : matrix_find(matrix, lenx, leny, 'a' + to -1);

          mat[(keyn+1) * from + to] = reachmat[lenx * p1.y + p1.x];
          mat[(keyn+1) * from + to].needed[mat[(keyn+1) * from + to].needed_len] = '\0';
        }
    }

  // DISPLAY
  // g_print("\t");
  // for (int to = 0; to < keyn + 1; to++)
  //   g_print("%c\t", to == 0 ? '0' : 'a' + to - 1);
  // g_print("\n");

  // for (int from = 0; from < keyn + 1; from++)
  // {
  //   g_print("%c\t", from == 0 ? '0' : 'a' + from - 1);

  //   for (int to = 0; to < keyn + 1; to++)
  //     g_print("%d\t", mat[(keyn + 1) * from + to].len);

  //   g_print("\n");
  // }

  // g_print("\n");

  // g_print("\t");
  // for (int to = 0; to < keyn + 1; to++)
  //   g_print("%c\t", to == 0 ? '0' : 'a' + to - 1);
  // g_print("\n");

  // for (int from = 0; from < keyn + 1; from++)
  // {
  //   g_print("%c\t", from == 0 ? '0' : 'a' + from - 1);

  //   for (int to = 0; to < keyn + 1; to++)
  //     g_print("%s\t", mat[(keyn + 1) * from + to].needed[0] == '\0' ? "-" : mat[(keyn + 1) * from + to].needed);

  //   g_print("\n");
  // }

  return visit(mat, keyn);
}

int es2()
{
  char matrix[10240];
  int lenx;
  int leny;

  read_input(matrix, &lenx, &leny);

  point pos = matrix_find(matrix, lenx, leny, '@');

  matrix[lenx * (pos.y - 1) + (pos.x-1)] = '.';
  matrix[lenx * (pos.y - 1) + (pos.x)] = '#';
  matrix[lenx * (pos.y - 1) + (pos.x+1)] = '.';

  matrix[lenx * (pos.y) + (pos.x-1)] = '#';
  matrix[lenx * (pos.y) + (pos.x)] = '#';
  matrix[lenx * (pos.y) + (pos.x+1)] = '#';

  matrix[lenx * (pos.y + 1) + (pos.x-1)] = '.';
  matrix[lenx * (pos.y + 1) + (pos.x)] = '#';
  matrix[lenx * (pos.y + 1) + (pos.x+1)] = '.';

  reach mat[MAXK*MAXK] = { 0 };

  int keyn = count_keys(matrix, lenx, leny);

  point posx[4] = { {pos.x-1, pos.y-1}, {pos.x+1, pos.y-1}, {pos.x-1, pos.y+1}, {pos.x+1, pos.y+1}};

  for (int from=0; from<keyn+4; from++)
    {
      reach reachmat[10000];
      for (int i = 0; i < 10000; i++)
        reachmat[i].len = INT_MAX;
      
      point p0 = from < 4 ? posx[from] : matrix_find(matrix, lenx, leny, 'a' + from -4);
      reach_matrix(matrix, lenx, leny, p0, reachmat);

      for (int to=0; to<keyn+4; to++)
        {
          point p1 = to < 4 ? posx[to] : matrix_find(matrix, lenx, leny, 'a' + to -4);
          mat[(keyn+4) * from + to] = reachmat[lenx * p1.y + p1.x];
          mat[(keyn+4) * from + to].needed[mat[(keyn+4) * from + to].needed_len] = '\0';
        }
    }

// DISPLAY
  // g_print("\t");
  // for (int to = 0; to < keyn + 4; to++)
  //   g_print("%c\t", to < 4 ? '0'+to : 'a' + to - 4);
  // g_print("\n");

  // for (int from = 0; from < keyn + 4; from++)
  // {
  //   g_print("%c\t", from < 4 ? '0'+from : 'a' + from - 4);

  //   for (int to = 0; to < keyn + 4; to++)
  //     if (mat[(keyn + 4) * from + to].len == INT_MAX)
  //       g_print("x\t");
  //     else
  //       g_print("%d\t", mat[(keyn + 4) * from + to].len);

  //   g_print("\n");
  // }

  // g_print("\n");

  // g_print("\t");
  // for (int to = 0; to < keyn + 4; to++)
  //   g_print("%c\t", to < 4 ? '0'+to : 'a' + to - 4);
  // g_print("\n");

  // for (int from = 0; from < keyn + 4; from++)
  // {
  //   g_print("%c\t", from < 4 ? '0'+from : 'a' + from - 4);

  //   for (int to = 0; to < keyn + 4; to++)
  //     g_print("%s\t", mat[(keyn + 4) * from + to].needed[0] == '\0' ? "-" : mat[(keyn + 4) * from + to].needed);

  //   g_print("\n");
  // }

  return visit2(mat, keyn);
}

int main()
{
  // printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
