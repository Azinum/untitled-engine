// random.c

random_t start_seed = 0;
random_t current_seed = 0;
const random_t a_const = 16807;
const random_t multiplier = 2147483647;
const random_t increment = 13;

void lc_random_init(random_t seed) {
  if (seed == 0) {
    start_seed = current_seed = 1;
  }
  else {
    start_seed = current_seed = seed;
  }
}

random_t lc_random() {
  current_seed = (current_seed * a_const + increment) & multiplier;
  return current_seed;
}
