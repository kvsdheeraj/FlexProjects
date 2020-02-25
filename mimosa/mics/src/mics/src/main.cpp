#include <cstdio>
#include "pcmUtil.h"
#include "commonDefines.h"
#include "BinarySemaphoreServer.h"
#include "BinarySemaphoreClient.h"
#include <semaphore.h>
#include "audioRecordUtil.h"
#include "sharedMemAudio.h"

//BinarySemaphoreClient g_semCli;

void receiveAudioLoop()
{
    pcm *inputAudioHandle = NULL;

    unsigned int idx = 0;
    //SharedMemAudio_t *g_audioBufferPtr = getSharedMemAudio();
    unsigned int bufferBytes = MICS_ALL_CHANNELS_BUFFER_SIZE * sizeof(int);

    inputAudioHandle = openInputAudio(
        MICS_DEVICE_CARD,
        MICS_DEVICE_NUM,
        MICS_PDM_CHANNELS,
        MICS_PDM_SAMPLE_RATE
        );

    if( inputAudioHandle == NULL )
    {
        return;
    }

    for(;;)
    {
       /* readMic(
            inputAudioHandle,
            g_audioBufferPtr->mics[idx],
            MICS_NUMBER_OF_FRAMES,
            MICS_DEVICE_NAME
        );*/

        /*g_semCli.lock(MICS_SEM_NUM_FOR_MIC_INPUT);
        g_audioBufferPtr->micsBlocksCount++;
        g_audioBufferPtr->micsSlotIdx = idx;
        g_semCli.unlock(MICS_SEM_NUM_FOR_MIC_INPUT);*/

        idx = (idx + 1) % MICS_SHM_SLOTS;
    }
}

int main()
{
   // g_semCli.setFileToKey(MICS_IPC_ID_FILE);
   // g_semCli.tryToOpenUntilSuccess();

  receiveAudioLoop();

	return 0;
}
