// shader_compile.c

#include <stdio.h>
#include <stdint.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

static void string_replace(char* s, char a, char b);

void string_replace(char* s, char a, char b) {
  char* iter = s;
  while (*iter != '\0') {
    if (*iter == a) {
      *iter = b;
    }
    ++iter;
  }
}

i32 main(i32 argc, char** argv) {
  if (argc < 2) {
    return 0;
  }

  char* name = argv[1];
  FILE* fp = fopen(name, "rb");
  FILE* out = stdout;
  u32 count = 0;

  string_replace(name, '.', '_');

  #define WIDTH 14

  if (fp) {
    fprintf(out, "char %s[] = {\n  ", name);

    while (!feof(fp)) {
      u8 byte = 0;
      if (fread(&byte, 1, 1, fp) == 0) {
        break;
      }
      fprintf(out, "0x%.2x, ", (i32)byte);
      ++count;
      if (!(count % WIDTH)) {
        fprintf(out, "\n  ");
      }
    }
    fprintf(out, "0x00, ");
    fprintf(out, "};\n\n");
    fclose(fp);
  }
  return 0;
}
