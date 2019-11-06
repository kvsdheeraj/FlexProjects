/**
 * @author 
 * @file Description
 * @desc Created on 2019-11-05 4:35:51 pm
 * @copyright Sample Codes : All Rights Reserved.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <ctype.h>
#include "gen2test.h"

#define OUTPUT_PIPE "/home/pi/Desktop/code/playback"

#define PI 3.14159265359

#define NUM_OF_SAMPLES 888
#define GEN_BUFFER_SIZE NUM_OF_SAMPLES * 2
#define SHMEM_ID_FILE "/etc/passwd"
#define SHMEM_CODE 'a'
#define BETA 0.8

key_t g_sharedMemoryID = (key_t)-1;

typedef enum
{
    IDLE,
    Look_Key_Start,
    Look_Key_End,
    Look_Vel_Start,
    Look_Dial_1_A_Start,
    Look_Dial_1_B_Start,
    Look_Dial_2_Start,
    Look_Dial_4_Start,
    Look_Mod_Wheel_Start,
    Look_PB_Wheel_Start,
    Look_PB_Wheel_Stop,
    Look_Slider_Start,
    
} Midi_States;

typedef enum
{
    ATTACK,
    SUSTAIN,
    DECAY,
    RELEASE,

}Control_States;

static unsigned int Midi_State = IDLE;

//You can define the fields here by modifying this one ...
typedef struct mySharedMemoryStructure
{
    short samples[GEN_BUFFER_SIZE];
    int blknum;
    int Realdata;
    int Result;

} MySharedMemoryStructure_t;

MySharedMemoryStructure_t *g_shMemPtr = NULL;

/* Declerations */
void createSharedMemory();

void createSharedMemory()
{
    key_t sharedKey = -1;

    //Get the unique token (key) for identify the shared memory (SHM)
    sharedKey = ftok(
        SHMEM_ID_FILE,
        SHMEM_CODE);

    //Try to attach the shared memory
    g_sharedMemoryID = shmget(sharedKey, 0, 0);

    //The SHM does not exists, so create it
    if (g_sharedMemoryID < 0)
    {
        //Create the shared memory (SHM) with the key
        printf("creating shared memory\n");
        g_sharedMemoryID = shmget(
            sharedKey,
            sizeof(MySharedMemoryStructure_t),
            IPC_CREAT | 0600);

        //attach to the shared memory
        g_shMemPtr = (MySharedMemoryStructure_t *)shmat(g_sharedMemoryID, 0, 0);
    }
    else
    {
        g_shMemPtr = (MySharedMemoryStructure_t *)shmat(g_sharedMemoryID, 0, 0);
    }
}



    float generatepitchbend(char data)
    {
    float temp;
    temp = data; 
    temp = (((temp / 127.0 ) * (1.5)) + 0.5);

    //temp = ((temp-1)/12) + 1;

    if ((temp >= 0.972 ) && (temp <= 1.019 ))
    {
        temp = 1.0 ;
    }

    return temp ;   

    }


