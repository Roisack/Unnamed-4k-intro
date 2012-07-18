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
    audio.currentNote = 0;

    // Access the soundcard
#ifdef DEBUG
    if (snd_pcm_open(&audio.alsa_handle, audio.alsa_device, SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
        fprintf(stderr, "Error opening alsa device: %s\n", snd_strerror(err));
        return -1;
    }
#else
    //snd_pcm_open(&audio.alsa_handle, audio.alsa_device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
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

// Push some audio to the sound card, and return to the demo when no more can be pushed. Mark down how much was pushed, and continue to the next note if all the note has been played
void streamAudio(unsigned int* music, unsigned int* noteDurations, int totalNotes)
{
    if (audio.currentNote >= totalNotes)
        return;
    
    unsigned int noteLeft = playSound(music[audio.currentNote], noteDurations[audio.currentNote]);
    // A part of the note was sent, so reduce that from the note's leftover duration. We'll continue playing this note the next time the audio card accepts input
    
    fprintf(stderr, "Played note %d. Total left: %d\n", audio.currentNote, noteDurations[audio.currentNote]);

    noteDurations[audio.currentNote] = noteLeft;
    if (noteDurations[audio.currentNote] == 0)
        audio.currentNote++;
}

// note: the amplitude of the wave played
// duration: how many repeats. 48000 == 1 sec
// Returns an integer telling how much of the note was played
// If the entire note was played, then the return value equals duration
// Usually the sound card plays only a part of the note as it can accept
// only so much traffic
int playSound(unsigned int note, unsigned int duration)
{
    unsigned int blocksize = 4096;

    unsigned int buffer[duration];
    unsigned int i;

    // Instrument listing!
    // note + sqrt(i)*note; // Kind of a drum

    for (i = 0; i < duration; i++)
    {
        buffer[i] = note + sqrt(i)*note;// * sqrt(sin(note*i));
    }

    audio.position = 0;

    // Send data for ALSA
    while (audio.position < duration)
    {
        if (duration - audio.position < 4096)
        {
            blocksize = duration-audio.position;
        }
        // Write to the device
        audio.written = snd_pcm_writei(audio.alsa_handle, &buffer[audio.position], blocksize);

        if (audio.written == -EAGAIN)
        {
            audio.written = 0;
            fprintf(stderr, "Cannot write more. Returning %d\n", duration-audio.position);
            return (duration-audio.position);
        } else
        {
            audio.position += audio.written;
        }

#ifdef DEBUG
        if (audio.written < 0)
        {
            fprintf(stderr, "Error at writing to the sound device\n");
            snd_pcm_close(audio.alsa_handle);
            return -1;
        }
#endif
    }
    // The entire note has been played
    return 0;
}

void closeAudio()
{
    snd_pcm_close(audio.alsa_handle);
}

