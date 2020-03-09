#pragma once

#include "asoundlib.h"
#include "commonDefines.h"
    
struct pcm *openInputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate);
    
/*unsigned int readMic(
    struct pcm *pcm, 
    char *buffer,
    unsigned int size);*/

//void closeMic(struct pcm *pcm);

bool splitSamples(
    int *inputBuffer, 
    int **splittedBuffer);
    
struct pcm *openOutputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate);
