#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x;
  int y;
  int z;
  
  int vx;
  int vy;
  int vz;

} point;

int setinput(point input[])
{
  int idx = 0;

  // input
  input[idx].x = 5;
  input[idx].y = 4;
  input[idx].z = 4;
  input[idx].vx = input[idx].vy = input[idx].vz = 0;
  idx++;
  
  input[idx].x = -11;
  input[idx].y = -11;
  input[idx].z = -3;
  input[idx].vx = input[idx].vy = input[idx].vz = 0;
  idx++;

  input[idx].x = 0;
  input[idx].y = 7;
  input[idx].z = 0;
  input[idx].vx = input[idx].vy = input[idx].vz = 0;
  idx++;

  input[idx].x = -13;
  input[idx].y = 2;
  input[idx].z = 10;
  input[idx].vx = input[idx].vy = input[idx].vz = 0;
  idx++;

  return idx;
}

void evolve_coord(int x0, int x1, int* s0, int* s1)
{
  if (x0 < x1)
    {
      (*s0)++;
      (*s1)--;
    }
  else if (x0 > x1)
    {
      (*s0)--;
      (*s1)++;
    }
}

void evolve(point points[], int len)
{
  for (int i=0; i<len; i++)
    for (int k=i+1; k<len; k++)
      {
        evolve_coord(points[i].x, points[k].x, &points[i].vx, &points[k].vx);
        evolve_coord(points[i].y, points[k].y, &points[i].vy, &points[k].vy);
        evolve_coord(points[i].z, points[k].z, &points[i].vz, &points[k].vz);
      }
      
  for (int i=0; i<len; i++)
    {
      points[i].x += points[i].vx;
      points[i].y += points[i].vy;
      points[i].z += points[i].vz;
    }
}

int energy1(point pos)
{
  int kin = abs(pos.x) + abs(pos.y) + abs(pos.z);
  int pot = abs(pos.vx) + abs(pos.vy) + abs(pos.vz);

  return kin * pot;
}

int energy(point points[], int len)
{
  int total = 0;

  for (int i=0; i<len; i++)
    total += energy1(points[i]);

  return total;
}

void out_situation(point points[], int len, int t)
{
  g_print("After %d steps:\n", t);

  for (int i=0; i<len; i++)
    g_print("pos=<x=%d, y=%d, z=%d>, vel=<x=%d, y=%d, z=%d>\t\t E = %d\n", points[i].x, points[i].y, points[i].z, points[i].vx, points[i].vy, points[i].vz, energy1(points[i]));

  g_print("\n");
}

int es1()
{
  point points[10];
  int len = setinput(points);

  // out_situation(points, speeds, len, 0);

  for (int i=1; i<=1000; i++)
    {
      evolve(points, len);

      // out_situation(points, speeds, len, i+1);
    }

  return energy(points, len);
}

void extract_situation(point points[], int len, int output[], int what)
{
  for (int i=0; i<len; i++)
    {
      output[2*i + 0] = what == 0 ? points[i].x : what == 1 ? points[i].y : points[i].z;
      output[2*i + 1] = what == 0 ? points[i].vx : what == 1 ? points[i].vy : points[i].vz;
    }
}

gpointer extract_situation_alloc(point points[], int len, int what)
{
  int* output = g_new(int, len*2);
  extract_situation(points, len, output, what);
  return output;
}

guint func_hash(gconstpointer ptr)
{
  int * p = (int*) ptr;

  return p[0] ^ p[1] ^ p[2] ^ p[3] ^ p[4] ^ p[5] ^ p[6] ^ p[7];
}

gboolean func_equal(gconstpointer p1, gconstpointer p2)
{
  int * p = (int*) p1;
  int * q = (int*) p2;

  for (int i=0; i<8; i++)
    if (p[i] != q[i])
      return FALSE;

  return TRUE;
}

long long lcd(long long n1, long long n2)
{
  long long gcd;
  for (long long i = 1; i <= n1 && i <= n2; ++i)
    if (n1 % i == 0 && n2 % i == 0)
      gcd = i;
  return (n1 * n2) / gcd;
}

long long es2()
{
  point points[10];
  int len = setinput(points);

  GHashTable* table[3];
  long long ret[3] = { -1, -1, -1 };

  for (int c=0; c<3; c++)
    {
      table[c] = g_hash_table_new_full(func_hash, func_equal, g_free, NULL);
      g_hash_table_insert(table[c], extract_situation_alloc(points, len, c), GINT_TO_POINTER(1));
    }

  for (int i=1; ret[0] < 0 || ret[1] < 0 || ret[2] < 0; i++)
    {
      evolve(points, len);

      for (int c=0; c<3; c++)
        if (ret[c] < 0)
          {
            gpointer sit = extract_situation_alloc(points, len, c);
            gpointer ptr = g_hash_table_lookup(table[c], sit);

            if (ptr == NULL)
                g_hash_table_insert(table[c], sit, GINT_TO_POINTER(i+1));
            else
              {
                // g_print("repeting for coordinate=%d, @t=%d t1=%lld\n", c, i, ret[c]);
                g_free(sit);
                ret[c] = i;
              }
          } 
    }

  for (int c=0; c<3; c++)
    g_hash_table_destroy(table[c]);

  return lcd(lcd(ret[0], ret[1]), ret[2]);
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %lld\n", es2());
  return 0;
}
