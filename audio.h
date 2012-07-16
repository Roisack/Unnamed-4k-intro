#include <alsa/asoundlib.h>

struct AudioStuff
{
    char* alsa_device;                      // Name of the device used
    unsigned int alsa_card;                 // Number of the card used
    snd_pcm_t* alsa_handle;                 // A handle for talking with ALSA
    unsigned int position;                  // Position in the current buffer
    unsigned int samples[16*4096];          // Playback buffer containing actual music
};

int initAudio();
int playSound(u_int8_t* samples, unsigned int bytes);
void closeAudio();

