#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include "pcmUtil.h"
#include "asoundlib.h"

int check_param(struct pcm_params *params, unsigned int param, unsigned int value,
                 char *param_name, const char *param_unit)
{
    unsigned int min;
    unsigned int max;
    int is_within_bounds = 1;

    min = pcm_params_get_min(params, (pcm_param) param);
    if (value < min) {
        fprintf(stderr, "%s is %u%s, device only supports >= %u%s\n", param_name, value,
                param_unit, min, param_unit);
        is_within_bounds = 0;
    }

    max = pcm_params_get_max(params, (pcm_param) param);
    if (value > max) {
        fprintf(stderr, "%s is %u%s, device only supports <= %u%s\n", param_name, value,
                param_unit, max, param_unit);
        is_within_bounds = 0;
    }

    return is_within_bounds;
}

int sample_is_playable(unsigned int card, unsigned int device, unsigned int channels,
                        unsigned int rate, unsigned int bits, unsigned int period_size,
                        unsigned int period_count)
{
    struct pcm_params *params;
    int can_play;

    params = pcm_params_get(card, device, PCM_OUT);
    if (params == NULL) {
        fprintf(stderr, "Unable to open PCM device %u.\n", device);
        return 0;
    }

    can_play = check_param(params, PCM_PARAM_RATE, rate, "Sample rate", "Hz");
    can_play &= check_param(params, PCM_PARAM_CHANNELS, channels, "Sample", " channels");
    can_play &= check_param(params, PCM_PARAM_SAMPLE_BITS, bits, "Bitrate", " bits");
    can_play &= check_param(params, PCM_PARAM_PERIOD_SIZE, period_size, "Period size", " frames");
    can_play &= check_param(params, PCM_PARAM_PERIODS, period_count, "Period count", " periods");

    pcm_params_free(params);

    return can_play;
}

struct pcm *openOutputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate)
{
    struct pcm_config config;
    struct pcm *pcm;
    unsigned int period_size = 1024;
    unsigned int period_count = 4;

    memset(&config, 0, sizeof(config));
    config.channels = numOfChannels;
    config.rate = sampleRate;
    config.period_size = period_size;
    config.period_count = period_count;
    config.format = PCM_FORMAT_S32_LE; //PCM_FORMAT_S24_3LE; //PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    if (!sample_is_playable(card, device, numOfChannels, sampleRate, PCM_FORMAT_S32_LE, period_size, period_count)) {
        return 0;
    }

    pcm = pcm_open(card, device, PCM_OUT, &config);
    if (!pcm || !pcm_is_ready(pcm)) {
        fprintf(stderr, "Unable to open PCM device %u (%s)\n",
                device, pcm_get_error(pcm));
        return 0;
    }

    fprintf(stderr, "Successfully opened PCM card :%u  device :%u\n",
                card,device);
    return pcm;
}

struct pcm *openInputAudio(
    unsigned int device,
    unsigned int card,
    int numOfChannels,
    unsigned int sampleRate)
{
    struct pcm_config config;
    struct pcm *pcm;
    unsigned int period_size = 1024;
    unsigned int period_count = 4;

    memset(&config, 0, sizeof(config));
    config.channels = numOfChannels;
    config.rate = sampleRate;
    config.period_size = period_size;
    config.period_count = period_count;
    config.format = PCM_FORMAT_S32_LE; //PCM_FORMAT_S24_3LE; //PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    /*if (!sample_is_playable(card, device, numOfChannels, sampleRate, PCM_FORMAT_S32_LE, period_size, period_count)) {
        return;
    }*/

    pcm = pcm_open(card, device, PCM_IN, &config);
    if (!pcm || !pcm_is_ready(pcm)) {
        fprintf(stderr, "Unable to open PCM device %u (%s)\n",
                device, pcm_get_error(pcm));
        return 0;
    }

    fprintf(stderr, "Successfully opened PCM card :%u  device :%u\n",
                card,device);

    return pcm;
}

/*
void readMic(
    snd_pcm_t *micHandle, 
    void *inputBuffer,
    snd_pcm_uframes_t numberOfFrames,
    const char *deviceName)
{
    int err = 0;
    static char errorMessage1[256] = {'\0'};
    static char errorMessage2[256] = {'\0'};
    
    err = snd_pcm_readi(
        micHandle, 
        inputBuffer,
        numberOfFrames);
        
    if( err < 0 )
    {
        if (err == -EPIPE)
        {
            snprintf(
                errorMessage1,
                sizeof errorMessage1,
                "Error on snd_pcm_readi. PCM device: <%s>, error:<%s>\n",
                deviceName,
                snd_strerror(err)
            );
        }
        
        err = snd_pcm_recover(
            micHandle,
            err, 
            1       //silent mode
        );
        if (err < 0)
        {
            snprintf(
                errorMessage2,
                sizeof errorMessage1,
                "Error on snd_pcm_readi. PCM device: <%s>, error:<%s>\n",
                deviceName,
                snd_strerror(err)
            );
        }
    }

    if(errorMessage1[0] != '\0')
    {
        fprintf( stderr, errorMessage1 );
        errorMessage1[0] = '\0';
    }
    if(errorMessage2[0] != '\0')
    {
        fprintf( stderr, errorMessage2 );
        errorMessage2[0] = '\0';
    }
}
*/

bool splitSamples(int *inputBuffer, int **splittedBuffer)
{
    static int channelItemIndex = -1;
    int inputBufferIndex = 0;
    int channelIndex = 0;
        
    for(
        inputBufferIndex = 0;
        inputBufferIndex < MICS_ALL_CHANNELS_BUFFER_SIZE;
        inputBufferIndex++)
    {
        channelIndex = inputBufferIndex % MICS_PDM_CHANNELS;
        if(channelIndex == 0)
        {
            channelItemIndex++;
        }
        
        splittedBuffer[channelIndex][channelItemIndex] = 
            inputBuffer[inputBufferIndex];

        if(
            channelItemIndex + 1 == MICS_NUMBER_OF_FRAMES && 
            channelIndex + 1 == MICS_PDM_CHANNELS)
        {
            channelItemIndex = -1;
                                    
            return true;
        }
    }
        
    return false;
}
