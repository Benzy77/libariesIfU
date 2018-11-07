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
#include <stdio.h>
#include <pthread.h>

#include "../commonType.h"
#include "../synchrolock.h"

#define MUTEX_ATTR_INIT(mLock) \
	do \
    { \
		int errNo; \
		pthread_mutexattr_t attr; \
		if((errNo = pthread_mutexattr_init(&attr)) != 0) \
		{ \
			printf("pthread_mutexattr_init() failed! errNo = %s\n", strerror(errNo)); \
			break; \
		} \
		/* enable recursive lock */ \
		if((errNo = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0) \
		{ \
			printf("pthread_mutexattr_settype() failed! errNo= %s\n", strerror(errNo)); \
			pthread_mutexattr_destroy(&attr); \
			break; \
		} \
		if((errNo = pthread_mutex_init(&(mLock), &attr)) != 0) \
		{ \
			printf("pthread_mutex_init() failed! errNo = %s\n", strerror(errNo)); \
			pthread_mutexattr_destroy(&attr); \
			break; \
		} \
		pthread_mutexattr_destroy(&attr); \
   }while(0)

#define SEM_ATTR_INIT(semId) \
	do \
	{ \
	    int errNo; \
		if((errNo = pthread_mutex_init(&semId.semLock.mLock, NULL)) != 0) \
		{ \
			printf("pthread_mutex_init()failed! errNo = %s\n", strerror(errNo)); \
			break; \
		} \
		if((errNo = pthread_cond_init(&semId.semLock.condVal, NULL)) != 0) \
		{ \
			printf("pthread_cond_init()failed! errNo = %s\n", strerror(errNo)); \
			break; \
		} \
		semId.semLock.binVal = false; \
	}while(0)



namespace Csp
{
/*******************************************************************************
  Function:  Mutex::Mutex()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Mutex::Mutex()
{
	  _sMutexInfo.creator = pthread_self();
	  _sMutexInfo.name = "null";
	  MUTEX_ATTR_INIT(_sMutexInfo.id.mLock);
}

/*******************************************************************************
  Function:  Mutex::Mutex(const char* name)
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Mutex::Mutex(const char* name)
{
	_sMutexInfo.creator = pthread_self();
	_sMutexInfo.name = name;
	MUTEX_ATTR_INIT(_sMutexInfo.id.mLock);
}

/*******************************************************************************
  Function:  Mutex::~Mutex()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Mutex::~Mutex()
{
	int errNo;
	if((errNo = pthread_mutex_destroy(&_sMutexInfo.id.mLock)) != 0)
	{
		printf("pthread_mutex_destroy() failed! errNo = %s\n", strerror(errNo));
	}
}

/*******************************************************************************
  Function:  Int32 Mutex::activeTakeLock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 Mutex::activeTakeLock(Int32 timeout)
{
    int error = APP_STATUS_ERROR;
    switch(timeout)
    {
    //Non-blocking for lock.
    case NO_WAIT:
    	if((errNo = pthread_mutex_trylock(&_sMutexInfo.id.mLock)) != 0)
    	{
    		if(errNo != EBUSY)
    		{
    		  printf("pthread_mutex_trylock() failed, errNo = %s\n", strerror(errNo));
    		}
    		 errNo = APP_STATUS_ERROR;
    	}
    	break;
    case WAIT_FOREVER:
    	if((errNo = pthread_mutex_lock(&_sMutexInfo.id.mLock)) != 0)
    	{
    		if(errNo != EBUSY)
    		{
    		  printf("pthread_mutex_trylock() failed, errNo = %s\n", strerror(errNo));
    		}
    		 errNo = APP_STATUS_ERROR;
    	}
    	break;
    }
}

/*******************************************************************************
  Function:  Int32 Mutex::activeTakeLock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 Mutex::activeUnlock(Int32 timeout)
{
	int errNo;
	if((errNo = pthread_mutex_unlock(&_sMutexInfo.id.mLock)) != 0)
	{
		printf("pthread_mutex_unlock() failed! errNo = %s\n", strerror(errNo));
		return APP_STATUS_ERROR;
	}
	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:  Mutex::getSemId()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
SemId Mutex::getSemId()
{
	return _sMutexInfo.id;
}

/*******************************************************************************
  Function:  Mutex::getSemInfo
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 Mutex::getSemInfo(S_SemInfo& semInfo)
{
	semInfo = _sMutexInfo;
	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:  MutexLocker::MutexLocker
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
MutexLocker::MutexLocker(Mutex* pMutex)
:_pMutex(pMutex)
{
	if(_pMutex)
	{
		_pMutex->activeTakeLock();
	}
}

/*******************************************************************************
  Function:  MutexLocker::~MutexLocker
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
MutexLocker::~MutexLocker()
{
	if(_pMutex)
	{
		_pMutex->activeUnlock();
	}
}

/*******************************************************************************
  Function:  ReadWriteLock::ReadWriteLock(const char* name)
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
ReadWriteLock::ReadWriteLock(const char* name)
{
	  int errNo;
	  _sRWLockInfo.creator = pthread_self();
	  _sRWLockInfo.name = name;
	  if((errNo = pthread_rwlock_init(&_sRWLockInfo.id.rwLock, NULL)) != 0)
	  {
		  printf("Error, pthread_rwlock_init()failed! errNo = %s\n", strerror(errNo));
	  }
}

/*******************************************************************************
  Function:  ReadWriteLock::~ReadWriteLock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
ReadWriteLock::~ReadWriteLock()
{
	  int errNo;
	  if((errNo = pthread_rwlock_destroy(&_sRWLockInfo.id.rwLock)) != 0)
	  {
		  printf("Error, pthread_rwlock_destroy()failed! errNo = %s\n", strerror(errNo));
	  }
}

/*******************************************************************************
  Function:  ReadWriteLock::takeRLock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
ReadWriteLock::takeRLock(Int32 timeout)
{
	int errNo = APP_STATUS_ERROR;

	switch(timeout)
	{
	case NO_WAIT:
		if((errNo = pthread_rwlock_tryrdlock(&_sRWLockInfo.id.rwLock)) != 0)
		{
			if(errNo != EBUSY)
			{
				printf("Error, pthread_rwlock_tryrdlock()failed! errNo = %s\n", strerror(errNo));
			}
			errNo = APP_STATUS_ERROR;
		}
		break;
	case WAIT_FOREVER:
		if((errNo = pthread_rwlock_rdlock(&_sRWLockInfo.id.rwLock)) != 0)
		{
			printf("Error, pthread_rwlock_rdlock()failed! errNo = %s\n", strerror(errNo));
		}
		break;
	default:
		printf("ReadWriteLock::takeRLock(timeout)failed, timeout should be NO_WAIT or WAIT_FOREVER!\n");
		break;
	}
	return errNo;

}

/*******************************************************************************
  Function:  ReadWriteLock::takeWLock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
ReadWriteLock::takeWLock(Int32 timeout)
{
	int errNo = APP_STATUS_ERROR;

	switch(timeout)
	{
	case NO_WAIT:
		if((errNo = pthread_rwlock_trywrlock(&_sRWLockInfo.id.rwLock)) != 0)
		{
			if(errNo != EBUSY)
			{
				printf("Error, pthread_rwlock_tryrdlock()failed! errNo = %s\n", strerror(errNo));
			}
			errNo = APP_STATUS_ERROR;
		}
		break;
	case WAIT_FOREVER:
		if((errNo = pthread_rwlock_wrlock(&_sRWLockInfo.id.rwLock)) != 0)
		{
			printf("Error, pthread_rwlock_rdlock()failed! errNo = %s\n", strerror(errNo));
		}
		break;
	default:
		printf("ReadWriteLock::takeRLock(timeout)failed, timeout should be NO_WAIT or WAIT_FOREVER!\n");
		break;
	}
	return errNo;
}

/*******************************************************************************
  Function:  ReadWriteLock::ReadWriteUnlock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 ReadWriteLock::ReadWriteUnlock()
{
	int errNo;
	if((errNo = pthread_rwlock_unlock(&_sRWLockInfo.id.rwLock)) != 0)
	{
		BL_LOG_ERROR("Error, pthread_rwlock_unlock()failed! errNo = %s\n", strerror(errNo));
		return APP_STATUS_ERROR;
	}
	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:  ReadWriteLock::ReadWriteUnlock()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
SemId ReadWriteLock::getId()
{
	return _sRWLockInfo.id;
}

/*******************************************************************************
  Function:  ReadWriteLock::getSemInfo()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 ReadWriteLock::getSemInfo(S_SemInfo& sRWLockInfo)
{
	sRWLockInfo = _sRWLockInfo;
	return APP_STATUS_SUCCESS;
}

}
