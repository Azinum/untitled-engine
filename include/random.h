// random.h

#ifndef _RANDOM_H
#define _RANDOM_H

typedef u64 random_t;

typedef enum Random_type {
  RANDOM_STUB = 0,
  RANDOM_LC,  // Linear congruent generator

  MAX_RANDOM_GENERATOR,
} Random_type;

f64 random_chi_square_test(const u32 iterations, const u32 range);

void random_push_generator(Random_type generator);

void random_pop_generator();

void random_clear_generators();

random_t random_init(random_t seed);

random_t random_number();

#endif
