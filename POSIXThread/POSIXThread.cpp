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
    _bActive = true;               //Thread has created successfully.
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

/*******************************************************************************
  Function:    cancel()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 posixThreadtask::cancel()
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

/*******************************************************************************
  Function:    waitThreadTaskForExit()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 posixThreadtask::waitThreadTaskForExit()
{
	if(_bActive)
	{
		_bActive = false;
		return pthread_join(_sTaskInfo.id, NULL);
	}
	return APP_STATUS_ERROR;
}
/****************************posixThreadtask  end*******************************/

/*******************************************************************************
  Function:    threadTaskManager()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
threadTaskManager::threadTaskManager()
:_rwLock("threadTaskManager")
{

}

/*******************************************************************************
  Function:    threadTaskManager()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
threadTaskManager:: ~threadTaskManager()
{
	_pInstance = NULL;
}

/*******************************************************************************
  Function:    showAllTaskInfo()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 threadTaskManager::showAllTaskInfo()
{
	int res = _rwLock.takeRLock(WAIT_FOREVER);
	if(-1 == res)
	{
		printf("showAllTaskInfo::getrdLock error:\n");
		return APP_STATUS_ERROR;
	}

	std::map<TaskId, S_TaskInfo*>::iterator iter = _taskInfoMap.begin();

	for( ; iter != _taskInfoMap.end(); iter++)
	{
		printf("Task %d info:\n", (Uint32)iter->first);
		printf("{\n");
		printf("TaskName %s\n", iter->second->name.c_str());
		printf("creator %d \n", (Uint32)iter->second->creator);
		printf("priority %d \n", (Uint32)iter->second->priority);
		printf("stackSize %d \n", (Uint32)iter->second->stackSize);
		printf("}\n");
	}

    _rwLock.give();

	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:    addThreadTaskInfo()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 threadTaskManager::addThreadTaskInfo(S_threadTaskInfo* pTaskInfo)
{

	CHECK_POINTER_VALID(pTaskInfo);

	int res =  _rwLock.takeWLock(WAIT_FOREVER);
	if(-1 == res)
	{
		printf("addTaskInfo::getwrLock error:\n");
		return APP_STATUS_ERROR;
	}

	std::map<TaskId, S_TaskInfo*>::const_iterator iter= _taskInfoMap.find(pTaskInfo->id);
	if(iter != _taskInfoMap.end())
	{
		printf("task id %d is exist in TaskManager!!!\n",(Uint32)pTaskInfo->id);
		_rwLock.give();
		return APP_STATUS_ERROR;
	}

	if(!_taskInfoMap.insert(std::pair<TaskId, S_TaskInfo*>(pTaskInfo->id, pTaskInfo)).second)
	{
		printf("TaskManager add task id %d info error!!!\n",(Uint32)pTaskInfo->id);
		_rwLock.give();
		return APP_STATUS_ERROR;
	}

	_rwLock.give();
	return APP_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:    removeThreadTaskInfo()
  Description:
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
Int32 threadTaskManager::removeThreadTaskInfo(threadTaskId id)
{
	int res = _rwLock.takeWLock(WAIT_FOREVER);
	if(-1 == res)
	{
		printf("removeTaskInfo::getwrLock error:\n");
		return APP_STATUS_ERROR;
	}
	std::map<TaskId, S_TaskInfo*>::const_iterator iter= _taskInfoMap.find(id);
	if(iter == _taskInfoMap.end())
	{
		printf("task id %d isn't exist in TaskManager!!!\n",(Uint32)id);
		_rwLock.give();
		return APP_STATUS_ERROR;
	}

	_taskInfoMap.erase(iter->first);

	_rwLock.give();

	return APP_STATUS_SUCCESS;
}
