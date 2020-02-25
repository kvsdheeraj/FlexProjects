#pragma once

#include <cstdio>
#include <cstring>

#define CONTROL_FILE_NAME_SIZE      128
#define RECORD_FILE_PREFIX_SIZE     32
#define MAX_RECORD_CHANNELS         8
#define MAX_FILESYSTEM_USAGE        95

enum RecordingMode
{
    AUDIO_REC_16BIT_MODE,
    AUDIO_REC_32BIT_MODE
};

typedef struct audioRecordHandle_t
{
    char controlFileName[CONTROL_FILE_NAME_SIZE];
    char fileNamePrefix[RECORD_FILE_PREFIX_SIZE];
    char fileDirectory[CONTROL_FILE_NAME_SIZE];
    FILE *fp[MAX_RECORD_CHANNELS];
    size_t numberOfFrames;
    bool recordingEnabled;
    bool previousRecordingEnabled;
    size_t count;
    size_t maxCount;
    size_t numberOfChannels;
    enum RecordingMode recMode;
} AudioRecordHandle_t;

AudioRecordHandle_t *createAudioRecord(
    const char *controlFileName,
    const char *filePrefix,
    const char *fileDirectory,
    size_t numOfFrames,
    size_t numberOfChannels,
    enum RecordingMode recMode,
    size_t sampleRate,
    size_t maxSecondsPerFile);
    
void setAudioRecordDirectory(
    AudioRecordHandle_t *audioRecordHandle,
    const char *directory);
    
void setRecordingMode(
    AudioRecordHandle_t *audioRecordHandle,
    RecordingMode recMode);

void setAudioRecordControlFileName(
    AudioRecordHandle_t *audioRecordHandle, 
    const char *fileName);
    
void setAudioRecordPrefixName(
    AudioRecordHandle_t *audioRecordHandle,
    const char *prefix);
    
int getFileNextSeqNumber(
    AudioRecordHandle_t *audioRecordHandle);
    
void enableRecording(
    AudioRecordHandle_t *audioRecordHandle,
    bool recordingEnabled);
    
void recordAudio(
    AudioRecordHandle_t *audioRecordHandle,
    int **channelsBuffer);
    
int getFilesystemUsage();
