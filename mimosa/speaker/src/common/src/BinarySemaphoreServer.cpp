
#include "BinarySemaphoreServer.h"

/*BinarySemaphoreServer::BinarySemaphoreServer(void)
{
    m_semaphoreID		= -1;
    m_semaphoreKey		= 0;
    m_numOfSemaphores	= 0;
    m_semvalArray		= NULL;
}*/

/*BinarySemaphoreServer::~BinarySemaphoreServer(void)
{
}*/

/*BinarySemaphoreServer& BinarySemaphoreServer::setFileToKey(
        const std::string& a_fileToKey )
{
    m_fileToKey = a_fileToKey;
    return *this;
}*/

bool BinarySemaphoreServer::create(
        unsigned short int a_numOfSemaphores )
{
    if( m_semaphoreID != -1 )
    {
        printf("Semaphore already created\n");
        return true;
    }
    
    m_semaphoreID		= -1;
    m_semaphoreKey		= 0;
    m_numOfSemaphores	= 0;
    m_semvalArray		= 0;

    m_numOfSemaphores	= a_numOfSemaphores;
    //m_semaphoreKey		= ftok( m_fileToKey.c_str(), 1 );
    
    if( m_semaphoreKey == -1 )
    {
        printf( "BinarySemaphoreServer::create(ftok) - %s\n", strerror(errno) );
    }

   /* m_semaphoreID = semget(
            m_semaphoreKey,
            m_numOfSemaphores,
            IPC_CREAT | IPC_EXCL| 0600 );
    if(errno == EEXIST)
    {
        m_semaphoreID = semget(
                m_semaphoreKey,
                0,
                0 );
    }
    
    if( m_semaphoreKey == -1 )
    {
        printf( "BinarySemaphoreServer::create(semget) - %s\n", strerror(errno) );
    }*/
    
    //INICIA SEMAFOROS
    m_semvalArray =
            new unsigned short[m_numOfSemaphores];
    memset(
            m_semvalArray,
            0,
            ( sizeof (unsigned short) ) * m_numOfSemaphores );

    for(
            unsigned short int semnum = 0;
            semnum < m_numOfSemaphores;
            ++semnum )
    {
        m_semvalArray[semnum] = 1;
    }

    /*int l_semctlRet = semctl(
            m_semaphoreID,
            0,
            SETALL,
            m_semvalArray );

    if( l_semctlRet == -1 ){
        const char* sl_strError = strerror(errno);
        semctl( m_semaphoreID, 0, IPC_RMID, m_semvalArray );
        printf( "BinarySemaphoreServer::create(semctl) - %s\n", sl_strError );
    }*/
    
    return true;
}
bool BinarySemaphoreServer::lockAll(void)
{
    if( m_semvalArray == NULL || m_semaphoreID == -1 )
    {
        printf("lockAll error, semaphores not created\n" );
        return false;
    }
    
    memset(
            m_semvalArray,
            0,
            ( sizeof (unsigned short) ) * m_numOfSemaphores );

    for( unsigned short int semnum = 0; semnum < m_numOfSemaphores; ++semnum ){
        m_semvalArray[semnum] = 0;
    }

    /*int l_semctlRet = semctl( m_semaphoreID, 0, SETALL, m_semvalArray );
    
    if( l_semctlRet == -1 )
    {
        printf( "BinarySemaphoreServer::lockAll(semctl) - %s\n",
            strerror(errno) );
        return false;
    }*/
    
    return true;
}
bool BinarySemaphoreServer::unlockAll(void)
{
    
    if( m_semvalArray == NULL || m_semaphoreID == -1 )
    {
        printf("unlockAll error, semaphores not created\n" );
        return false;
    }

    memset( m_semvalArray, 0, ( sizeof (unsigned short) ) * m_numOfSemaphores );
    for( unsigned short int semnum = 0; semnum < m_numOfSemaphores; ++semnum ){
        m_semvalArray[semnum] = 1;
    }
    /*int l_semctlRet = semctl( m_semaphoreID, 0, SETALL, m_semvalArray );
    if( l_semctlRet == -1 )
    {
        printf( "BinarySemaphoreServer::unlockAll(semctl) - %s\n",
            strerror(errno) );
        return false;
    }*/
    
    return true;
}
void BinarySemaphoreServer::destroy(void)
{
    if( m_semaphoreID == -1 ){
        return;
    } else if( m_semaphoreID == 0 ){
       // m_semaphoreKey	= ftok( m_fileToKey.c_str(), 1 );
       // m_semaphoreID	= semget( m_semaphoreKey, 0, 0 );
        return;
    }
    //semctl( m_semaphoreID, 0, IPC_RMID, m_semvalArray );

    delete m_semvalArray;

    m_semaphoreID		= 0;
    m_semaphoreKey		= 0;
    m_numOfSemaphores	= 0;
    m_semvalArray		= NULL;
}
unsigned short int BinarySemaphoreServer::numOfSemaphores(void) const
{
    return m_numOfSemaphores;
}
int BinarySemaphoreServer::semaphoreID(void) const
{
    return m_semaphoreID;
}
