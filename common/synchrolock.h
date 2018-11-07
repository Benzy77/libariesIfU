/*******************************************************************************
*  Copyleft (C), 2018
*  FileName:      synchrolock.h.
*  Author:        Ben.D
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*       Ben.D      2018-10-15        1.0           create
*
*  Description:
*  This is synchrolock for base library. with poxis.
*  why do encapsulation base libary
*  because  some base API will change to. you will need to change many point in your repo
*  synchroloc.h .
*******************************************************************************/
#ifndef SYNCHROLOCK_H
#define SYNCHROLOCK_H

#include <map>
#include <string>
#include <pthread.h>
#include <sys/fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include "commonType.h"


#ifndef WAIT_FOREVER
#define WAIT_FOREVER -1
#endif

#ifndef NO_WAIT
#define NO_WAIT 0
#endif

typedef pthread_t ThreadId;

typedef struct
{
	pthread_mutex_t mLock;
	pthread_cond_t condVal;
	bool binVal;
}threadSem_t;

typedef union
{
	pthread_mutex_t mLock;
	pthread_rwlock_t rwLock;
	threadSem_t semLock;
	sem_t* semIpc;
}SemId;

typedef struct
{
    std::string name;
    SemId       id;
    pthread_t   creator; //this is change could be a thread for project.
}S_SemInfo;

namespace Csp
{

class SynchrolockIfU
{
public:
	SynchrolockIfU(){}
    virtual ~SynchrolockIfU(){}
    virtual Int32  activeTakeLock(Int32 timeout) = 0;  //pure function as an interface.
    virtual Int32  activeUnlock() = 0;
    virtual SemId  getSemId() = 0;
    virtual Int32  getSemInfo(S_SemInfo& semInfo) = 0;
};

class Mutex : public SynchrolockIfU
{
public:
	Mutex();
	Mutex(const char* name);
    virtual ~Mutex();
    virtual Int32  activeTakeLock(Int32 timeout = WAIT_FOREVER);
    virtual Int32  activeUnlock();
    virtual SemId  getSemId();
    virtual Int32  getSemInfo(S_SemInfo& semInfo);
private:
	S_SemInfo _sMutexInfo;
};

class MutexLocker
{
public:
	MutexLocker(Mutex* pMutex);
    ~MutexLocker();
private:
    Mutex* _pMutex;
};

class ReadWriteLock
{
public:
	 ReadWriteLock(const char* name);
     ~ReadWriteLock();
     Int32 takeRLock(Int32 timeout = WAIT_FOREVER);
     Int32 takeWLock(Int32 timeout = WAIT_FOREVER);
     Int32 ReadWriteUnlock();
     SemId getId();
     Int32 getSemInfo(S_SemInfo& sRWLockInfo);
private:
	S_SemInfo _sRWLockInfo;
};

}

#endif	/* SYNCHROLOCK_H */
/* end of file */
