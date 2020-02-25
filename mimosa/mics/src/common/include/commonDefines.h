#pragma once

#define MICS_DEVICE_NUM                     0
#define MICS_DEVICE_CARD                    0
#define MICS_PDM_CHANNELS                   8
#define MICS_PDM_SAMPLE_RATE                16000
#define MICS_NUMBER_OF_FRAMES               1536
#define MICS_ALL_CHANNELS_BUFFER_SIZE       MICS_PDM_CHANNELS * MICS_NUMBER_OF_FRAMES

#define MICS_DEBUG_ENABLING_FILE            "/etc/debugmicr"
#define MICS_IPC_ID_FILE                    "/etc/passwd"
#define MICS_IPC_ID_CODE                    20
#define MICS_SEM_ARRAY_SIZE                 3
#define MICS_SHM_SLOTS                      4
#define SPEAKERS_SHM_SLOTS                  4

#define MICS_SEM_NUM_FOR_MIC_INPUT          0
#define SPEAKERS_SEM_NUM                    1
#define SPEAKERS_NUMBER_OF_FRAMES           1536
#define SPEAKER_CHANNELS                    2
#define SPEAKER_CHANNELS_BUFFER_SIZE        SPEAKER_CHANNELS * SPEAKERS_NUMBER_OF_FRAMES
#define DSP_FFT_SIZE                        1024
#define DSP_INPUT_BLOCK_SIZE                512
#define DSP_FFT_BLOCKS                      4
#define DSP_FFT_DATA_SIZE                   (DSP_FFT_SIZE/2)+1
#define DSP_ALIGNED_ALLOC_SIZE              64 * 1024
