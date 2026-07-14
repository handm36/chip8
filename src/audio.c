#include "audio.h"
#include <SDL3/SDL.h>

static SDL_AudioStream *stream = NULL;
static int current_sine_sample = 0;

void play_buzzer() {
  const int minimum_audio = (8000 * sizeof(float)) / 2;

  if (SDL_GetAudioStreamQueued(stream) < minimum_audio) {
    static float samples[512];
    int i;

    for (i = 0; i < SDL_arraysize(samples); i++) {
      const int freq = 440;
      const float phase = current_sine_sample * freq / 8000.0f;
      samples[i] = SDL_sinf(phase * 2 * SDL_PI_F);
      current_sine_sample++;
    }

    current_sine_sample %= 8000;

    SDL_PutAudioStreamData(stream, samples, sizeof(samples));
  }
}

void stop_buzzer() { SDL_ClearAudioStream(stream); }

int init_buzzer() {
  SDL_AudioSpec spec;

  spec.channels = 1;
  spec.format = SDL_AUDIO_F32;
  spec.freq = 8000;
  stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec,
                                     NULL, NULL);
  if (!stream) {
    SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_ResumeAudioStreamDevice(stream);
  return SDL_APP_CONTINUE;
}
