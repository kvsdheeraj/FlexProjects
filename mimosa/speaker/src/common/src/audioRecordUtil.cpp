#include "audioRecordUtil.h"
#include "fileSystemUtil.h"
#include <stdlib.h>
#include <stddef.h>

AudioRecordHandle_t *createAudioRecord(
    const char *controlFileName,
    const char *filePrefix,
    const char *fileDirectory,
    size_t numberOfFrames,
    size_t numberOfChannels,
    enum RecordingMode recMode,
    size_t sampleRate,
    size_t maxSecondsPerFile)
{
    AudioRecordHandle_t *newAudioRecordHandle = (AudioRecordHandle_t *)malloc(
        sizeof(AudioRecordHandle_t)
        );
    
    setAudioRecordControlFileName( newAudioRecordHandle, controlFileName );
    setAudioRecordPrefixName( newAudioRecordHandle, filePrefix );
    setAudioRecordDirectory( newAudioRecordHandle, fileDirectory );
    newAudioRecordHandle->numberOfFrames = numberOfFrames;
    newAudioRecordHandle->numberOfChannels = numberOfChannels;
    newAudioRecordHandle->recMode = recMode;
    newAudioRecordHandle->recordingEnabled = false;
    newAudioRecordHandle->previousRecordingEnabled = false;
    newAudioRecordHandle->count = 0;
    newAudioRecordHandle->maxCount = maxSecondsPerFile * sampleRate / numberOfFrames;
    
    for(int i = 0; i < MAX_RECORD_CHANNELS; i++)
    {
        newAudioRecordHandle->fp[i] = NULL;
    }
    
    return newAudioRecordHandle;
}

void setAudioRecordControlFileName(
    AudioRecordHandle_t *audioRecordHandle, 
    const char *fileName)
{
    strncpy(
        audioRecordHandle->controlFileName, 
        fileName,
        CONTROL_FILE_NAME_SIZE);
}

void enableRecording(
    AudioRecordHandle_t *audioRecordHandle,
    bool recordingEnabled)
{
    audioRecordHandle->recordingEnabled = recordingEnabled;
}

void setAudioRecordPrefixName(
    AudioRecordHandle_t *audioRecordHandle,
    const char *prefix)
{
    strncpy(
        audioRecordHandle->fileNamePrefix, 
        prefix,
        RECORD_FILE_PREFIX_SIZE );
}

void setAudioRecordDirectory(
    AudioRecordHandle_t *audioRecordHandle,
    const char *directory)
{
    strncpy(
        audioRecordHandle->fileDirectory, 
        directory,
        CONTROL_FILE_NAME_SIZE );
}

int getFileNextSeqNumber(
    AudioRecordHandle_t *audioRecordHandle)
{
    FILE *fp = NULL;
    int seqno = 0;
        
    fp = fopen( audioRecordHandle->controlFileName, "r+" );
    
    if(fp == NULL)
    {
        fp = fopen( audioRecordHandle->controlFileName, "w" );
        if(fp != NULL)
        {
            fwrite(&seqno, sizeof(int), 1, fp);
            fclose(fp);
        }
    }
    else
    {
        fread( &seqno, sizeof(int), 1, fp );        
        seqno++;
        fseek ( fp , 0 , SEEK_SET );
        fwrite(&seqno, sizeof(int), 1, fp);
        fclose(fp);
    }
    
    return seqno;
}
void setRecordingMode(
    AudioRecordHandle_t *audioRecordHandle,
    RecordingMode recMode)
{
    audioRecordHandle->recMode = recMode;
}

void recordAudio(
    AudioRecordHandle_t *audioRecordHandle,
    int **channelsBuffer)
{
    if( !audioRecordHandle->recordingEnabled )
    {
        audioRecordHandle->previousRecordingEnabled = false;
        return;
    }
    
    if( !audioRecordHandle->previousRecordingEnabled )
    {
        audioRecordHandle->count = 0;
        
        for(
            int channel = 0; 
            channel < audioRecordHandle->numberOfChannels;
            channel++ )
        {
            if(audioRecordHandle->fp[channel] != NULL)
            {
                fclose(audioRecordHandle->fp[channel]);
                audioRecordHandle->fp[channel] = NULL;
            }
        }
    }
    audioRecordHandle->previousRecordingEnabled = true;
    
    if(audioRecordHandle->count == 0)
    {    
        int seqno = getFileNextSeqNumber(audioRecordHandle);
        for(
            int channel = 0; 
            channel < audioRecordHandle->numberOfChannels;
            channel++ )
        {
            char filename[CONTROL_FILE_NAME_SIZE];
            sprintf(filename, "%s/%s_s%02d_c%02d.pcm",
                audioRecordHandle->fileDirectory,
                audioRecordHandle->fileNamePrefix,
                seqno,
                channel + 1 );
                
            audioRecordHandle->fp[channel] = 
                fopen( 
                    filename,
                    "wb" );
        }
    }
    
    for(
        int channel = 0; 
        channel < audioRecordHandle->numberOfChannels;
        channel++ )
    {
        size_t size = 0;
        
        if( audioRecordHandle->recMode == AUDIO_REC_16BIT_MODE )
        {
            size = audioRecordHandle->numberOfFrames * sizeof(short);
        }
        else
        {
            size = audioRecordHandle->numberOfFrames * sizeof(int);
        }
        if(audioRecordHandle->fp[channel] != NULL)
        {
            fwrite(
                channelsBuffer[channel], 
                size,
                1, 
                audioRecordHandle->fp[channel]);
        }
    }
    
    if( audioRecordHandle->count == audioRecordHandle->maxCount )
    {
        audioRecordHandle->count = 0;
        
        for(
            int channel = 0; 
            channel < audioRecordHandle->numberOfChannels;
            channel++ )
        {
            if(audioRecordHandle->fp[channel] != NULL)
            {
                fclose(audioRecordHandle->fp[channel]);
                audioRecordHandle->fp[channel] = NULL;
            }
        }
    }
    else
    {
        audioRecordHandle->count++;
    }
}
