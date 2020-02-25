#include <cstdio>
#include "pcmUtil.h"
#include "commonDefines.h"
#include "BinarySemaphoreServer.h"
#include "BinarySemaphoreClient.h"
#include <semaphore.h>
#include "audioRecordUtil.h"
//#include "sharedMemAudio.h"
//BinarySemaphoreServer g_semSvr;
//BinarySemaphoreClient g_semCli;

int main(int argc, char **argv)
{    
    //SharedMemAudio_t *audioBufferPtr = NULL;
    
    if( argc == 2 )
    {
        //audioBufferPtr = getSharedMemAudio();
        //audioBufferPtr->baypassDSP = !(audioBufferPtr->baypassDSP);
        return 0;
    }
    
    //audioBufferPtr = createSharedMemAudio();
    
    //audioBufferPtr->recordAudioEnabled = false;
    
    //g_semSvr.setFileToKey(MICS_IPC_ID_FILE);
    //g_semSvr.create(MICS_SEM_ARRAY_SIZE);

    
    //audioBufferPtr->micsBlocksCount = 0;
    //audioBufferPtr->micsSlotIdx = 0;
    //audioBufferPtr->baypassDSP = false;
    sleep(3);
    
    for(;;)
    {
        if( getFilesystemUsage() > MAX_FILESYSTEM_USAGE )
        {
            //audioBufferPtr->recordAudioEnabled = false;
        }
        else if(getFilesystemUsage() < MAX_FILESYSTEM_USAGE - 20) 
        {
            //audioBufferPtr->recordAudioEnabled = true;
        }

        sleep(1);
    }
	return 0;
}
