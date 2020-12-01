#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum 
{
  DEAL,
  CUT,
  DEALINC
} ActionType;

typedef struct
{
  ActionType type;
  long long arg;
} action;

void show_deck(int deck[], int len)
{
  g_print("{ ");

  for (int i=0; i<len; i++)
    g_print("%d ", deck[i]);

  g_print("}\n");
}

int read_input(action actions[])
{
  FILE* fp = fopen("aoc22.txt", "r");

  int idx = 0;
  char line[1024];

  while (fscanf(fp, "%s", line) != EOF)
    {
      if (strcmp(line, "cut") == 0)
      {
        actions[idx].type = CUT;
        fscanf(fp, "%lld", &actions[idx++].arg);
      }
      else if (strcmp(line, "deal") == 0)
      {
        fscanf(fp, "%s", line);

        if (strcmp(line, "into") == 0)
          {
            fscanf(fp, "%*s %*s");
            actions[idx++].type = DEAL;
          }
        else if (strcmp(line, "with") == 0)
          {
            actions[idx].type = DEALINC;
            fscanf(fp, "%*s %lld", &actions[idx++].arg);
          }
        else
          g_print("UNKNOWN DEAL COMMAND [%s]\n", line);              
      }
      else
        g_print("UNKNOWN COMMAND [%s]\n", line);      
    }

  fclose(fp);

  return idx;
}

void execute_action_deal(int deck[], int deck_len)
{
  for (int i=0; i<deck_len/2; i++)
    {
      int temp = deck[i];
      deck[i] = deck[deck_len-1-i];
      deck[deck_len-1-i] = temp;
    }
}

void execute_action_cut(int deck[], int deck_len, int arg)
{
  int newdeck[deck_len];
  int idx = 0;

  if (arg >= 0)
    {
      for (int i=arg; i<deck_len; i++)
        newdeck[idx++] = deck[i];

      for (int i=0; i<arg; i++)
        newdeck[idx++] = deck[i];
    }
  else
    {
      int limit = deck_len+arg;
      for (int i = limit; i < deck_len; i++)
        newdeck[idx++] = deck[i];

      for (int i = 0; i < limit; i++)
        newdeck[idx++] = deck[i];
    }

  memcpy(deck, newdeck, sizeof(newdeck));
}

void execute_action_dealinc(int deck[], int deck_len, int arg)
{
  int newdeck[deck_len];
  int idx = 0;
  int cur = 0;

  while (idx < deck_len)
    {
      newdeck[cur % deck_len] = deck[idx++];
      // newdeck[idx++] = deck[cur % deck_len];
      cur += arg;
    }

  memcpy(deck, newdeck, sizeof(newdeck));
}

void execute_action(int deck[], int deck_len, action action)
{
  if (action.type == DEAL)
    execute_action_deal(deck, deck_len);
  else if (action.type == CUT)
    execute_action_cut(deck, deck_len, action.arg);
  else if (action.type == DEALINC)
    execute_action_dealinc(deck, deck_len, action.arg);
}

void execute(int deck[], int deck_len, action actions[], int actions_len)
{
  for (int i=0; i<actions_len; i++)
    {
      execute_action(deck, deck_len, actions[i]);
      // show_deck(deck, deck_len);
    }
}

int create_deck(int deck[], int len)
{
  for (int i=0; i<len; i++)
    deck[i] = i;

  return len;
}


void expect(char* test_name, int deck1[], int deck2[], int deck_len)
{
  gboolean ok = TRUE;

  for (int i=0; i<deck_len; i++)
    if (deck1[i] != deck2[i])
      {
        ok = FALSE;
        break;
      }

  if (!ok)
    {
      g_print("TEST %s failed!\n", test_name);
      g_print("IS ");
      show_deck(deck1, deck_len);
      g_print("VS ");
      show_deck(deck2, deck_len);
    }
}

