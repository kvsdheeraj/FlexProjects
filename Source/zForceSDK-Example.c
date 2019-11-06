/*! \file
 * \copyright
 * COPYRIGHT NOTICE: (c) 2017-2018 Neonode Technologies AB. All rights reserved.
 *
 */

// Header Files
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <zForceCommon.h>
#include <OsAbstractionLayer.h>
#include <zForce.h>
#include <Queue.h>
#include <Connection.h>
#include <Device.h>
#include <Message.h>
#include "ErrorString.h"
#include "DumpMessage.h"
#include "gen2test.h"

#define HIDDEVICEVID "0x1536"     // Vendor ID of Device.
#define HIDDEVICEPID "0x0101"     // Product ID of Device.
#define TESTRESOLUTIONX1 640
#define TESTRESOLUTIONY1 480
#define TESTRESOLUTIONZ1 100

void   DumpMessage (Message * message);
void   DumpEnableMessage (Message * message);
void   DumpDisableMessage (Message * message);
void   DumpOperationModesMessage (Message * message);
void   DumpResolutionMessage (Message * message);
void   DumpTouchActiveArea (Message * message);
void   DumpReverseTouchActiveArea (Message * message);
void   DumpTouchMessage (Message * message);
void   DumpNumberOfTrackedObjectsMessage (Message * message);
void   DumpFingerFrequencyMessage (Message * message);
void   DumpIdleFrequencyMessage (Message * message);
void   DumpDetectedObjectSizeRestrictionMessage (Message * message);

void   DumpMessageError (Message * message);
char * ErrorString (int errorCode);

void   DoDisable (SensorDevice * sensorDevice);
void   DoEnable (SensorDevice * sensorDevice);
void   DoOperationModes (SensorDevice * sensorDevice);
void   DoTouchActiveArea (SensorDevice * sensorDevice);
void   DoResolution (SensorDevice * sensorDevice);
void   DoNumberOfTrackedObjects (SensorDevice * sensorDevice);
void   DoFingerFrequency (SensorDevice * sensorDevice);
void   DoIdleFrequency (SensorDevice * sensorDevice);
void   DoDetectedObjectSizeRestrictionMessage (SensorDevice * sensorDevice);

/*
 * We will let the user quit the program by pressing
 * Control-C. In such an event SignalHandler will be called.
 */
void   SignalHandler (int sig);

/*
 * The procedure we sue to close everything down correctly.
 */
void Destroy (void);

// Local (static) Variables
static bool         zForceInitialized = false;
static Connection * MyConnection = NULL;
static bool         IsConnected = false;

