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


#define WAIT_FOREVER -1

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
    virtual Int32  take(Int32 timeout) = 0;  //pure function as an interface.
    virtual Int32  give() = 0;
    virtual SemId  getId() = 0;
    virtual Int32  getSemInfo(S_SemInfo& semInfo) = 0;
};

class Mutex : public SynchrolockIfU
{
public:
	Mutex();
	Mutex(const char* name);
    virtual ~Mutex();
    virtual Int32  take(Int32 timeout = WAIT_FOREVER);
    virtual Int32  give();
    virtual SemId  getId();
    virtual Int32  getSemInfo(S_SemInfo& semInfo);
private:
	S_SemInfo _sMutexInfo;
};


class ScopedLocker
{
public:
	ScopedLocker(Mutex* pMutex);
    ~ScopedLocker();
private:
    Mutex* _pMutex;
};

}

#endif	/* SYNCHROLOCK_H */
/* end of file */