void test1()
{
  int deck[10];

  {
    int expected_deck[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    action actions[] = { { .type = DEAL, .arg = 0} };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("DEAL", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 };
    action actions[] = { { .type = CUT, .arg = 3} };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("CUT+", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 };
    action actions[] = { { .type = CUT, .arg = -4} };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("CUT-", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 0, 7, 4, 1, 8, 5, 2, 9, 6, 3 };
    action actions[] = { { .type = DEALINC, .arg = 3} };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("DEALINC", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 0, 3, 6, 9, 2, 5, 8, 1, 4, 7 };
    action actions[] = { { .type = DEALINC, .arg = 7}, { .type = DEAL, .arg = 0}, { .type = DEAL, .arg = 0}  };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("1", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 3, 0, 7, 4, 1, 8, 5, 2, 9, 6 };
    action actions[] = { { .type = CUT, .arg = 6}, { .type = DEALINC, .arg = 7}, { .type = DEAL, .arg = 0}  };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("2", deck, expected_deck, 10);
  }

  {
    int expected_deck[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2, 9 };
    action actions[] = { { .type = DEALINC, .arg = 7}, { .type = DEALINC, .arg = 9}, { .type = CUT, .arg = -2},  };

    create_deck(deck, 10);
    execute(deck, 10, actions, sizeof(actions)/sizeof(actions[0]));

    expect("3", deck, expected_deck, 10);
  }
}



int es1()
{
  test1();

  #define LEN 10007

  int deck[LEN];
  int deck_len = create_deck(deck, LEN);

  action actions[1024];
  int actions_len = read_input(actions);

  execute(deck, deck_len, actions, actions_len);

  for (int i=0; i<LEN; i++)
    if (deck[i] == 2019)
      return i;

  return -1;
}


long long track_deal(long long pos0, long long deck_len)
{
  return deck_len-1-pos0;
}

long long track_cut(long long pos0, long long deck_len, int arg)
{
  // return arg >= 0 ? (pos0>=arg? pos0-arg: deck_len+pos0-arg) : (pos0>=deck_len+arg ? pos0-arg-deck_len : pos0-arg);  
  // so, basically  pos0-arg  mod deck_len, but % doesn't work on negatives, add +deck_len or 2*deck_len should solve:
  return (deck_len+pos0-arg) % deck_len;
}

long long track_dealwith(long long pos0, long long deck_len, int arg)
{
  return arg*pos0 % deck_len;
}

long long track_action(long long pos0, long long deck_len, action action)
{
  if (action.type == DEAL)
    return track_deal(pos0, deck_len);
  else if (action.type == CUT)
    return track_cut(pos0, deck_len, action.arg);
  else if (action.type == DEALINC)
    return track_dealwith(pos0, deck_len, action.arg);
  
  else
    return -1;  
}



long long inv_track_deal(long long pos0, long long deck_len)
{
  return deck_len-1-pos0;
}

long long inv_track_cut(long long pos0, long long deck_len, int arg)
{
  return (pos0+arg) % deck_len;
}

long long inv_track_dealwith(long long pos0, long long deck_len, int arg)
{
  return arg*pos0 % deck_len;
}

long long inv_track_action(long long pos0, long long deck_len, action action)
{
  if (action.type == DEAL)
    return inv_track_deal(pos0, deck_len);
  else if (action.type == CUT)
    return inv_track_cut(pos0, deck_len, action.arg);
  else if (action.type == DEALINC)
    return inv_track_dealwith(pos0, deck_len, action.arg);
  
  else
    return -1;  
}




long long track(long long pos0, long long deck_len, action actions[], int actions_len)
{
  for (int i=0; i<actions_len; i++)
    pos0 = track_action(pos0, deck_len, actions[i]);

  return pos0;
}

void test2()
{
  {
    long long fixture[][2] = {{0, 9},
                              {1, 8},
                              {2, 7},
                              {3, 6},
                              {4, 5},
                              {5, 4},
                              {6, 3},
                              {7, 2},
                              {8, 1},
                              {9, 0},

                              {-1, -1}};

    for (int i=0; fixture[i][0] >=0; i++)
      if (track_deal(fixture[i][0], 10) != fixture[i][1])
        g_print("FAIL track_deal(%lld, 10) = %lld should be %lld\n", fixture[i][0], track_deal(fixture[i][0], 10), fixture[i][1]);        
  }

  {
    long long fixture[][3] = {{0, 3, 7},
                              {1, 3, 8},
                              {2, 3, 9},
                              {3, 3, 0},
                              {4, 3, 1},
                              {5, 3, 2},
                              {6, 3, 3},
                              {7, 3, 4},
                              {8, 3, 5},
                              {9, 3, 6},

                              {0, -4, 4},
                              {1, -4, 5},
                              {2, -4, 6},
                              {3, -4, 7},
                              {4, -4, 8},
                              {5, -4, 9},
                              {6, -4, 0},
                              {7, -4, 1},
                              {8, -4, 2},
                              {9, -4, 3},

                              {-1, -1, -1}};

    for (int i=0; fixture[i][0] >=0; i++)
      if (track_cut(fixture[i][0], 10, fixture[i][1]) != fixture[i][2])
        g_print("FAIL track_cut(%lld, 10, %lld) = %lld should be %lld\n", fixture[i][0], fixture[i][1], track_cut(fixture[i][0], 10, fixture[i][1]), fixture[i][2]);         
  }

  {
    long long fixture[][3] = {{0, 3, 0},
                              {1, 3, 3},
                              {2, 3, 6},
                              {3, 3, 9},
                              {4, 3, 2},
                              {5, 3, 5},
                              {6, 3, 8},
                              {7, 3, 1},
                              {8, 3, 4},
                              {9, 3, 7},

                              {-1, -1, -1}};

    for (int i=0; fixture[i][0] >=0; i++)
      if (track_dealwith(fixture[i][0], 10, fixture[i][1]) != fixture[i][2])
        g_print("FAIL track_cut(%lld, 10, %lld) = %lld\n", fixture[i][0], fixture[i][1], track_dealwith(fixture[i][0], 10, fixture[i][1]));            
  }

  {
    action actions[1024];
    int actions_len = read_input(actions);

    long long res = track(2019, 10007, actions, actions_len);
    if (res != es1())
      g_print("ES1 FAIL: %lld\n", res);
  }
}

int es2()
{
  test2();

  action actions[1024];
  int actions_len = read_input(actions);

  long long deck_len = 119315717514047;
  long long iterations = 101741582076661;

  long long pos = 2020;

  long long stack[1000000];
  int stackidx = 0;

  for (long long i=0; i<iterations; i++)
    {
      pos = track(pos, deck_len, actions, actions_len);
      
      // if (i % 1024*1024 == 0)
      g_print("%lld\t\t%lld\n", i, pos);

          for (int i=0; i<stackidx; i++)
            if (stack[i] == pos)
              {
                g_print("%d equal to %lld\n", i, stack[i]);
                break;
              }

      stack[stackidx++] = pos;

      if (pos == 0)
        break;
    }

  return -1;
}

int main()
{
  printf("1 -> %d\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
