// audio.h

#ifndef _AUDIO_H
#define _AUDIO_H

typedef struct Audio_source {
  f32* buffer;
  i32 sample_count;
  i32 sample_rate;
  i32 channel_count;
} Audio_source;

i32 audio_load_from_pack(const char* path, const char* pack_file, Audio_source* audio);

void audio_unload(Audio_source* audio);

i32 audio_convert_to_f32_buffer(f32* restrict out_buffer, i16* restrict in_buffer, u32 sample_count);

i32 audio_convert_to_i16_buffer(i16* restrict out_buffer, f32* restrict in_buffer, u32 sample_count);

void audio_zero_f32_buffer(f32* buffer, u32 size /* size in bytes */);

void audio_sum_f32_buffers(f32* out_buffer, f32* in_a, f32* in_b, u32 size);

#endif // _AUDIO_H