int zforce_Init (void)
{
    bool resultCode = zForce_Initialize (NULL);

    if (resultCode)
    {
        zForceInitialized = true;
    }
    else
    {
        printf ("zForce initialization failed.\n");
        Destroy ();
        exit (-1);
    }

    // Install the Control-C handler.
    signal (SIGINT, SignalHandler);

    // Here we connect to a device using hidpipe.
    //
    // HidPipeTransport (hidpipe) has the following options:
    //
    // vid: Vendor ID. Current neonode sensors have VID 0x1536.
    // pid: Product ID. Current neonode sensors have PID 0x0101.
    // index: If the computer has multiple connected Neonode sensors,
    //        the first one has index 0, the second has index 1, etc.
    //        Note: The order is decided by the Operating System.
    //
    // Asn1Protocol (asn1) currently has no options.
    //
    // StreamingDataFrame (Streaming) is the DataFrame type used by both
    // HidPipeTransport and Asn1Protocol.
    //
    MyConnection = Connection_New (
            "hidpipe://vid="HIDDEVICEVID",pid="HIDDEVICEPID",index=0", // Transport
            "asn1://",                                                 // Protocol
            "Streaming");                                              // DataFrame type. Both Transport and Protocol must support the same.

    if (NULL == MyConnection)
    {
        printf ("Unable to create connection: (%d) %s.\n",
                zForceErrno,
                ErrorString (zForceErrno));
        Destroy ();
        exit (-1);
    }

    printf ("Connection created.\n");

    printf ("Connecting to Device.\n");

    bool connectionAttemptResult = MyConnection->Connect (MyConnection);

    if (!connectionAttemptResult)
    {
        printf ("Unable to connect to device: (%d) %s\n",
                zForceErrno,
                ErrorString (zForceErrno));
        Destroy ();
        exit (-1);
    }

    // Wait for Connection response to arrive within 1000 seconds.
    ConnectionMessage * connectionMessage =
        MyConnection->ConnectionQueue->Dequeue (MyConnection->ConnectionQueue,
                                                1000000);

    if (NULL == connectionMessage)
    {
        printf ("No Connection Message Received.\n");
        printf ("   Reason: %s\n", ErrorString (zForceErrno));
        Destroy ();
        exit (-1);
    }

    printf ("Devices: %d\n", MyConnection->NumberOfDevices);

    PlatformDevice * platformDevice =
        (PlatformDevice *)MyConnection->FindDevice (MyConnection, Platform, 0);

    if (NULL == platformDevice)
    {
        printf ("No Platform device found.\n");
        Destroy ();
        exit (-1);
    }

    // Find the first Sensor type device (Core/Air/Plus).
    SensorDevice * sensorDevice =
        (SensorDevice *)MyConnection->FindDevice (MyConnection, Sensor, 0);

    if (NULL == sensorDevice)
    {
        printf ("No Sensor device found.\n");
        Destroy ();
        exit (-1);
    }

    char     * deviceTypeString = NULL;
    DeviceType deviceType = sensorDevice->DeviceType & ~Sensor;

    switch (deviceType)
    {
        case Platform:
            deviceTypeString = "Platform";
        break;
        case Core:
            deviceTypeString = "Core";
        break;
        case Air:
            deviceTypeString = "Air";
        break;
        case Plus:
            deviceTypeString = "Plus";
        break;
        case Lighting:
            deviceTypeString = "Lighting";
        break;
        default:
            deviceTypeString = "Unknown";
        break;
    }

    printf ("Found Device: %s.\n", deviceTypeString);

    /*
     * The sequence will now be
     * 1) SetOperationModes.
     * 2) GetResolution.
     * 3) GetMcuUniqueIdentifier.
     * 4) Enable notifications.
     *
     */
    if (!sensorDevice->SetOperationModes (sensorDevice,
                                          DetectionMode|SignalsMode|LedLevelsMode|DetectionHidMode|GesturesMode,
                                          DetectionMode))
    {
        printf ("SetOperationModes error (%d) %s.\n", zForceErrno, ErrorString (zForceErrno));

        Destroy ();
        exit (-1);
    }

    for (;;)
    {
        // Wait for the answer to arrive, timeout after 1 second (1000ms).
#define DEQUEUETIMEOUT 1000
        Message * message = MyConnection->DeviceQueue->Dequeue (MyConnection->DeviceQueue, DEQUEUETIMEOUT);

        if (NULL != message)
        {
            DumpMessage (message);

            switch (message->MessageType)
            {
                case EnableMessageType:
                    /* We are enabled and can now receive notifications */
                break;

                case OperationModesMessageType:
                    if (!sensorDevice->GetResolution (sensorDevice))
                    {
                        printf ("GetResolution error (%d) %s\n", zForceErrno, ErrorString (zForceErrno) );

                        Destroy ();
                        exit (-1);
                    }
                break;

                case ResolutionMessageType:
                    if (!platformDevice->GetMcuUniqueIdentifier (platformDevice))
                    {
                        printf("GetMcuUniqueIdentifier error (%d) %s.\n", zForceErrno, ErrorString(zForceErrno));

                        Destroy();
                        exit(-1);
                    }
                break;

                case McuUniqueIdentifierMessageType:
                    if (!sensorDevice->SetEnable (sensorDevice, true, 0))
                    {
                        printf ("SetEnable error (%d) %s.\n", zForceErrno, ErrorString (zForceErrno));

                        Destroy ();
                        exit (-1);
                    }
                break;
                default:
                    /* Do nothing */
                break;
            }

            message->Destructor (message);
        }

    }

    Destroy ();
}

void Destroy (void)
{
    if (IsConnected)
    {
        MyConnection->Disconnect (MyConnection);
        IsConnected = false;
    }

    if (NULL != MyConnection)
    {
        MyConnection->Destructor (MyConnection);
    }

    if (zForceInitialized)
    {
        zForce_Uninitialize ();
        zForceInitialized = false;
    }

}

void SignalHandler (int sig)
{
    (void) sig;

    Destroy ();
    exit (-1);
}
