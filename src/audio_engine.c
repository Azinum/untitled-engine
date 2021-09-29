// audio_engine.c

extern i32 audio_engine_init(Audio_engine* a);

extern i32 audio_engine_start(Audio_engine* a);

extern void audio_engine_exit();

Audio_engine audio_engine = {0};
Audio_engine* a = &audio_engine;

#if USE_PORTAUDIO
  #include "audio_portaudio.c"
#else
  #include "audio_null.c"
#endif

i32 audio_engine_state_init(i32 sample_rate, i32 frames_per_buffer) {
  a->sample_rate = sample_rate;
  a->frames_per_buffer = frames_per_buffer;
  a->out = NULL;
  i32 result = audio_engine_init(a);
  return result == NO_ERR ? audio_engine_start(a) : result;
}

i32 audio_engine_process(void* out_buffer) {
  f32* out = (f32*)out_buffer;

  for (u32 i = 0; i < a->frames_per_buffer; ++i) {
    *out++ = 0;
    *out++ = 0;
  }
  return NO_ERR;
}

void audio_engine_terminate() {
  audio_engine_exit();
}
