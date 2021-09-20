// arg_parser.h

#ifndef _ARG_PARSER_H
#define _ARG_PARSER_H

typedef enum Arg_type {
  ARG_INT = 0,
  ARG_FLOAT,
  ARG_STRING,
  ARG_BUFFER,

  MAX_ARG_TYPE,
} Arg_type;

static const char* arg_type_desc[MAX_ARG_TYPE] = {
  "integer",
  "float",
  "string",
  "buffer",
};

typedef struct Parse_arg {
  char flag;  // Single char to identify the argument flag
  const char* long_flag; // Long string to identify the argument flag
  const char* desc; // Description of this flag
  Arg_type type;  // Which type the data argument is to be
  i32 num_args;  // Can be either one or zero for any one flag
  void* data; // Reference to the data which is going to be overwritten by the value of the argument(s)
} Parse_arg;

void args_print_help(FILE* fp, Parse_arg* args, i32 num_args, i32 argc, char** argv);

i32 parse_args(Parse_arg* args, i32 num_args, i32 argc, char** argv);

#endif
