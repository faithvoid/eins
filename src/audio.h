#ifndef AUDIO_H
#define AUDIO_H

#include <SDL3/SDL.h>
#include <stdio.h>

// nxdk shim for dr_mp3 
#if defined(_MSC_VER)
typedef int errno_t;

static inline errno_t fopen_s(FILE **pFile, const char *filename, const char *mode) {
    if (!pFile) return 22;
    *pFile = fopen(filename, mode);
    return (*pFile != NULL) ? 0 : 2;
}

static inline errno_t _wfopen_s(FILE **pFile, const void *filename, const void *mode) {
    (void)filename; (void)mode;
    if (pFile) *pFile = NULL;
    return 22;
}
#endif
//

#include "dr_mp3.h"

typedef struct {
    drmp3 mp3;
    int isLooping;
    int isInitialized;
    SDL_AudioStream *resampler;
} MusicStream;

typedef struct {
    SDL_AudioDeviceID deviceId;
    SDL_AudioDeviceID micDeviceId;
    SDL_AudioStream *outputStream;
    SDL_AudioStream *sfxStream;
    SDL_AudioStream *micStream;
    SDL_AudioStream *voipStream;
    SDL_AudioSpec spec;
    MusicStream *activeStream;
} SoundManager;

int audio_init(SoundManager *audio);
void audio_close(SoundManager *audio);

MusicStream* audio_stream_open(const char *filepath, int loop);
void audio_stream_update(SoundManager *audio);
void audio_play_music_stream(SoundManager *audio, const char *filepath, int loops);
void audio_stop_music_stream(SoundManager *audio);

Uint8* audio_load_sfx(SoundManager *audio, const char *filepath, Uint32 *len);
void audio_play_sfx(SoundManager *audio, const Uint8 *sfxBuf, int sfxLen);

int  audio_init_voip(SoundManager *audio);
int  audio_capture_voip(SoundManager *audio, void *outBuf, int maxLen);
void audio_play_voip(SoundManager *audio, const void *inBuf, int len);

#endif
