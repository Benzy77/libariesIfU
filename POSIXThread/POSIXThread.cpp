/*******************************************************************************
*  Copyleft (C), 2018,
*  FileName:      posixThread.cpp
*  Author:        Ben.D
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*        Ben.D      2018-10-27        1.0           create
*
*  Description:  map to manage thread.
*  Main Thread .
*******************************************************************************/

#include"../common/POSIXThread.h"

/*******************************************************************************
  Function:    posixThreadtask()
  Description: constructor
  Input:    const Int8* name
            Int32 priority
            Int32 stackSize
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
posixThreadtask::posixThreadtask(const char* name, Int32 priority, Int32 stackSize)
:_bActive(false)
{
	_sthreadInfo.name = name;
	_sthreadInfo.priority = priority;
//	PTHREAD_STACK_MIN > stackSize ? (_sTaskInfo.stackSize = PTHREAD_STACK_MIN) : (_sTaskInfo.stackSize = stackSize);
	_sthreadInfo.stackSize = (PTHREAD_STACK_MIN > stackSize ? PTHREAD_STACK_MIN : stackSize);
}

/*******************************************************************************
  Function:    ~posixThreadtask()
  Description: cancel() must be call before ~posixThreadtask
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
posixThreadtask::~posixThreadtask()
:_bActive(false)
{

}

/*******************************************************************************
  Function:    activateThreadtask()
  Description: set attr and create thread.
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32  posixThreadtask::activateThreadtask()
{
	int errNo = 0;
	pthread_attr_t attr;
	if(_sthreadInfo.priority > MAX_PRI_VALUE || _sthreadInfo.priority < MIN_PRI_VALUE)
	{
		printf("Error,activate()failed, task priority invalid[%d]\n", _sthreadInfo.priority);
	    return APP_STATUS_ERROR;
	}

	if((errNo = pthread_attr_init(&attr)) != 0)
	{
		printf("Error, pthread_attr_init() failed, errno = %d", errNo);
		return APP_STATUS_ERROR;
	}

	if((errNo = pthread_attr_setstacksize(&attr, _sthreadInfo.stackSize)) != 0)
	{
		printf("Error, pthread_attr_setstacksize() failed, stackSize = %d, errno = %d",
				_sthreadInfo.stackSize, errNo);

		pthread_attr_destroy(&attr);
		return APP_STATUS_ERROR;
	}

    if((errNo = pthread_create(&_sthreadInfo.id, &attr, (void* (*)(void*))entryPoint, this)) != 0)
    {
    	printf("pthread_create() failed! errNo = %d\n", errNo);

    	pthread_attr_destroy(&attr);
    	return APP_STATUS_ERROR;
    }
    pthread_attr_destroy(&attr);
    _bActive = true;            //Thread has created successfully.
    _sthreadInfo.pid = getNLPid();

	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:    setPriority()
  Description: set attr and create thread.
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 posixThreadtask::setPriority(Int32 priority)
{

	if(MAX_PRI_VALUE < priority < MIN_PRI_VALUE)
	{
		_sthreadInfo.priority =  pritority;
		return APP_STATUS_SUCCESS;
	}

		return APP_STATUS_ERROR;
}

/*******************************************************************************
  Function:    getPriority()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 posixThreadtask::getPriority(Int32& pPriority)
{
	pPriority = _sTaskInfo.priority;
	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:    ThreadID()
  Description: ThreadID
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
TaskId posixThreadtask::getThreadID()
{
	return _sthreadInfo.id;
}

/*******************************************************************************
  Function:    ThreadName()
  Description: ThreadName
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
const char* posixThreadtask::getThreadName()
{
	return _sthreadInfo.name.c_str();
}


/*******************************************************************************
  Function:    getThreadInfo()
  Description:  Thread information
  Input:    none
  Output:   S_threadTaskInfo& taskInfo
  Return:   none
  Others:   none
*******************************************************************************/
Int32 posixThreadtask::getThreadInfo(S_threadTaskInfo& taskInfo)
{
	taskInfo = _sthreadInfo;
    return APP_STATUS_SUCCESS;
}

Int32 cancel()
{
	if(!_bActive)
	{
//		TaskManager::getInstance()->removeTaskInfo(_sTaskInfo.id);
		return APP_STATUS_ERROR;
	}

	pthread_cancel(_sthreadInfo.id);
	Int32 ret = pthread_join(_sthreadInfo.id, NULL);

	if (0 == ret)
	{
		_bActive = false;
//		TaskManager::getInstance()->removeTaskInfo(_sTaskInfo.id);
	}

	return ret;
}


