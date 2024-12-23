#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct
{
  const char *start;
  const char *current;
  int line;
} Scanner;

Scanner scanner;

void
init_scanner (const char *source)
{
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool
is_alpha (char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool
is_digit (char c)
{
  return c >= '0' && c <= '9';
}

static bool
is_at_end ()
{
  return *scanner.current == '\0';
}

static bool
is_end_of_line ()
{
  return *scanner.current == '\n';
}

static bool
is_whitespace (char c)
{
  if ((c == ' ') || (c == '\r') || (c == '\t') || (c == '\n'))
    return true;
  return false;
}

static char
advance ()
{
  scanner.current++;
  return scanner.current[-1];
}

static char
peek ()
{
  return *scanner.current;
}

static char
peek_next ()
{
  if (is_at_end ())
    return '\0';
  return scanner.current[1];
}

static bool
match (char expected)
{
  if (is_at_end ())
    return false;
  if (*scanner.current != expected)
    return false;
  scanner.current++;
  return true;
}

static token_t
make_token (token_type type)
{
  token_t token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static token_t
error_token (const char *message)
{
  token_t token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen (message);
  token.line = scanner.line;
  return token;
}

static void
skip_whitespace ()
{
  const char *start = scanner.current;
  for (;;)
    {
      char c = peek ();
      switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
          advance ();
          break;
        case '\n':
          scanner.line++;
          advance ();
          break;
	default:
	  return;
        }
    }
}

static token_t
number ()
  {
    while (is_digit (peek ()))
      advance();
    if (peek () == '.')
      advance ();
    else
      return make_token (TOKEN_NUM);
    while (is_digit (peek ()))
      advance();
    return make_token (TOKEN_NUM);
  }


/*  We have a series of reserved words, make one here or ERROR.  */
static token_t
get_reserved_word (char c)
{
  int i = 0;
  char *buf = malloc(sizeof (char) * (MAX_CMD_LEN + 1));

  while (is_alpha(c))
    {
      buf[i] = c;
      c = advance ();
      i++;
    }
  buf[i] = '\0';

  if (strcmp (buf, "acc") == 0)
    return make_token (TOKEN_ACC);
  if (strcmp (buf, "dat") == 0)
    return make_token (TOKEN_DAT);
  if (strcmp (buf, "mov") == 0)
    return make_token (TOKEN_MOV);
  if (strcmp (buf, "stout") == 0)
    return make_token (TOKEN_STOUT);
  if (strcmp (buf, "stin") == 0)
    return make_token (TOKEN_STIN);
  if (strcmp (buf, "add") == 0)
    return make_token (TOKEN_ADD);
  if (strcmp (buf, "sub") == 0)
    return make_token (TOKEN_SUB);
  if (strcmp (buf, "mul") == 0)
    return make_token (TOKEN_MUL);
  if (strcmp (buf, "div") == 0)
    return make_token (TOKEN_DIV);
  if (strcmp (buf, "teq") == 0)
    return make_token (TOKEN_TEQ);
  if (strcmp (buf, "tgt") == 0)
    return make_token (TOKEN_TGT);
  if (strcmp (buf, "tlt") == 0)
    return make_token (TOKEN_TLT);
  if (strcmp (buf, "tcp") == 0)
    return make_token (TOKEN_TCP);
  if (strcmp (buf, "jmp") == 0)
    return make_token (TOKEN_JMP);
  if (peek () == ':')
    {
      advance ();
      return make_token (TOKEN_LABEL);
    }

  char msg[50];
  sprintf(msg, "Unrecognized string on line %d: %s\n", scanner.line, buf);
  return error_token (msg);
}

token_t
scan_token ()
{
  scanner.start = scanner.current;
  skip_whitespace ();


  if (is_at_end ())
    return make_token (TOKEN_EOF);

  char c = advance ();
  switch (c)
    {
    case ' ':
    case '\r':
    case '\t':
      skip_whitespace ();
    case '\n':
      scanner.line++;
      skip_whitespace ();
    case '+':
      return make_token (TOKEN_T_POS);
    case '-':
      return make_token (TOKEN_T_NEG);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return number();
    default:
      return get_reserved_word (c);
    }
    
    
  /*  It should never execute this code  */
  return error_token ("Unexpected character.");
}
