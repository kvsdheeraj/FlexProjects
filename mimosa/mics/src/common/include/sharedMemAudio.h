#pragma once

#include "audioRecordUtil.h"
#include "commonDefines.h"

typedef struct sharedMemAudio_t 
{
    int mics[MICS_SHM_SLOTS][MICS_ALL_CHANNELS_BUFFER_SIZE];
    int speaker[SPEAKERS_SHM_SLOTS][SPEAKERS_NUMBER_OF_FRAMES];
    int micsBlocksCount;
    int micsSlotIdx;
    int speakerBlocksCount;
    int speakerSlotIdx;
    
    bool recordAudioEnabled;
    bool baypassDSP;
    
    
} SharedMemAudio_t ;

SharedMemAudio_t *getSharedMemAudio();
SharedMemAudio_t *createSharedMemAudio();

