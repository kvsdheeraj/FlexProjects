#include "sharedMemAudio.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

SharedMemAudio_t *createSharedMemAudio()
{
    static SharedMemAudio_t *sharedMemAudio = NULL;
    /*
    if( sharedMemAudio == NULL )
    {    
        key_t sharedKey = -1;
        void *shMemPtr = NULL;
        key_t sharedMemoryID;
        
        sharedKey = 
            ftok(MICS_IPC_ID_FILE, MICS_IPC_ID_CODE);
            
        sharedMemoryID = 
            shmget( 
                sharedKey, 
                sizeof(SharedMemAudio_t), 
                IPC_CREAT | 0600
            );
    
        if( sharedMemoryID < 0 )
        {
            fprintf(
                stderr,
                "Error creating audio reader shared memory\n"
            );
                    
            sharedMemoryID = 
                shmget(
                    sharedKey , 0,0 );
        }
        
        shMemPtr = shmat(sharedMemoryID, 0, 0);
        sharedMemAudio = (SharedMemAudio_t *)shMemPtr;
    }*/
    
    return sharedMemAudio;
}

SharedMemAudio_t *getSharedMemAudio()
{
    static SharedMemAudio_t *sharedMemAudio = NULL;
    /*
    if( sharedMemAudio == NULL )
    {
        key_t sharedKey = -1;
        key_t sharedMemoryID;
        void *shMemPtr = NULL;
        
        sharedKey = 
            ftok(MICS_IPC_ID_FILE, MICS_IPC_ID_CODE);
            
        sharedMemoryID = 
            shmget( sharedKey, 0, 0 );
            
        for(;;)
        {
            if( sharedMemoryID >= 0 ) break;
            sleep(1);
            sharedMemoryID = 
                shmget( sharedKey, 0, 0 );
        }
        
        shMemPtr = shmat(sharedMemoryID, 0, 0);
        sharedMemAudio = (SharedMemAudio_t *)shMemPtr;
    }*/
    return sharedMemAudio;
}

