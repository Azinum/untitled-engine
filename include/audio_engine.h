// audio_engine.h

#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

typedef struct Audio_engine {
  i32 sample_rate;
  i32 frames_per_buffer;
  u8* out;
} Audio_engine;

extern Audio_engine audio_engine;

i32 audio_engine_state_init(i32 sample_rate, i32 frames_per_buffer);

i32 audio_engine_process(void* out_buffer);

void audio_engine_terminate();

#endif
