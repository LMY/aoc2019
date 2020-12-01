#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  void read_input(char matrix[], int* lenx, int* leny)
{
  FILE* fp = fopen("aoc20.txt", "r");

  *lenx = 0;
  *leny = 0;
  int idx = 0;

  while ((matrix[idx]=fgetc(fp)) != EOF)
    {
      if (matrix[idx] == '\n')
        {
          if (*lenx == 0)
            *lenx = idx;
          (*leny)++;
        }
      else
        idx++;
    }

  fclose(fp);
}

typedef struct
{
  int x;
  int y;
} point;

point INVALID = { -1, -1 };


point matrix_find_char(char* matrix, int lenx, int leny, char who)
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

int matrix_find_gate(char* matrix, int lenx, int leny, char who[], point pos[])
{
  int idx = 0;

  for (int y=0; y<leny; y++)
      for (int x=0; x<lenx; x++)
        if (matrix[lenx*y+x] == who[0])
          {
            pos[idx] = INVALID;

            if (x+2<lenx && matrix[lenx*y+(x+1)] == who[1] &&  matrix[lenx*y+(x+2)] == '.')
              {
                pos[idx].x = x+2;
                pos[idx].y = y;
              }
            else if (x+1<lenx && x >0 && matrix[lenx*y+(x+1)] == who[1] &&  matrix[lenx*y+(x-1)] == '.')
              {
                pos[idx].x = x-1;
                pos[idx].y = y;
              }
            else if (y+2 < leny && matrix[lenx*(y+1)+x] == who[1] &&  matrix[lenx*(y+2)+x] == '.')
              {
                pos[idx].x = x;
                pos[idx].y = y+2;
              }
            else if (y+1 < leny && leny > 0 && matrix[lenx*(y+1)+x] == who[1] &&  matrix[lenx*(y-1)+x] == '.')
              {
                pos[idx].x = x;
                pos[idx].y = y-1;
              }

            if (pos[idx].x >= 0 && pos[idx].y >= 0)
              idx++;            
          }
  
  return idx;
}

void get_label_at(char* matrix, int lenx, int leny, int px, int py, char label[])
{
  if (px >= 1 && matrix[lenx*(py)+(px)] >= 'A' && matrix[lenx*(py)+(px)] <= 'Z' && matrix[lenx*(py)+(px-1)] >= 'A' && matrix[lenx*(py)+(px-1)])
    {
      label[0] = matrix[lenx*(py)+(px-1)];
      label[1] = matrix[lenx*(py)+(px-0)];
    }
  else if (px+1 < lenx && matrix[lenx*(py)+(px)] >= 'A' && matrix[lenx*(py)+(px)] <= 'Z'&& matrix[lenx*(py)+(px+1)] >= 'A' && matrix[lenx*(py)+(px+1)] <= 'Z')
    {
      label[0] = matrix[lenx*(py)+(px+0)];
      label[1] = matrix[lenx*(py)+(px+1)];
    }
  else if (py >= 1 && matrix[lenx*(py)+(px)] >= 'A' && matrix[lenx*(py)+(px)] <= 'Z' && matrix[lenx*(py-1)+(px)] >= 'A' && matrix[lenx*(py-1)+(px)] <= 'Z')
    {
      label[0] = matrix[lenx*(py-1)+(px)];
      label[1] = matrix[lenx*(py-0)+(px)];
    }
  else if (py+1 < leny && matrix[lenx*(py)+(px)] >= 'A' && matrix[lenx*(py)+(px)] <= 'Z'&& matrix[lenx*(py+1)+(px)] >= 'A' && matrix[lenx*(py+1)+(px)] <= 'Z')
    {
      label[0] = matrix[lenx*(py+0)+(px)];
      label[1] = matrix[lenx*(py+1)+(px)];
    }
  else
    label[0] = label[1] = '\0';
}

gboolean is_label_internal(int lenx, int leny, int px, int py)
{
  return px<=2 || py <= 2 || px >= lenx-2 || py >= leny-2;
}

