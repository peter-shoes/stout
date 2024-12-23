#ifndef SCANNER_H
#define SCANNER_H

#define MAX_CMD_LEN 16

typedef enum
{
  /*  Base types/registers.  */
  TOKEN_NUM,
  TOKEN_ACC,
  TOKEN_DAT,
  /*  Basic I/O.  */
  TOKEN_MOV,
  TOKEN_STOUT,
  TOKEN_STIN,
  /*  Math commands.  */
  TOKEN_ADD,
  TOKEN_SUB,
  TOKEN_MUL,
  TOKEN_DIV,
  /*  Branch commands.  */
  TOKEN_TEQ,
  TOKEN_TGT,
  TOKEN_TLT,
  TOKEN_TCP,
  TOKEN_T_POS,
  TOKEN_T_NEG,
  /*  Jump instructions.  */
  TOKEN_JMP,
  TOKEN_LABEL,
  // special
  TOKEN_ERROR,
  TOKEN_EOF
} token_type;

typedef struct token
{
  token_type type;
  const char *start;
  int length;
  int line;
} token_t;

void init_scanner (const char *source);
token_t scan_token ();

#endif
