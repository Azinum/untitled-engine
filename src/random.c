// random.c

random_t start_seed = 0;
random_t current_seed = 0;

typedef random_t (*generator_func)();

#define MAX_GEN_STACK 12
static generator_func generator_stack[MAX_GEN_STACK] = {NULL};
static u32 generator_count = 0;

static random_t lc_random();

random_t lc_random() {
  const random_t a = 16807;
  const random_t multiplier = 2147483647;
  const random_t increment = 13;

  current_seed = (current_seed * a + increment) & multiplier;

  return current_seed;
}

static generator_func generator_table[MAX_RANDOM_GENERATOR] = {
  NULL,
  lc_random,
};

void random_push_generator(Random_type generator) {
  if (generator_count < MAX_GEN_STACK) {
    generator_stack[generator_count++] = generator_table[generator];
  }
}

void random_pop_generator() {
  if (generator_count > 0) {
    generator_count--;
  }
}

void random_clear_generators() {
  generator_count = 0;
}

random_t random_init(random_t seed) {
  if (seed == 0) {
    start_seed = current_seed = 1;
  }
  else {
    start_seed = current_seed = seed;
  }
  return start_seed;
}

random_t random_number() {
  for (u32 i = 0; i < generator_count; ++i) {
    generator_func generator = generator_stack[i];
    if (generator) {
      current_seed += generator();
    }
  }
  return current_seed;
}
