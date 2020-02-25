#pragma once

#include "asoundlib.h"
#include "commonDefines.h"
    
struct pcm *openInputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate);
    
/*void readMic(
    snd_pcm_t *micHandle, 
    void *inputBuffer,
    snd_pcm_uframes_t numberOfFrames,
    const char *deviceName);*/

bool splitSamples(
    int *inputBuffer, 
    int **splittedBuffer);
    
struct pcm *openOutputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate);
