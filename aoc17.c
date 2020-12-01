#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

int es1()
{
  long long code[4000] = { 0 };
  int len = intcode_read("aoc17.txt", code);

  int ip = 0;
  int base = 0;
  int retcode = -1;
  int input_idx = 0;

  char matrix[10240];
  int idx = 0;

  while (retcode != 0)
    matrix[idx++]=(char)intcode_exec(code, len, NULL, 0, &input_idx, &ip, &retcode, &base);
  matrix[idx++]='\0';

  int linelen=0;
  while (matrix[linelen] != 10)
    linelen++;
  linelen++;

  int ymax = idx/linelen;

  int total = 0;
  
  for (int y=1; y<ymax-1; y++)
    for (int x=1; x<linelen-2; x++)
      if ( matrix[linelen*(y)+(x)] == '#' && matrix[linelen*(y)+(x+1)] == '#' && matrix[linelen*(y)+(x-1)] == '#' && 
          matrix[linelen*(y+1)+(x)] == '#' && matrix[linelen*(y-1)+(x)] == '#')
          {
          total += y*x;
          }

  return total;
}

char direction(char* matrix, int lenx, int posx, int posy)
{
  char content = matrix[lenx*posy+posx];

  if (content == '^')
    return 1;
  else if (content == '>')
    return 2;
  else if (content == 'v')
    return 3;
  else if (content == '<')
    return 4;

  return 0;
}

char new_direction(char* matrix, int lenx, int leny, int posx, int posy)
{
  int dir = direction(matrix, lenx, posx, posy);

  int newdir = 0;

  if (posy>0 && (matrix[lenx*(posy-1)+(posx)] == '#' || matrix[lenx*(posy-1)+(posx)] == 'O'))
    newdir = 1;
  else if (posx<lenx && (matrix[lenx*(posy)+(posx+1)] == '#' || matrix[lenx*(posy)+(posx+1)] == 'O'))
    newdir = 2;
  else if (posy<leny && (matrix[lenx*(posy+1)+(posx)] == '#' || matrix[lenx*(posy+1)+(posx)] == 'O'))
    newdir = 3;
  else if (posx>0 && (matrix[lenx*(posy)+(posx-1)] == '#' || matrix[lenx*(posy)+(posx-1)] == 'O'))
    newdir = 4;

  if ((newdir == 4 ? 1 : newdir+1) == dir)
    {
      matrix[lenx*(posy)+(posx)] = newdir==1?'^':newdir==2?'>':newdir==3?'v':newdir==4?'<':'X';
      return 'L';
    }
  else if (newdir == (dir == 4 ? 1 : dir+1))
    {
      matrix[lenx*(posy)+(posx)] = newdir==1?'^':newdir==2?'>':newdir==3?'v':newdir==4?'<':'X';
      return 'R';
    }
  else
    return '?';
}

int proceed(char* matrix, int lenx, int leny, int* posx, int* posy)
{
  char dir = direction(matrix, lenx, *posx, *posy);
  int cnt = 0;

  matrix[lenx*(*posy)+(*posx)] = '.';

  int dx = (dir == 2? +1 : dir == 4 ? -1 : 0);
  int dy = (dir == 1? -1 : dir == 3 ? +1 : 0);

  while ( *posx+dx >= 0 && *posx+dx < lenx  &&  *posy+dy>=0 && *posy+dy < leny && 
        (matrix[lenx*(*posy+dy)+(*posx+dx)] == '#' || matrix[lenx*(*posy+dy)+(*posx+dx)] == 'O'))
    {
      if (matrix[lenx*(*posy+dy)+(*posx+dx)] == 'O')
        matrix[lenx*(*posy+dy)+(*posx+dx)] = '#';
      else
        matrix[lenx*(*posy+dy)+(*posx+dx)] = '.';

      *posx += dx;
      *posy += dy;
      cnt++;
    }

  matrix[lenx*(*posy)+(*posx)] = dir==1?'^':dir==2?'>':dir==3?'v':dir==4?'<':'X';

  return cnt;
}

typedef struct
{
  char dir;
  int len;
} mov;

void mov2string(mov movs[], int movs_len, char* output)
{
  int idx = 0;

  for (int i=0; i<movs_len; i++)
    {
      output[idx++] = movs[i].dir;
      output[idx++] = ',';

      if (movs[i].len >= 10)
        output[idx++] = '0' + (movs[i].len / 10);
      output[idx++] = '0' + (movs[i].len % 10);

      if (i != movs_len-1)
        output[idx++] = ',';
    }

  output[idx++] = '\0';
}

