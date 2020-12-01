#include <stdio.h>
#include <glib-2.0/glib.h>

typedef struct 
{
  int x;
  int y;
} point;

GSList* toPoints(char* c)
{
  char** tokens = g_strsplit(c, ",", -1);

  GSList *ret = NULL;

  {
    point* pt = g_new0(point, 1);
    ret = g_slist_prepend(ret, pt);
  }

  point* oldpt = g_new0(point, 1);

  for (char**t = tokens; *t; t++)
    {
      int len = atoi(&(*t)[1]);
      point* newpt = g_new0(point, 1);
      newpt->x = oldpt->x;
      newpt->y = oldpt->y;

      if (*t[0] == 'U')
        newpt->y -= len;
      else if (*t[0] == 'D')
        newpt->y += len;
      else if (*t[0] == 'R')
        newpt->x += len;
      else if (*t[0] == 'L')
        newpt->x -= len;

      ret = g_slist_prepend(ret, newpt);
      oldpt->x = newpt->x;
      oldpt->y = newpt->y;
    }

  g_strfreev(tokens);
  g_free(oldpt);

  return g_slist_reverse(ret);
}

void readinput(GSList** l1, GSList** l2)
{
  FILE* fp = fopen("aoc03.txt", "r");

  char line[10240];
  fscanf(fp, "%s", line);
  *l1 = toPoints(line);
  
  fscanf(fp, "%s", line);
  *l2 = toPoints(line);

  fclose(fp);
}

void readinputTest(GSList** l1, GSList** l2)
{
  *l1 = toPoints("R75,D30,R83,U83,L12,D49,R71,U7,L72");
  *l2 = toPoints("U62,R66,U55,R34,D71,R55,D58,R83");

  // *l1 = toPoints("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51 ");
  // *l2 = toPoints("U98,R91,D20,R16,D67,R40,U7,R15,U6,R7");

  // *l1 = toPoints("R3,U4,L2,D2");
  // *l2 = toPoints("U5,R2,D3,L1");
}

#define MANDIST(p) (ABS(p->x) + ABS(p->y))

gboolean isin(int a0, int a1, int x)
{
  int b0 = MIN(a0, a1);
  int b1 = MAX(a0, a1);
  return (x >= b0 && x <= b1);
}

gboolean intersect(point* a0, point* a1, point* b0)
{
  return isin(a0->x, a1->x, b0->x) && isin(a0->y, a1->y, b0->y);
}

int sintersect(point* a0, point* a1, point* b0, point* b1)
{
  int dx = b1->x > b0->x ? +1 : b1->x < b0->x ? -1 : 0;
  int dy = b1->y > b0->y ? +1 : b1->y < b0->y ? -1 : 0;

  int mindist = INT_MAX;

  point* curr = g_new0(point, 1);
  curr->x = b0->x;
  curr->y = b0->y;

  while (curr->x != b1->x+dx || curr->y != b1->y+dy)
    {
      if (intersect(a0, a1, curr))
        {
          int md = MANDIST(curr);
          if (md < mindist)
            mindist = md;
        }

      curr->x += dx;
      curr->y += dy;
    }

  g_free(curr);

  return mindist;
}

point* sintersect2(point* a0, point* a1, point* b0, point* b1)
{
  int dx = b1->x > b0->x ? +1 : b1->x < b0->x ? -1 : 0;
  int dy = b1->y > b0->y ? +1 : b1->y < b0->y ? -1 : 0;

  point* ret = NULL;

  point* curr = g_new0(point, 1);
  curr->x = b0->x;
  curr->y = b0->y;

  while (curr->x != b1->x+dx || curr->y != b1->y+dy)
    {
      if (intersect(a0, a1, curr))
        {
          if (ret != NULL)
            g_print("WARNING, 2!\n"); // doesn't happen

          ret = g_new0(point, 1);
          ret->x = curr->x;
          ret->y = curr->y;
        }

      curr->x += dx;
      curr->y += dy;
    }
  g_free(curr);

  return ret;
}

int es1()
{
  GSList* l1;
  GSList* l2;
  readinput(&l1, &l2);

  int mindist = INT_MAX;

  point* p0 = (point*) l1->data;

  for (GSList* l=l1->next; l; l=l->next)
    {
      point* p1 = (point*) l->data;
      
      
      point* q0 = (point*) l2->data;

      for (GSList* q=l2->next; q; q=q->next)
        {
          point* q1 = (point*) q->data;

          int dist = sintersect(p0, p1, q0, q1);
          if (dist != INT_MAX && dist > 0) 
            {
            if (dist < mindist)
              mindist = dist;
          }
            q0 = q1;
        }

      p0 = p1;
    }

  g_slist_free_full(l1, g_free);
  g_slist_free_full(l2, g_free);

  return mindist;
}

GSList* IntersectionPoints(GSList* l1, GSList* l2)
{
  GSList* ret = NULL;

  point* p0 = (point*) l1->data;

  for (GSList* l=l1->next; l; l=l->next)
    {
      point* p1 = (point*) l->data;
      
      
      point* q0 = (point*) l2->data;

      for (GSList* q=l2->next; q; q=q->next)
        {
          point* q1 = (point*) q->data;

          point* dist = sintersect2(p0, p1, q0, q1);
          if (dist != NULL) 
            {
              ret = g_slist_prepend(ret, dist);
            }
            q0 = q1;
        }

      p0 = p1;
    }

  return ret;
}

int walk(GSList* l1, point* pt)
{
  int steps = 0;

  point X;
  X.x = ((point *)l1->data)->x;
  X.y = ((point *)l1->data)->y;

  for (GSList *l = l1->next; l; l = l->next)
    {
      point *p1 = (point *)l->data;
      int dx = p1->x > X.x ? +1 : p1->x < X.x ? -1 : 0;
      int dy = p1->y > X.y ? +1 : p1->y < X.y ? -1 : 0;
      
      while (X.x != p1->x || X.y != p1->y)
        {
          if (X.x == pt->x && X.y == pt->y)
            return steps;

          steps++;
          X.x += dx;
          X.y += dy;
        }
    }

  return steps;
}

int es2()
{
  GSList* l1;
  GSList* l2;
  readinput(&l1, &l2);

  int mindist = INT_MAX;

  GSList* inte = IntersectionPoints(l1, l2);

  for (GSList* i=inte; i; i=i->next)
    {
      point* pt = (point*) i->data;

      int len1 = walk(l1, pt);
      int len2 = walk(l2, pt);

      int this = len1+len2;

      if (len1 > 0 && len2 > 0 && this < mindist)
        mindist = this;
    }

  g_slist_free_full(inte, g_free);

  g_slist_free_full(l1, g_free);
  g_slist_free_full(l2, g_free);

  return mindist;  
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
