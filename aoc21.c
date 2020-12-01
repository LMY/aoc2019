#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

void set_program(intcode_machine* m, char* program)
{
  for (int i=0; program[i]; i++)
    m->inputs[m->input_len++] = program[i];
}

long long es1()
{
  intcode_machine m = intcode_machine_create("aoc21.txt");

  char program[] = "NOT A J\n" \
                   "NOT B T\n" \
                   "OR T J\n" \
                   "NOT C T\n" \
                   "OR T J\n" \

                   "AND D J\n" \

                   "WALK\n";

  set_program(&m, program);

  do
    {
      intcode_machine_execute(&m);

      if (m.output[0] > 255)
        return m.output[0];
      else
        {
          // g_print("%c", (char)m.output[0]);
          m.output_len = 0;
        }
    } while (m.retcode != 0);

  return 0;
}

int es2()
{
  intcode_machine m = intcode_machine_create("aoc21.txt");

  char program[] = 
                   "NOT A J\n" \
                   "NOT B T\n" \
                   "OR T J\n" \
                   "NOT C T\n" \
                   "OR T J\n" \

                   "AND D J\n" \

                   "NOT E T\n" \
                   "NOT T T\n" \
                   "OR H T\n" \
                   
                   "AND T J\n" \

                   "RUN\n";

  set_program(&m, program);

  do
    {
      intcode_machine_execute(&m);

      if (m.output[0] > 255)
        return m.output[0];
      else
        {
          // g_print("%c", (char)m.output[0]);
          m.output_len = 0;
        }
    } while (m.retcode != 0);

  return 0;
}

int main()
{
  printf("1 -> %lld\n", es1());
  printf("2 -> %d\n", es2());
  return 0;
}