void decomposeSeq(mov movs[], int mov_len, char* mainseq, char* a, char* b, char* c)
{
  mov mova[50];
  mov movb[50];
  mov movc[50];

  for (int lena=5; lena>=2; lena--)
    {
      int from_where1 = 0;
      memcpy(mova, &movs[from_where1], lena*sizeof(mov));
      from_where1 = lena;

      for (int lenb=5; lenb>=2; lenb--)
        {
          int from_where2 = from_where1;

          while (memcmp(&movs[from_where2], mova, lena*sizeof(mov)) == 0)
              from_where2 += lena;

          memcpy(movb, &movs[from_where2], lenb*sizeof(mov));
          from_where2 += lenb;

          for (int lenc=5; lenc>=2; lenc--)
            {
              int from_where3 = from_where2;

              while (1)
                {
                  if (memcmp(&movs[from_where3], mova, lena*sizeof(mov)) == 0)
                    from_where3 += lena;
                  else if (memcmp(&movs[from_where3], movb, lenb*sizeof(mov)) == 0)
                    from_where3 += lenb;
                  else
                    break;
                }

              memcpy(movc, &movs[from_where3], lenc*sizeof(mov));

              gboolean is_valid = TRUE;
              int idx = 0;
              int sid = 0;

              while (idx<mov_len)
                {
                  if (memcmp(&movs[idx], mova, lena*sizeof(mov)) == 0)
                    {
                      idx += lena;
                      mainseq[sid++] = 'A';
                      mainseq[sid++] = ',';
                    }
                  else if (memcmp(&movs[idx], movb, lenb*sizeof(mov)) == 0)
                    {
                      idx += lenb;
                      mainseq[sid++] = 'B';
                      mainseq[sid++] = ',';
                    }
                  else if (memcmp(&movs[idx], movc, lenc*sizeof(mov)) == 0)
                    {
                      idx += lenc;
                      mainseq[sid++] = 'C';
                      mainseq[sid++] = ',';
                    }
                  else
                    {
                      is_valid = FALSE;
                      break;
                    }
                }

              if (is_valid)
                {
                  mainseq[sid-1] = '\0';
                  mov2string(mova, lena, a);
                  mov2string(movb, lenb, b);
                  mov2string(movc, lenc, c);
                  return;
                }
            }
        }
    }
}

long long es2()
{
  long long code[10000] = { 0 };
  int len = intcode_read("aoc17.txt", code);

  int ip = 0;
  int base = 0;
  int retcode = -1;
  int input_idx = 0;

  char matrix[10240];
  int idx = 0;

  while (retcode != 0)
    matrix[idx++]=(char)intcode_exec(code, len, NULL, 0, &input_idx, &ip, &retcode, &base);
  matrix[idx++]='\0';

  int linelen=0;
  while (matrix[linelen] != 10)
    linelen++;
  linelen++;

  int ymax = idx/linelen;

  char newmatrix[10240];
  strcpy(newmatrix, matrix);


  for (int y=1; y<ymax-1; y++)
    for (int x=1; x<linelen-2; x++)
      if (matrix[linelen*(y)+(x)] == '#' && matrix[linelen*(y)+(x+1)] == '#' && matrix[linelen*(y)+(x-1)] == '#' && 
          matrix[linelen*(y+1)+(x)] == '#' && matrix[linelen*(y-1)+(x)] == '#')
          {
            newmatrix[linelen*(y)+(x)] = 'O';
          }

  strcpy(matrix, newmatrix);

  int px = INT_MIN;
  int py = INT_MIN;

  for (int y=0; y<ymax; y++)
    for (int x=0; x<linelen; x++)
      {
        char content = matrix[linelen*y+x];

        if (content == '^' || content == '>' || content == 'v' || content == '<')
          {
            px = x;
            py = y;
          }
      }
  // g_print("robot is at (%d, %d) dir=%d\n", px, py, dir);

  mov seq[1000];
  int mov_len = 0;


  while (1)
    {
      seq[mov_len].dir = new_direction(matrix, linelen, ymax, px, py);
      if (seq[mov_len].dir == '?')
          break;
      
      seq[mov_len++].len = proceed(matrix, linelen, ymax, &px, &py);
      // g_print("%s", matrix);
    }

  // char result[1024] = { 0 };
  // mov2string(seq, mov_len, result);
  // g_print("%s\n", result);

  char mainseq[1024] = { 0 };
  char seqA[1024] = { 0 };
  char seqB[1024] = { 0 };
  char seqC[1024] = { 0 };

  decomposeSeq(seq, mov_len, mainseq, seqA, seqB, seqC);

  // g_print("Found something! [%s] (%s)  (%s)  (%s)\n", mainseq, seqA, seqB, seqC);

  long long  input[1000];
  int iidx = 0;


  for (int i=0; mainseq[i]; i++)
    input[iidx++] = mainseq[i];
  input[iidx++] = 10;

  for (int i=0; seqA[i]; i++)
    input[iidx++] = seqA[i];
  input[iidx++] = 10;

  for (int i=0; seqB[i]; i++)
    input[iidx++] = seqB[i];
  input[iidx++] = 10;

  for (int i=0; seqC[i]; i++)
    input[iidx++] = seqC[i];
  input[iidx++] = 10;


  input[iidx++] = 'n';
  input[iidx++] = 10;


  // g_print("{ ");
  // for (int i=0; i<iidx; i++)
  //   {
  //     g_print("%lld ", input[i]);
  //   }
  // g_print("}\n");


  ip = 0;
  base = 0;
  input_idx = 0;


  memset(code, '\0', sizeof(code));
  len = intcode_read("aoc17.txt", code);
  code[0] = 2;

  long long total = 0;

  while (1)
    {
      long long res = intcode_exec(code, len, input, iidx, &input_idx, &ip, &retcode, &base);

      if (res < 256)
        ; //g_print("%c\n", (char)res);
      else
        total = res;
        // g_print("%lld\n", res);

      if (retcode == 0)
        break;
    }




  return total;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %lld\n", es2());
  return 0;
}
