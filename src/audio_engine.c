// audio_engine.c

extern i32 audio_engine_init(Audio_engine* a);

extern i32 audio_engine_start(Audio_engine* a);

extern void audio_engine_exit();

Audio_engine audio_engine = {0};
Audio_engine* a = &audio_engine;

#if USE_PORTAUDIO
  #include "audio_portaudio.c"
#elif USE_SDL
  #include "audio_sdl.c"
#else
  #include "audio_null.c"
#endif

static void clear_bus(Bus* bus);
static void process_bus(Bus* bus);

void clear_bus(Bus* bus) {
  audio_zero_f32_buffer(bus->buffer, sizeof(f32) * a->frames_per_buffer * bus->channel_count);
  for (u32 i = 0; i < bus->audio_count; ++i) {
    Audio_state* state = &bus->audio_states[i];
    // Remove if audio state has reached the end
    if (state->reached_end) {
      if (bus->audio_count > 0) {
        bus->audio_states[i] = bus->audio_states[--bus->audio_count];
      }
      else {
        --bus->audio_count;
      }
    }
  }
}

void process_bus(Bus* bus) {
  for (u32 i = 0; i < bus->audio_count; ++i) {
    Audio_state* state = &bus->audio_states[i];
    if (state->id < 0 || state->reached_end) {
      continue;
    }
    Audio_source* audio = &a->audio_source[state->id];
    f32* buffer = &bus->buffer[0];
    for (u32 buffer_index = 0; buffer_index < a->frames_per_buffer * bus->channel_count; ++buffer_index, ++buffer) {
      if (state->sample_index < audio->sample_count) {
        *buffer += audio->buffer[state->sample_index++];
      }
      else {
        state->reached_end = 1;
        break;
      }
    }
  }
  audio_mul_f32_buffers(bus->buffer, bus->buffer, bus->gain, sizeof(f32) * a->frames_per_buffer * bus->channel_count);
}

i32 audio_engine_state_init(i32 sample_rate, i32 frames_per_buffer, i32 channel_count) {
  a->sample_rate = sample_rate;
  a->frames_per_buffer = frames_per_buffer;
  a->channel_count = channel_count;
  a->out = NULL;
  a->gain = 1.0f;

  a->bus_buffer_memory = zone_malloc(MAX_BUS * sizeof(f32) * frames_per_buffer * channel_count);
  for (u32 i = 0; i < MAX_BUS; ++i) {
    Bus* bus = &a->buses[i];
    bus->buffer = &a->bus_buffer_memory[i * frames_per_buffer * channel_count];
    audio_zero_f32_buffer(bus->buffer, sizeof(f32) * frames_per_buffer * channel_count);
    bus->gain = 1.0f;
    bus->channel_count = channel_count;
    bus->audio_count = 0;
    ++a->bus_count;
  }

  i32 result = audio_engine_init(a);
  return result == NO_ERR ? audio_engine_start(a) : result;
}

i32 audio_engine_process(void* out_buffer) {
  f32* out = (f32*)out_buffer;

  // Clear output buffer
  audio_zero_f32_buffer(out, sizeof(f32) * a->frames_per_buffer * a->channel_count);

  // Clear all buses
  for (u32 i = 0; i < a->bus_count; ++i) {
    Bus* bus = &a->buses[i];
    clear_bus(bus);
  }

  // Process all buses
  for (u32 i = 0; i < a->bus_count; ++i) {
    Bus* bus = &a->buses[i];
    process_bus(bus);
  }

  // Sum all buses into output buffer
  for (u32 i = 0; i < a->bus_count; ++i) {
    Bus* bus = &a->buses[i];
    audio_sum_f32_buffers(out, out, bus->buffer, sizeof(f32) * a->frames_per_buffer * bus->channel_count);
  }
  audio_mul_f32_buffers(out, out, a->gain, sizeof(f32) * a->frames_per_buffer * a->channel_count);
  return NO_ERR;
}

i32 audio_engine_push_audio_source(Audio_source* audio) {
  assert(audio != NULL);
  i32 id = -1;
  if (a->audio_source_count < MAX_AUDIO_SOURCE) {
    id = a->audio_source_count++;
    a->audio_source[id] = *audio;
  }
  return id;
}

void audio_engine_play_audio_once(i32 audio_id, u32 bus_id, f32 gain) {
  assert(bus_id < MAX_BUS && audio_id < MAX_AUDIO_SOURCE);
  if (audio_id < 0) {
    return;
  }
  Bus* bus = &a->buses[bus_id];
  if (bus->audio_count < MAX_AUDIO_STATE) {
    bus->audio_states[bus->audio_count] = (Audio_state) {
      .id = audio_id,
      .sample_index = 0,
      .reached_end = 0,
    };
    bus->gain = gain;
    bus->audio_count++;
  }
}

void audio_engine_state_free() {
  zone_free(a->bus_buffer_memory);
  for (u32 i = 0; i < a->audio_source_count; ++i) {
    Audio_source* audio = &a->audio_source[i];
    audio_unload(audio);
  }
  a->audio_source_count = 0;
}

void audio_engine_terminate() {
  audio_engine_state_free();
  audio_engine_exit();
}
