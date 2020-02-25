
#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string>
#include <cstring>

class BinarySemaphoreClient
{
public:

    BinarySemaphoreClient(void);
    virtual ~BinarySemaphoreClient(void);

    BinarySemaphoreClient& setFileToKey(
            const std::string& a_fileToKey );
    bool open(void);
    void tryToOpenUntilSuccess(void);
    void close(void);

    int semaphoreID(void) const;

    bool lock(
            unsigned short int a_semaphoreNumber = 0);
    void unlock(
            unsigned short int a_semaphoreNumber = 0);
    bool setVal(
            unsigned short int a_semaphoreNumber,
            unsigned short int a_val );
private:
    int m_semaphoreID;
    key_t m_semaphoreKey;
    unsigned short int m_semaphoreNumber;
    std::string m_fileToKey;
};
