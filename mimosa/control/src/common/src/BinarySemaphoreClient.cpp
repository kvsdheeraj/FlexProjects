#include "BinarySemaphoreClient.h"
#define RETURN_IF(CONDITION,RETURN_VALUE)			if(CONDITION) return RETURN_VALUE;

/*BinarySemaphoreClient::BinarySemaphoreClient(void)
{
    m_semaphoreID		= -1;
    m_semaphoreKey		= 0;
    m_semaphoreNumber	= 0;
}

BinarySemaphoreClient::~BinarySemaphoreClient(void)
{
}


BinarySemaphoreClient& BinarySemaphoreClient::setFileToKey(
        const std::string& a_fileToKey )
{
    m_fileToKey = a_fileToKey;
    return *this;
}*/

bool BinarySemaphoreClient::open(void)
{
    RETURN_IF( m_semaphoreID != -1, true );
    //m_semaphoreKey = ftok( m_fileToKey.c_str(), 1 );
    //m_semaphoreID = semget( m_semaphoreKey, 0, 0 );
    RETURN_IF( m_semaphoreID == -1, false );

    return true;
}

void BinarySemaphoreClient::tryToOpenUntilSuccess(void)
{
    if( m_semaphoreID != -1 )
    {
        return;
    }
    
    int count = 0;
    for(;;)
    {
       // m_semaphoreKey = ftok( m_fileToKey.c_str(), 1 );
       // m_semaphoreID = semget( m_semaphoreKey, 0, 0 );
        count++;
        if( m_semaphoreID != -1 ) break;
        usleep(5000);
    }
    
    fprintf(
        stderr,
        "Semaphore opening tries count = %d\n",
        count );
}

bool BinarySemaphoreClient::setVal(
        unsigned short int a_semaphoreNumber,
        unsigned short int a_val )
{
    RETURN_IF( m_semaphoreID == -1, false );
    /*int l_semctlRet = semctl(
            m_semaphoreID,
            a_semaphoreNumber,
            SETVAL,
            a_val );
            
    if(l_semctlRet == -1)
    {
        printf( "BinarySemaphoreClient::create(semctl) - %s\n", strerror(errno) );
        return false;
    }*/
    
    return true;
}

bool BinarySemaphoreClient::lock(
        unsigned short int a_semaphoreNumber)
{
    RETURN_IF( m_semaphoreID == -1, false );
    struct sembuf l_sembuf;
    memset( &l_sembuf, 0, sizeof l_sembuf );

    l_sembuf.sem_num = a_semaphoreNumber;
    l_sembuf.sem_op = -1;
    l_sembuf.sem_flg = 0;//SEM_UNDO;

    /*int l_semopRet = 
        semop( m_semaphoreID, &l_sembuf, 1 );
    while( l_semopRet == -1 && errno == EINTR )
    {
        l_semopRet = 
            semop( m_semaphoreID, &l_sembuf, 1 );
    }

    bool l_ret = l_semopRet == 0;*/
    bool l_ret = 0;

    return l_ret;
}
void BinarySemaphoreClient::unlock(
        unsigned short int a_semaphoreNumber)
{
    struct sembuf l_sembuf;
    memset( &l_sembuf, 0, sizeof l_sembuf );

    l_sembuf.sem_num = a_semaphoreNumber;
    l_sembuf.sem_op = 1;
    l_sembuf.sem_flg = 0;//SEM_UNDO;

   /* int l_semopRet = 
        semop( m_semaphoreID, &l_sembuf, 1 );
    while( l_semopRet == -1 && errno == EINTR )
    {
        l_semopRet = 
            semop( m_semaphoreID, &l_sembuf, 1 );
    }

    if( l_semopRet != 0 )
    {
        printf( 
            "BinarySemaphoreClient::unlock::semop - %s\n", 
            strerror(errno) );
    }*/
}

void BinarySemaphoreClient::close(void)
{
}

int BinarySemaphoreClient::semaphoreID(void) const
{
    return m_semaphoreID;
}
