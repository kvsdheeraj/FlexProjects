#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string>
#include <cstring>

class BinarySemaphoreServer
{
public:
    BinarySemaphoreServer(void);
    virtual ~BinarySemaphoreServer(void);

    BinarySemaphoreServer& setFileToKey(
            const std::string& a_fileToKey );
    bool create(
            unsigned short int a_numOfSemaphores );
    void destroy(void);

    int semaphoreID(void) const;
    unsigned short int numOfSemaphores(void) const;

    bool lockAll(void);
    bool unlockAll(void);

private:
    int m_semaphoreID;
    key_t m_semaphoreKey;
    unsigned short int m_numOfSemaphores;
    std::string m_fileToKey;
    unsigned short int* m_semvalArray;
};
