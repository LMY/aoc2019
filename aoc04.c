#include <glib-2.0/glib.h>

gboolean is_ok1(int x)
{
  char cyp[6];
  
  for (int i=0; i<6; i++)
    {
      cyp[5-i] = x % 10;
      x /= 10;
    }

  gboolean rep = FALSE;

  for (int i=0; i<5; i++)
    if (cyp[i+1] < cyp[i])
      return FALSE;
    else if (cyp[i+1] == cyp[i])
      rep = TRUE;

  return rep;
}

gboolean is_ok2(int x)
{
  char cyp[6];
  
  for (int i=0; i<6; i++)
    {
      cyp[5-i] = x % 10;
      x /= 10;
    }

  gboolean done = FALSE;
  gboolean repcnt = 1;

  for (int i=0; i<5; i++)
    if (cyp[i+1] < cyp[i])
      return FALSE;
    else
      {
        if (cyp[i+1] == cyp[i])
          repcnt++;
        else
          {
            if (repcnt == 2)
              done = TRUE;

            repcnt = 1;          
          }
      }
  if (repcnt == 2)
    done = TRUE;

  return done;
}

typedef gboolean (*ok_func)(int);

int count(int min, int max, ok_func func)
{
  int cnt = 0;

  for (int i=min; i<max; i++)
    if (func(i))
      cnt++;

  return cnt;
}

int es1()
{
  return count(197487, 673251, is_ok1);
}

int es2()
{
  return count(197487, 673251, is_ok2);
}

int main()
{
  g_print("1 -> %d\n", es1());
  g_print("2 -> %d\n", es2());
  return 0;
}
