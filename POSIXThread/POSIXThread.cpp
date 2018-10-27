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