static int fd; /* File descriptor for the port */

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
    {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;      /* 8-bit characters */
    tty.c_cflag &= ~PARENB;  /* no parity bit */
    tty.c_cflag &= ~CSTOPB;  /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        printf("error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        printf("error %d setting term attributes", errno);
}

struct KeyStates
{
    char state;
    char key;
    float wsum;
    char vel;
    double freq;
    struct KeyStates *next;
};

typedef struct KeyStates KeyStates;

struct KeyList
{
    int size;
    KeyStates *head;
};

typedef struct KeyList KeyList;

KeyList *create_keylist(void)
{
    KeyList *kl = (KeyList *)malloc(sizeof(KeyList));
    kl->head = NULL;
    kl->size = 0;
    return kl;
}

bool empty_list(KeyList *kl)
{
    return kl->size == 0;
}

void push_keylist(KeyList *kl, char state, char key, char vel)
{
    KeyStates *ks = (KeyStates *)malloc(sizeof(KeyStates));
    ks->key = key;
    ks->wsum = 0;
    ks->state = state;
    ks->vel = vel;
    ks->freq = (double)(440 * pow(2, (key - 69) / 12.0f));
    ks->next = kl->head;
    kl->head = ks;
    kl->size++;
}

// This function prints contents of linked list starting from
// the given node
void print_list(KeyStates *n, int size)
{
    while (n != NULL)
    {
        printf("%d->", n->key);
        n = n->next;
    }

    if (size == 0)
    {
        printf("All keys Released\n");
    }
    printf("\n");
}

// This function prints contents of linked list starting from
// the given node
void call_list(KeyList *kl, int size)
{
    KeyStates *n = kl->head;
    
    while (n != NULL)
    {
        printf("%d->", n->key);
        n = n->next;
    }
    
    if (size == 0)
    {
        printf("All keys Released\n");
    }
    printf("\n");
}

double sum_freq(unsigned char cmd, KeyStates *n, int size)
{
    double avg_result = 0.0;
    static double ret_result = 0.0;
    int count = 0;

    if (size > 0)
    {
        ret_result = 0.0;
    }

    while (n != NULL)
    {
        avg_result = (440 * pow(2, (n->key - 69) / 12.0f));
        ret_result += avg_result;
        n = n->next;
        count++;
    }

    if (size == 0)
    {
        ret_result = 0.0;
    }

    return ret_result;
}

int pop_keylist(KeyList *kl, char key)
{
    if (empty_list(kl))
    {
        printf("\nList is Empty\n");
        return 0;
    }

    int value = 0;
    KeyStates *ks = kl->head, *prev;
    // If head node itself holds the key to be deleted
    if (ks != NULL && ks->key == key)
    {
        value = ks->key;
        kl->head = ks->next; // Changed head
        free(ks);            // free old head
        kl->size--;
        return value;
    }

    while (ks != NULL && ks->key != key)
    {
        prev = ks;
        ks = ks->next;
    }

    if (ks == NULL)
        return 0;

    prev->next = ks->next;
    value = ks->key;
    free(ks);
    kl->size--;

    return value;
}

short *generateSineWave(double freq /*120.0*/, double freqSampling /* 44100 */, KeyStates *n, float slope, float old_pitchbend)
{
    static short sinwave[GEN_BUFFER_SIZE];

           
     //printf("x_old is %f",x_old);
     
     for (int count = 0; count < GEN_BUFFER_SIZE; count += 2)
    {
    double w = (2.0 * freq * PI * (float)old_pitchbend) / freqSampling;
     sinwave[count] = (short)(sinf(n->wsum) * (32767/8));
     sinwave[count + 1] = (short)(sinf(n->wsum) * (32767/8));
     n->wsum += w;
     old_pitchbend = old_pitchbend + slope ;
    }
    
    
    //printf("X-OLD 2  is %f",x_old);
    

    return sinwave;
}

int main(int argc, char *argv[])
{
    short alsabuf[GEN_BUFFER_SIZE];
    
    FILE *fpipe = NULL;
    //Open the file for binary write
    fpipe = fopen(OUTPUT_PIPE, "wb");
    if (fpipe == NULL)
    {
        fprintf(stderr, "Error opening file %s: %s", OUTPUT_PIPE, strerror(errno));
        exit(-1);
    }

    /* open serial port */
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    printf("fd opened as %i\n", fd);
    if (fd > 0)
    {
        printf("Open successfully\n");
    }
    else
    {
        printf("failed to open device\n");
    }

    set_interface_attribs(fd, B19200);
    set_blocking(fd, 0);
    fcntl(fd, F_SETFL, FNDELAY);
    


    createSharedMemory();
    g_shMemPtr->Result = 0;
    g_shMemPtr->Realdata = 0;
    sleep(1);
    printf("Buffer Size is %d", GEN_BUFFER_SIZE);

    long idx = 0;

    KeyList *kl = create_keylist();
    static volatile bool flag = 0;
    zforce_Init();
    do
    {   
        static int count = 0;
        static unsigned char command;
        static unsigned char temp_data[8];
        static unsigned char slider_global; 
        static unsigned char temp_dial;
        static unsigned char key;
        static unsigned char temp_wheel = 42;
        static float_t old_wheel = 42;
        static double sum_frq = 0.0;
        unsigned char buf;
        int rdlen = 0;
        rdlen = read(fd, &buf, sizeof(char));
        if (rdlen > 0)
        {
         command = buf;
         printf("%d\n",command); 

         switch (Midi_State)
        {

            case IDLE:
            if  ((command & 0xf0) == 144){
                sum_frq = sum_freq(command,kl->head,kl->size);
                Midi_State = Look_Key_Start;
                
                }
            else if ((command & 0xf0) == 128) {
                sum_frq = sum_freq(command,kl->head,kl->size);
                Midi_State = Look_Key_End;
              
                
            }
            else if ((command & 0xf0) == 192) {
                Midi_State = Look_Dial_1_A_Start;
                
            }
            else if ( command == 177) {
                Midi_State = Look_Dial_1_B_Start;
               
            }
            else if (command == 176) {
                Midi_State = Look_Dial_2_Start;
                
            }

            else if (command == 178) {
                Midi_State = Look_Dial_4_Start;
                
            }   
            else if (command == 224 || command == 231) {
                Midi_State = Look_PB_Wheel_Start;
                
               
            }
            else if (command == 240){
                Midi_State = Look_Slider_Start;
               
            }    
            count = 0;         
            break;

        case Look_Key_Start:
        //printf("Look_Key_Start\n"); 
        temp_data[count++] = command;
        if(count == 2)
        {
         //printf("temp_data[0] :%d temp_data[1] : %d\n",temp_data[0],temp_data[1]);
         push_keylist(kl, ATTACK,temp_data[0],temp_data[1]);
         Midi_State = IDLE;
        }
        
        if(kl->size > 8)
        {
          Midi_State = IDLE;  
        }
            
        break;
    
        
        case Look_Key_End:
        temp_data[count++] = command;
        if(count == 2)
        {
        printf("Pop : %d->\n",pop_keylist(kl,temp_data[0])); 
        Midi_State = IDLE;
        }
        
        break;
        
        
        case Look_Dial_1_A_Start:
        temp_data[count++] = command;
        if(count == 2)
        {
        temp_dial = temp_data[1];
        Midi_State = IDLE;
        }

        break;

        case Look_Dial_1_B_Start:
        
        key = command;  
        Midi_State = IDLE;
        

        break;
        
        case Look_Dial_2_Start:
         
        temp_data[count++] = command;
        if(count == 2)
        {
        temp_dial = temp_data[1];
        Midi_State = IDLE;
        }
        break;
        
        case Look_Dial_4_Start:
        
        temp_data[count++] = command;
        if(count == 2)
        {
        temp_dial = temp_data[1];
        Midi_State = IDLE;
        }
        break;

        case Look_Slider_Start:
        temp_data[count++] = command;
        if(count == 7)
        {
        slider_global = temp_data[6];
        Midi_State = IDLE;
        }
        break;
     
        case Look_Mod_Wheel_Start:
        
        temp_data[count++] = command;
        if(count == 2)
        {
        temp_wheel = temp_data[1];
        Midi_State = IDLE;
        }
        break;

        case Look_PB_Wheel_Start:
        temp_data[count++] = command;
        
        if(count == 2)
        {
  
        temp_wheel = temp_data[1];
        
        Midi_State = IDLE;
        }
        
        break;
        }   
        
    }

        if (g_shMemPtr->Result == 1)
        {
            memset(alsabuf,0,GEN_BUFFER_SIZE * sizeof(short));
            if (kl->size != 0)
            {
                KeyStates *n = kl->head;
                double freq = 0.0;
                //float buf_slope = ((temp_wheel - old_wheel)/ 10);  

                float x_old = generatepitchbend ((unsigned char)old_wheel);
                old_wheel = old_wheel*BETA + (float)temp_wheel*(1-BETA);
                float x = generatepitchbend ((unsigned char)old_wheel);
                float slope = ((x - x_old)/ NUM_OF_SAMPLES);  
                printf("%f ",slope);
                while (n != NULL)
                { 
                    
                    freq = n->freq;
                    //printf("\nKey : %d-> Sine Wave Freq(Hz) : %lf ", n->key ,n->freq);
                    short *readSineVal = generateSineWave(freq, 44100, n,slope,x_old);
                    for(int i = 0; i < GEN_BUFFER_SIZE ; i++){
                        alsabuf[i] += *readSineVal++; 
                    }
                    n = n->next;
                }
 
                memcpy(&g_shMemPtr->samples, alsabuf, GEN_BUFFER_SIZE * sizeof(short));
                //printf("idx is %ld \n",idx);
                g_shMemPtr->blknum++;
                g_shMemPtr->Realdata = 1;
                g_shMemPtr->Result = 0;
            }

            else
            {
                old_wheel = old_wheel*BETA + (float)temp_wheel*(1-BETA);
                //temp_wheel = 42;
                g_shMemPtr->blknum = 0;
                g_shMemPtr->Realdata = 0;
                g_shMemPtr->Result = 0;
 //               idx = 0;
            }
        }

    } while (1);

    return 0;

    fclose(fpipe);
}
