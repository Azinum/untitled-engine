// audio_engine.h

#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#define MAX_BUS 16
#define MAX_AUDIO_STATE 16
#define MAX_AUDIO_SOURCE 64

enum Audio_bus_id {
  AUDIO_BUS_MASTER = 0,
};

typedef struct Audio_state {
  i32 id;
  u32 sample_index;
  u32 reached_end;
} Audio_state;

typedef struct Bus {
  f32* buffer;
  u32 channel_count;
  f32 gain;
  Audio_state audio_states[MAX_AUDIO_STATE];
  u32 audio_count;
} Bus;

typedef struct Audio_engine {
  i32 sample_rate;
  i32 frames_per_buffer;
  i32 channel_count;
  u8* out;
  f32 gain;

  Audio_source audio_source[MAX_AUDIO_SOURCE];
  u32 audio_source_count;

  f32* bus_buffer_memory;
  Bus buses[MAX_BUS];
  u32 bus_count;
} Audio_engine;

extern Audio_engine audio_engine;

i32 audio_engine_state_init(i32 sample_rate, i32 frames_per_buffer, i32 channel_count);

i32 audio_engine_process(void* out_buffer);

i32 audio_engine_push_audio_source(Audio_source* audio);

void audio_engine_play_audio_once(i32 audio_id, u32 bus_id, f32 gain);

void audio_engine_state_free();

void audio_engine_terminate();

#endif
