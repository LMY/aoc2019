#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592654

void readinput(gboolean input[], int* lenx, int* leny)
{
  FILE* fp = fopen("aoc10.txt", "r");

  char line[1024];

  *leny = 0;

  while (fscanf(fp, "%s", line) != EOF)
    {
      int lenx1 = strlen(line);
      if (lenx1 > 0)
        {
          *lenx = lenx1;

          for (int x=0; x<*lenx; x++)
            input[(*lenx)*(*leny)+x] = line[x] == '#' ? TRUE : FALSE;
        
          (*leny)++;
        }
    }

  fclose(fp);
}

gboolean ison(int x0, int y0, int x1, int y1, int xp, int yp)
{
  // if ((x0 == xp && y0 == yp) || (x1 == xp && y1 == yp))
  //   return TRUE;

  int dx = x1-x0;
  int dy = y1-y0;

  int dx0 = x0-xp;
  int dy0 = y0-yp;

  int dx1 = x1-xp;
  int dy1 = y1-yp;

  if (dx * dy1 != dy * dx1)
    return FALSE;

  if (dx * dx + dy * dy < dx1 * dx1 + dy1 * dy1)
    return FALSE;

  if (dx * dx + dy * dy < dx0 * dx0 + dy0 * dy0)
    return FALSE;

  return TRUE;
}

gboolean isvalid(gboolean input[], int lenx, int leny, int x1, int y1, int x0, int y0)
{
  for (int y=0; y<leny; y++)
    for (int x=0; x<lenx; x++)
      if (input[lenx*y+x])
        if ((x != x1 || y != y1) && (x != x0 || y != y0))
          if (ison(x0, y0, x1, y1, x, y))
          {
            // g_print("(%d, %d) -- (%d, %d) -- (%d, %d)\n", x0, y0, x, y, x1, y1);
            return FALSE;
          }

  return TRUE;
}

int count(gboolean input[], int lenx, int leny, int px, int py)
{
  int cnt = 0;

  for (int y=0; y<leny; y++)
    for (int x=0; x<lenx; x++)
      if (input[lenx*y+x])
        if (x != px || y != py)
          if (isvalid(input, lenx, leny, px, py, x, y))
            cnt++;

  return cnt;
}

int es1()
{
  int lenx, leny;
  gboolean input[100000];
  
  readinput(input, &lenx, &leny);

  int max = INT_MIN;

  for (int y=0; y<leny; y++)
    for (int x=0; x<lenx; x++)
      if (input[lenx*y+x])
        {
          int this = count(input, lenx, leny, x, y);

          // g_print("(%d, %d) = %d\n", x, y, this);
          if (this > max)
            max = this;
        }

  return max;
}

typedef struct 
{
  int x;
  int y;

  float theta;

} asteroid;

gint point_compare(gpointer a, gpointer b)
{
  asteroid* p0 = (asteroid*) a;
  asteroid* p1 = (asteroid*) b;

  return  p0->theta < p1->theta  ? -1 :
          p0->theta == p1->theta ?  0 :
                                   +1;
}

float normangle(float x)
{
  if (x < 0)
    x += 2*PI;

  return x;
}

int es2()
{
  int lenx, leny;
  gboolean input[100000];
  
  readinput(input, &lenx, &leny);

  int max = INT_MIN;
  int px = 0;
  int py = 0;

  for (int y=0; y<leny; y++)
    for (int x=0; x<lenx; x++)
      if (input[lenx*y+x])
        {
          int this = count(input, lenx, leny, x, y);

          // g_print("(%d, %d) = %d\n", x, y, this);
          if (this > max)
            {
              max = this;
              px = x;
              py = y;
            }
        }

  int cnt = 0;
  int ret = INT_MIN;
  // g_print("center is (%d, %d)\n", px, py);
  
  do {
    GSList* list = NULL;

    gboolean input2[100000];
    memcpy(input2, input, sizeof(input));

    for (int y=0; y<leny; y++)
      for (int x=0; x<lenx; x++)
        if (input[lenx*y+x])
          if (x != px || y != py)
            if (isvalid(input, lenx, leny, px, py, x, y))
              {
                asteroid* ast = g_new0(asteroid, 1);
                ast->x = x;
                ast->y = y;
                ast->theta = normangle(atan2(x-px, py-y));

                input2[lenx*y+x] = FALSE;

                list = g_slist_prepend(list, ast);
              }
              
    memcpy(input, input2, sizeof(input));              

    list = g_slist_sort(list, (GCompareFunc) point_compare);

    for (GSList* iter = list; iter; iter = iter->next)
      {
        if (++cnt == 200)
        {
          asteroid* ast = (asteroid*) iter->data;
          ret = 100*ast->x + ast->y;
        }

        // g_print("%f (%d, %d)\n", ((asteroid*) iter->data)->theta, ((asteroid*) iter->data)->x, ((asteroid*) iter->data)->y);
      }

    // for tests
    // if (list == NULL)
    //     break;
    // else
    //   g_print("iter: %d\n", g_slist_length(list));

    g_slist_free_full(list, g_free);
  } while (ret == INT_MIN);

  return ret;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
