#include "audio.h"

//#define DEBUG

struct AudioStuff audio;

// TODO: Remove fprints and useless error checks for non-debug builds
// Connects to the sound device using ALSA and sets all required configurations
int initAudio()
{
    int i;
    int err;
    audio.alsa_device = "default";
    audio.alsa_card = 0;
    audio.alsa_handle;
    audio.position = 0;

    // Access the soundcard
#ifdef DEBUG
    if (snd_pcm_open(&audio.alsa_handle, audio.alsa_device, SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
        fprintf(stderr, "Error opening alsa device: %s\n", snd_strerror(err));
        return -1;
    }
#else
    snd_pcm_open(&audio.alsa_handle, audio.alsa_device, SND_PCM_STREAM_PLAYBACK, 0);
#endif

    // Set paremeters for the device
#ifdef DEBUG
    if ((err = snd_pcm_set_params(audio.alsa_handle, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 48000, 1, 5000000)) < 0)
    {
        fprintf(stderr, "Error setting alsa parameters: %s\n", snd_strerror(err));
        return -1;
    }
#else
    snd_pcm_set_params(audio.alsa_handle, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 48000, 1, 5000000);
#endif

    return 0;
}

int playSound(u_int8_t* samples, unsigned int bytes)
{
    // Send data for ALSA
    while (audio.position < bytes)
    {
        int written, blocksize;

        if (bytes-audio.position < 4096)
        {
            blocksize = bytes-audio.position;
        }
        else
        {
            blocksize = 4096;
        }

        // Write to the device
        written = snd_pcm_writei(audio.alsa_handle, &samples[audio.position], blocksize);

        // If ALSA can't take more data, try again later. Return to the main loop
        if (written == -EAGAIN)
        {
            written = 0;
            return 10;
        }
#ifdef DEBUG
        else
        {
            if (written < 0)
            {
                fprintf(stderr, "Error at writing to the sound device\n");
                snd_pcm_close(audio.alsa_handle);
                return -1;
            }
        }
#endif
        // Move forwards in the sound data buffer
        audio.position += written;
    }
    return 0;
}

void closeAudio()
{
    snd_pcm_close(audio.alsa_handle);
}

