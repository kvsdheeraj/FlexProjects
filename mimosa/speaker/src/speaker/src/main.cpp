#include <cstdio>
#include "pcmUtil.h"
#include "commonDefines.h"
#include "BinarySemaphoreServer.h"
#include "BinarySemaphoreClient.h"
#include <semaphore.h>
#include "audioRecordUtil.h"
#include "sharedMemAudio.h"

#define WRITE_DEVICE_NUM    0
#define WRITE_DEVICE_CARD   0
#define OUTPUT_CHANNELS     2
#define OUTPUT_SAMPLE_RATE  16000  
#define WRITE_BUFFER_SIZE SPEAKERS_NUMBER_OF_FRAMES * OUTPUT_CHANNELS

//BinarySemaphoreClient g_semCli;

void speakerProcess()
{
    //SharedMemAudio_t *g_audioBufferPtr = getSharedMemAudio();
    
    //int curIdx = 0;
    int referenceSlotIdx = 0;
    int slotIndex = 0;

    int pcmReadyForIO = 0;
    int countRecoveries = 0;
    int countPrintDelay = 20000;
    int countNotRightState = 0;
    int countUnderRun = 0;
    int countSuspendEvent = 0;
    int countRecover = 0;
    unsigned int writeReturn = 0;
    unsigned int recoverReturn = 0;
    unsigned int recoveryFramesWritten = 0;
    bool started = false;
    pcm *handleWrite = NULL;
    static int bufferWrite[WRITE_BUFFER_SIZE];
    
    for(;;)
    {          
        /*g_semCli.lock(SPEAKERS_SEM_NUM);
        if(referenceSlotIdx != g_audioBufferPtr->speakerSlotIdx)
        {
            referenceSlotIdx = g_audioBufferPtr->speakerSlotIdx;
        }        
        g_semCli.unlock(SPEAKERS_SEM_NUM);*/
        
        countPrintDelay--;
        if(countPrintDelay == 0)
        {
            if(countRecoveries > 0)
            {
                printf(
                    "\n\rec ct=%d, recFrWr=%d, norRtSt=%d, under=%d, suspevt=%d\n\n", 
                    countRecoveries, recoveryFramesWritten,
                    countNotRightState, countUnderRun, 
                    countSuspendEvent
                    );
                countRecoveries = 0;
            }
            countPrintDelay = 20000;
        }
        
        if( slotIndex != referenceSlotIdx )
        {
            if(!started)
            {
                started = true;
                handleWrite = openOutputAudio(
                    WRITE_DEVICE_NUM,
                    WRITE_DEVICE_CARD,
                    OUTPUT_CHANNELS,
                    OUTPUT_SAMPLE_RATE); 
                slotIndex = referenceSlotIdx;
            }
            else
            {
                slotIndex = (slotIndex + 1) % SPEAKERS_SHM_SLOTS;
            }
        } 
        else if(started)
        {
            if(pcmReadyForIO)
            {
                writeReturn = 
                    pcm_write(
                        handleWrite, 
                        &bufferWrite[0], 
                        SPEAKERS_NUMBER_OF_FRAMES);

                ++countRecoveries;
                slotIndex = referenceSlotIdx;
                                
                if(writeReturn < 0)
                {
                    //recoverReturn = snd_pcm_recover(
                     //   handleWrite, 
                     //   writeReturn,
                     //   1 );
                    if( recoverReturn >= 0 )
                    {
                        countRecover++;
                    }
                }

                switch(writeReturn)
                {
                    case -EPIPE:
                    countUnderRun++;
                    break;
                    case -EBADFD:
                    countNotRightState++;
                    break;
                    case -ESTRPIPE:
                    countSuspendEvent++;
                    break;
                    default:
                    recoveryFramesWritten += writeReturn;
                }
                
                pcmReadyForIO = 0;
            }
            else
            {
                pcmReadyForIO = 
                    pcm_wait(
                        handleWrite,
                        -1);
            }
            continue;
        }
        else
        {
            usleep(1000);
            continue;
        }

        pcmReadyForIO = 0;
        recoveryFramesWritten = 0;
        countNotRightState = 0;
        countUnderRun = 0;
        countSuspendEvent = 0;
        countRecover = 0;
                
        /*for( int i = 0, j = 0;
            i < WRITE_BUFFER_SIZE; 
            i+= 2, j++)
        {
            bufferWrite[i]      = g_audioBufferPtr->speaker[slotIndex][j];
            bufferWrite[i+1]    = g_audioBufferPtr->speaker[slotIndex][j];
        }*/
        
		int err = pcm_write(handleWrite, &bufferWrite[0], SPEAKERS_NUMBER_OF_FRAMES);
		if (err == -EPIPE) printf( "Oh My, Underrun occurred from write: %d\n", err);
		if (err < 0)
        {
			//err = snd_pcm_recover(handleWrite, err, 1);
                                    		
			//if (err < 0)
			//{
				printf( "Error occured while writing:\n");
                pcm_close(handleWrite);
				return;
			//}
            //g_semCli.lock(SPEAKERS_SEM_NUM);
            //referenceSlotIdx = g_audioBufferPtr->speakerSlotIdx;
           // slotIndex = referenceSlotIdx;
           // g_semCli.unlock(SPEAKERS_SEM_NUM);
		}
        memset( bufferWrite, 0, sizeof bufferWrite );
    }    
}

int main()
{
   // g_semCli.setFileToKey(MICS_IPC_ID_FILE);
   // g_semCli.tryToOpenUntilSuccess();
    
    speakerProcess();
    return 0;
}

