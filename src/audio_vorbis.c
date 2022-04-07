// audio_vorbis.c

#include "external/stb_vorbis.c"

i32 audio_vorbis_load_from_buffer(const char* path, Buffer* data, Audio_source* audio) {
  i32 result = NO_ERR;
  i32 channel_count = 0;
  i32 sample_rate = 0;
  i16* buffer = NULL;

  i32 sample_count = stb_vorbis_decode_memory((const u8*)&data->data[0], data->size, &channel_count, &sample_rate, &buffer);
  if (sample_count < 0) {
    fprintf(stderr, "audio_vorbis_load_from_buffer: Failed to decode file `%s`\n", path);
    return ERR;
  }
  audio->sample_count = sample_count * channel_count;
  audio->sample_rate = sample_rate;
  audio->channel_count = channel_count;
  audio->buffer = zone_malloc(audio->sample_count * sizeof(f32));
  if (!audio->buffer) {
    fprintf(stderr, "audio_vorbis_load_from_buffer: Failed to allocate memory for audio buffer\n");
    free(buffer);
    return ERR;
  }
  audio_convert_to_f32_buffer(audio->buffer, buffer, audio->sample_count);
  free(buffer);
  return result;
}
