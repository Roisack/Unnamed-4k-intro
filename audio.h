#include <alsa/asoundlib.h>

struct AudioStuff
{
    char* alsa_device;                      // Name of the device used
    unsigned int alsa_card;                 // Number of the card used
    snd_pcm_t* alsa_handle;                 // A handle for talking with ALSA
    unsigned int position;                  // Position in the current buffer
    unsigned int written;
};

int initAudio();
int playSound(unsigned int note, unsigned int duration);
void closeAudio();

