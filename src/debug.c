#include "debug.h"

#include "common.h"
#include <stdio.h>

void
debug_token (token_t token, int *line)
{
  if (token.line != *line)
    {
      printf ("%4d ", token.line);
      *line = token.line;
    }
  else
    printf ("   | ");
    printf ("%2d '%.*s'\n", token.type, token.length, token.start);
  // else
  //   printf ("%2d <WS>\n", token.type);
}
