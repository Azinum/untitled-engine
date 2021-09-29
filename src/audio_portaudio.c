// audio_portaudio.c

#include <portaudio.h>

static PaStream* stream = NULL;
static PaStreamParameters out_port;

static i32 open_stream();
static i32 stereo_callback(const void* in_buffer, void* out_buffer, unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags flags, void* user_data);

i32 open_stream(Audio_engine* a) {
  PaError err = Pa_OpenStream(
    &stream,
    NULL,
    &out_port,
    a->sample_rate,
    a->frames_per_buffer,
    paNoFlag,
    stereo_callback,
    NULL
  );
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "[PortAudio Error]: %s\n", Pa_GetErrorText(err));
    return ERR;
  }
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "[PortAudio Error]: %s\n", Pa_GetErrorText(err));
    return ERR;
  }
  return NO_ERR;
}

i32 stereo_callback(const void* in_buffer, void* out_buffer, unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags flags, void* user_data) {
  if (audio_engine_process(out_buffer) == NO_ERR) {
    audio_engine_process(out_buffer);
    return paContinue;
  }
  return paComplete;
}

i32 audio_engine_init(Audio_engine* a) {
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "[PortAudio Error]: %s\n", Pa_GetErrorText(err));
    return ERR;
  }

  i32 output_device = Pa_GetDefaultOutputDevice();
  out_port.device = output_device;
  out_port.channelCount = 2;
  out_port.sampleFormat = paFloat32;
  out_port.suggestedLatency = Pa_GetDeviceInfo(out_port.device)->defaultLowOutputLatency;
  out_port.hostApiSpecificStreamInfo = NULL;

  if ((err = Pa_IsFormatSupported(NULL, &out_port, a->sample_rate)) != paFormatIsSupported) {
    fprintf(stderr, "[PortAudio Error]: %s\n", Pa_GetErrorText(err));
    return ERR;
  }
  fprintf(stdout, "Using portaudio audio backend\n");
  return NO_ERR;
}

i32 audio_engine_start(Audio_engine* a) {
  i32 result = open_stream(a);
  if (result != NO_ERR) {
    return result;
  }
  return NO_ERR;
}

void audio_engine_exit() {
  Pa_CloseStream(stream);
  Pa_Terminate();
}
