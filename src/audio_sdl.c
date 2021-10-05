// audio_sdl.c

#include <SDL2/SDL.h>

static SDL_AudioSpec out_spec = {0};
static SDL_AudioSpec audio_spec = {0};
static i32 output_device = 0;

static void stereo_callback(void* user_data, u8* stream, i32 length);

void stereo_callback(void* user_data, u8* stream, i32 length) {
  if (length <= 0) {
    return;
  }
  (void)user_data; (void)length;
  audio_engine_process(stream);
}

i32 audio_engine_init(Audio_engine* a) {
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    fprintf(stderr, "audio_engine_init: Failed to initialize SDL audio subsystem\n");
    return ERR;
  }

  i32 driver = 0;
  const char* desired_driver = "alsa";
  const char* driver_name = NULL;

  FILE* fp = stdout;
  for (; driver < SDL_GetNumAudioDrivers(); ++driver) {
    const char* audio_driver = SDL_GetAudioDriver(driver);
    fprintf(fp, "driver: %s", audio_driver);
    if (!string_cmp(audio_driver, desired_driver)) {
      driver_name = audio_driver;
      fprintf(fp, " [SELECTED]");
    }
    fprintf(fp, "\n");
  }

  if (SDL_AudioInit(driver_name) != 0) {
    fprintf(stderr, "audio_engine_init: Failed to initialize audio driver: %s\n", driver_name);
    return ERR;
  }

  // Initialize output device
  memory_zero(&out_spec, sizeof(out_spec));
  out_spec.freq = a->sample_rate;
  out_spec.format = AUDIO_F32;
  out_spec.channels = a->channel_count;
  out_spec.samples = a->frames_per_buffer;
  out_spec.callback = stereo_callback;

  if ((output_device = SDL_OpenAudioDevice(NULL, 0, &out_spec, &audio_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE)) == 0) {
    fprintf(stderr, "audio_engine_init: Failed to open audio device: %s\n", SDL_GetError());
  }

  if (out_spec.format != audio_spec.format) {
    fprintf(stderr, "audio_engine_init: Requested format is not supported\n");
  }

  fprintf(fp, "Using SDL2 audio backend\n");
  return NO_ERR;
}

i32 audio_engine_start(Audio_engine* a) {
  SDL_PauseAudioDevice(output_device, 0);
  return NO_ERR;
}

void audio_engine_exit() {
  SDL_CloseAudioDevice(output_device);
  SDL_AudioQuit();
}
