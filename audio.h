#include <alsa/asoundlib2.h>

struct AudioStuff
{
    char* alsa_device;              // Name of the device used
    snd_pcm_t* alsa_handle;         // A handle for talking with ALSA
    unsigned int position;          // Position in the current buffer
    unsigned int written;           // How much data was sent to audio card
    int currentNote;                // Where in the song we are
    unsigned int buffer[48000];     // Stream buffer
    unsigned int stream_limit;      // How much can be transferred each loop
    unsigned int oldPosition;       // Keeps track for the total progress through a note
};

int initAudio();
int playSound(unsigned int note, unsigned int duration, int modulation);
void streamAudio(unsigned int* notes, unsigned int* noteDurations, int totalNotes, int* instrument);
void closeAudio();

