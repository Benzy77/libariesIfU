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
#ifndef POSIX_THREAD_H_
#define POSIX_THREAD_H_

#include <map>
#include <string>
#include <pthread.h>

namespace Csp
{
typedef pthread_t threadTaskId;
typedef pid_t     NLPthreadid;

typedef struct
{
	std::string   threadname;
	NLPthreadid   pid;
	threadTaskId  id;
	threadTaskId  creator;
    Int32         priority;
    Int32         stackSize;
}S_threadTaskInfo;

class posixThreadtask
{
public:
	    posixThreadtask(const char* name, Int32 priority, Int32 stackSize);
		virtual ~posixThreadtask();
	    Int32   setPriority(Int32 priority);
	    Int32   getPriority(Int32& pPriority);
	    threadTaskId getThreadID();

	    const char* getThreadName();
	    Int32 getThreadInfo(S_TaskInfo& taskInfo);
	    Int32 cancel();
	    Int32 setSched();
	    Int32 waitThreadTaskForExit();

protected:
        virtual Int32 run() = 0;
        static  Int32  entryPoint(Task* pTaskBase);
        Int32  getNLPid();

private:
        bool _bActive;
        S_threadTaskInfo _sthreadInfo;
}

class threadTaskManager
{
public:
	static threadTaskManager* getInstance();
	~threadTaskManager();
	Int32 dump();
	Int32 addTaskInfo(S_threadTaskInfo* pTaskInfo);
	Int32 removeTaskInfo(threadTaskId id);
	Int32 getTaskInfo(threadTaskId id, S_threadTaskInfo& taskInfo);
private:
	TaskManager();
private:
//	ReadWriteLock _rwLock;
	static threadTaskManager* _pInstance;
	std::map<threadTaskId, S_threadTaskInfo*> _threadTaskManagerInfoMap;
};

}

#endif	/* POSIX_THREAD_H_*/
/* end of file */
