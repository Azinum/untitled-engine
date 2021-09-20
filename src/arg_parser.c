// arg_parser.c

void args_print_help(FILE* fp, Parse_arg* args, i32 num_args, i32 argc, char** argv) {
  assert(argc != 0);
  i32 longest_arg_length = 0;
  // Find the longest argument (longflag)
  for (u32 arg_index = 0; arg_index < num_args; ++arg_index) {
    Parse_arg* arg = &args[arg_index];
    if (!arg->long_flag) {
      continue;
    }
    u32 argLength = string_len(arg->long_flag);
    if (longest_arg_length < argLength) {
      longest_arg_length = argLength;
    }
  }

  fprintf(fp, "USAGE:\n  %s [options]\n\n", argv[0]);
  fprintf(fp, "FLAGS:\n");
  for (u32 arg_index = 0; arg_index < num_args; ++arg_index) {
    Parse_arg* arg = &args[arg_index];
    fprintf(fp, "  ");
    if (arg->flag) {
      fprintf(fp, "-%c", arg->flag);
    }
    if (arg->flag && arg->long_flag) {
      fprintf(fp, ", --%-*s", longest_arg_length, arg->long_flag);
    }
    else if (!arg->flag && arg->long_flag) {
      fprintf(fp, "--%-*s", longest_arg_length, arg->long_flag);
    }
    if (arg->num_args > 0) {
      fprintf(fp, " ");
      fprintf(fp, "<%s>", arg_type_desc[arg->type]);
    }
    fprintf(fp, " %s\n", arg->desc);
  }
  fprintf(fp, "  -h, --%-*s show help menu\n\n", longest_arg_length, "help");
}

i32 parse_args(Parse_arg* args, i32 num_args, i32 argc, char** argv) {
  for (i32 index = 1; index < argc; ++index) {
    char* arg = argv[index];
    u8 long_flag = 0;
    u8 found_flag = 0;

    if (*arg == '-') {
      arg++;
      if (*arg == '-') {
        long_flag = 1;
        arg++;
      }
      if (*arg == 'h' && !long_flag) {
        args_print_help(stdout, args, num_args, argc, argv);
        return ERR_HELP;
      }
      if (long_flag) {
        if (!string_cmp(arg, "help")) {
          args_print_help(stdout, args, num_args, argc, argv);
          return ERR_HELP;
        }
      }
      Parse_arg* parse_arg = NULL;
      // Linear search over the array of user defined arguments
      for (i32 arg_index = 0; arg_index < num_args; ++arg_index) {
        parse_arg = &args[arg_index];
        if (long_flag) {
          if (parse_arg->long_flag) {
            if (!string_cmp(parse_arg->long_flag, arg)) {
              found_flag = 1;
              break;
            }
          }
        }
        else {
          if (parse_arg->flag == *arg) {
            // We found the flag
            found_flag = 1;
            break;
          }
        }
      }

      if (found_flag) {
        if (parse_arg->num_args > 0) {
          if (index + 1 < argc) {
            char* buffer = argv[++index];
            assert(buffer != NULL);
            assert(parse_arg);
            switch (parse_arg->type) {
              case ARG_INT: {
                sscanf(buffer, "%i", (i32*)parse_arg->data);
                break;
              }
              case ARG_FLOAT: {
                sscanf(buffer, "%f", (float*)parse_arg->data);
                break;
              }
              case ARG_STRING: {
                char** String = parse_arg->data;
                *String = buffer;
                break;
              }
              case ARG_BUFFER: {
                sscanf(buffer, "%s", (char*)parse_arg->data);
                break;
              }
              default:
                assert(0);
                break;
            }
          }
          else {
            fprintf(stderr, "Missing parameter after flag '%c'\n", *arg);
            return ERR;
          }
        }
        else {
          switch (parse_arg->type) {
            case ARG_INT: {
              *(i32*)parse_arg->data = 1;
              break;
            }
            case ARG_FLOAT: {
              *(float*)parse_arg->data = 1.0f;
              break;
            }
            default:
              break;
          }
        }
      }
      else {
        fprintf(stderr, "Flag '%s' not defined\n", arg);
        return ERR;
      }
    }
    else {
      Parse_arg* parse_arg = NULL;
      for (u32 arg_index = 0; arg_index < num_args; ++arg_index) {
        parse_arg = &args[arg_index];
        if (parse_arg->flag == 0 && parse_arg->long_flag == NULL) {
          switch (parse_arg->type) {
            case ARG_STRING: {
              char** String = parse_arg->data;
              *String = argv[index];
              break;
            }
            case ARG_BUFFER: {
              string_copy((char*)parse_arg->data, argv[index]);
            }
            default:
              break;
          }
          break;
        }
      }
    }
  }
  return NO_ERR;
}