int visit(char* matrix, int lenx, int leny)
{
  int dist[15*1024];
  for (size_t i=0; i<sizeof(dist)/sizeof(dist[0]); i++)
    dist[i] = INT_MAX;

  point stack[20000];
  int stack_idx = 1;

  if (matrix_find_gate(matrix, lenx, leny, "AA", stack) != 1)
    {
      g_print("Couldn't find starting point\n");
      return 0;
    }
  dist[lenx*stack[0].y + stack[0].x] = 0;

  while (stack_idx > 0)
    {
      point pt = stack[--stack_idx];

      int newdist = dist[lenx*pt.y + pt.x] + 1;

      int DX[4] = { +1, -1, 00, 00 };
      int DY[4] = { 00, 00, +1, -1 };

      for (int d=0; d<4; d++)
        {
          point newpos = { pt.x + DX[d], pt.y + DY[d] };

          if (newpos.x < 0 || newpos.x >= lenx || newpos.y < 0 || newpos.y >= leny || matrix[lenx*newpos.y + newpos.x] == '#')
            continue;
          
          if (matrix[lenx*newpos.y + newpos.x] >= 'A' && matrix[lenx*newpos.y + newpos.x] <= 'Z')
            {
              char label[3] = { 0 };
              get_label_at(matrix, lenx, leny, newpos.x, newpos.y, label);

              point gate[3];
              int n = matrix_find_gate(matrix, lenx, leny, label, gate);
              if (n != 2)
                continue;

              newpos = gate[gate[0].x == pt.x && gate[0].y == pt.y ? 1 : 0];
            }

          if (newdist < dist[lenx*newpos.y + newpos.x])
            {
              dist[lenx*newpos.y + newpos.x] = newdist;
              stack[stack_idx++] = newpos;
            }
        }
    }

  if (matrix_find_gate(matrix, lenx, leny, "ZZ", stack) != 1)
    {
      g_print("Couldn't find ending point\n");
      return 0;
    }

  return dist[lenx*stack[0].y + stack[0].x];
}

int visit2(char* matrix, int lenx, int leny)
{
#define MAXLEVEL 100
  int dist[15*1024*MAXLEVEL];
  for (size_t i=0; i<sizeof(dist)/sizeof(dist[0]); i++)
    dist[i] = INT_MAX;

#define STACK_SIZE 20000
  point stack[STACK_SIZE];
  int stack_lvl[STACK_SIZE] = {0};
  int stack_idx = 1;

  int max_level = 0;

  if (matrix_find_gate(matrix, lenx, leny, "AA", stack) != 1)
    {
      g_print("Couldn't find starting point\n");
      return 0;
    }
  dist[lenx*stack[0].y + stack[0].x] = 0;

  while (stack_idx > 0)
    {
      point pt = stack[--stack_idx];
      int level = stack_lvl[stack_idx];

      int newdist = dist[lenx*leny*level + lenx*pt.y + pt.x] + 1;

      int DX[4] = { +1, -1, 00, 00 };
      int DY[4] = { 00, 00, +1, -1 };

      for (int d=0; d<4; d++)
        {
          point newpos = { pt.x + DX[d], pt.y + DY[d] };
          int newlevel = level;

          if (newpos.x < 0 || newpos.x >= lenx || newpos.y < 0 || newpos.y >= leny || matrix[lenx*newpos.y + newpos.x] == '#')
            continue;
          
          if (matrix[lenx*newpos.y + newpos.x] >= 'A' && matrix[lenx*newpos.y + newpos.x] <= 'Z')
            {
              char label[3] = { 0 };
              get_label_at(matrix, lenx, leny, newpos.x, newpos.y, label);

              point gate[3];
              int n = matrix_find_gate(matrix, lenx, leny, label, gate);
              if (n != 2)
                continue;

              if (!is_label_internal(lenx, leny, newpos.x, newpos.y))
                {
                newlevel++;
                }
              else
                if (newlevel > 0)
                  newlevel--;
                else
                  continue;
                
            if (newlevel >= MAXLEVEL-1)   
              {
                // g_print("LEVEL CUT\n");
              continue;
              }

              newpos = gate[gate[0].x == pt.x && gate[0].y == pt.y ? 1 : 0];
            }

          if (newdist < dist[lenx*leny*newlevel + lenx*newpos.y + newpos.x])
            {
              dist[lenx*leny*newlevel + lenx*newpos.y + newpos.x] = newdist;
              stack_lvl[stack_idx] = newlevel;
              stack[stack_idx++] = newpos;

              if (stack_idx == STACK_SIZE-1)
                g_print("STACK LIMIT REACHED\n");

              if (newlevel > max_level)
                max_level = newlevel;
            }
        }
    }

  if (matrix_find_gate(matrix, lenx, leny, "ZZ", stack) != 1)
    {
      g_print("Couldn't find ending point\n");
      return 0;
    }

  return dist[lenx*stack[0].y + stack[0].x];
}

int es1()
{
  int lenx;
  int leny;
  char matrix[15*1024];

  read_input(matrix, &lenx, &leny);

  return visit(matrix, lenx, leny);
}

int es2()
{
  int lenx;
  int leny;
  char matrix[15*1024];

  read_input(matrix, &lenx, &leny);

  return visit2(matrix, lenx, leny);
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
