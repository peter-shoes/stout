#include <stdio.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"

static int line = -1;

static float acc_mem = 0;
static float dat_mem = 0;
static int branch_status = -1;

static char *
tok_to_str (token_t token)
{
  char *s = malloc(sizeof(char) * (token.length + 1));
  sprintf(s, "%.*s", token.length, token.start);
  return s;
}

static void
compiler_fini (token_t token)
{
  /*  Allocate a buffer and push onto the stack  */
  token_t *buf = (token_t *)malloc (sizeof (token_t));
  *buf = token;

  #ifdef DEBUG_SCANNER
  debug_token (token, &line);
  #endif
}

static void
compiler_error (char *msg, token_t token)
{
  fprintf (stderr,"COMPILER ERROR: line %d: %s\n", token.line, msg);
  #ifndef DEBUG_SCANNER
  exit (1);
  #endif
}

//TODO: we need something to do jumping to lines
//Should be noted that jumping can only be done backwards (I think)

void
compile (const char *source, const char *outpath)
{
  init_scanner (source);

#ifdef DEBUG_SCANNER
  printf ("\n======== SCANNER DEBUG TRACE ========\n");
#endif

  for (;;)
    {
      token_t token = scan_token ();

      switch (token.type)
	{
	  // TODO: set error here if the next token is not a number
	  case TOKEN_ADD:
	    acc_mem += atof(tok_to_str (scan_token ()));
	    break;
	  case TOKEN_SUB:
	    acc_mem -= atof(tok_to_str (scan_token ()));
	    break;
	  case TOKEN_MUL:
	    acc_mem *= atof(tok_to_str (scan_token ()));
	    break;
	  case TOKEN_DIV:
	    //TODO: Check Divide by zero error
	    acc_mem /= atof(tok_to_str (scan_token ()));
	    break;
	  // TODO: branch cases here
	  // TODO: if this is a pos or neg branch, check the store and skip to
	  // the next line if it is bad
	  case TOKEN_MOV:
	    token = scan_token ();
	    switch (token.type)
	      {
		/*  All of these cases can be moved to acc, dat, or stout.  */
		case TOKEN_ACC:
		  token = scan_token ();
		  switch (token.type)
		    {
		      case TOKEN_ACC:
			break;
		      case TOKEN_DAT:
			dat_mem = acc_mem;
			break;
		      case TOKEN_STOUT:
			printf("%.4f\n", acc_mem);
			break;
		      default:
			compiler_error ("move target is something other than a register.", token);
			break;
		    }
		  break;
		case TOKEN_DAT:
		  token = scan_token ();
		  switch (token.type)
		    {
		      case TOKEN_ACC:
			acc_mem = dat_mem;
			break;
		      case TOKEN_DAT:
			break;
		      case TOKEN_STOUT:
			printf("%.4f\n", dat_mem);
			break;
		      default:
			compiler_error ("move target is something other than a register.", token);
			break;
		    }
		case TOKEN_NUM:
		  float hold = atof(tok_to_str (token));
		  token = scan_token ();
		  switch (token.type)
		    {
		      case TOKEN_ACC:
			acc_mem = hold;
			break;
		      case TOKEN_DAT:
			dat_mem = hold;
			break;
		      case TOKEN_STOUT:
			printf("%.4f\n", hold);
			break;
		      default:
			compiler_error ("move target is something other than a register.", token);
			break;
		    }
		  break;
		/*  This case can also be moved to those registers, but you
		    need to read it in first.  */
		case TOKEN_STIN:
		  token = scan_token ();
		  char buf[256];
		  switch (token.type)
		    {
		      case TOKEN_ACC:
			fgets (buf, sizeof (buf), stdin);
			acc_mem = atof (buf);
			break;
		      case TOKEN_DAT:
			fgets (buf, sizeof (buf), stdin);
			dat_mem = atof (buf);
			break;
		      case TOKEN_STOUT:
			fgets (buf, sizeof (buf), stdin);
			printf ("%.4f\n", atof (buf));
			break;
		      default:
			compiler_error ("move target is something other than a register.", token);
			break;
		    }
		  break;
		default:
		  compiler_error ("attempting to move something other than a register or constant.", token);
		  break;
	      }
	    break;
	
	  /*  These cases should never be encountered by this loop.  */
	  case TOKEN_NUM:
	  case TOKEN_ACC:
	  case TOKEN_DAT:
	  case TOKEN_STOUT:
	  case TOKEN_STIN:
	    compiler_error ("you cannot start a line with a register or constant.", token);
	    break;

	  case TOKEN_ERROR:
	    fprintf(stderr, "%*.s", token.length, token.start);
	    #ifndef DEBUG_SCANNER
	    exit (1);
	    #endif
	    break;

	  default:
	    break;
	}

      compiler_fini (token);
      if (token.type == TOKEN_EOF)
	{
	  line = -1;
	  break;
	}
    }

  return;

  oom:
    fprintf (stderr, "ERROR: out of memory");
    exit (1);
}
