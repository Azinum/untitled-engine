// random.c

static random_t start_seed = 0;
static random_t current_seed = 0;

typedef random_t (*generator_func)();

#define MAX_GEN_STACK 12
static generator_func generator_stack[MAX_GEN_STACK] = {NULL};
static u32 generator_count = 0;

static random_t stub_random();
static random_t lc_random();

random_t stub_random() {
  return ++current_seed;
}

random_t lc_random() {
  const random_t a = 16807;
  const random_t multiplier = 2147483647;
  const random_t increment = 13;

  current_seed = (current_seed * a + increment) & multiplier;

  return current_seed;
}

static generator_func generator_table[MAX_RANDOM_GENERATOR] = {
  stub_random,
  lc_random,
};

f64 random_chi_square_test(const u32 iterations, const u32 range) {
  f64 sum = 0;
  const random_t expected = range / 2;
  const random_t expected_total = iterations * expected;

  // Using Pearson's chi-square test to check the quality of
  // the distribution of random numbers:
  //    (o - e)^2 / e, where o = observed and e = expected
  for (u32 i = 0; i < iterations; ++i) {
    random_t value = random_number() % range;
    sum += ((value - expected) * (value - expected)) / expected;
  }

  return sum / (f64)expected_total;
}

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
