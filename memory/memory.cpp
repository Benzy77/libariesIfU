/*******************************************************************************
*  Copyleft (C), 2018,
*  FileName:      memory.cpp
*  Author:        benqiang.yu
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*    benqiang.yu      2018-10-15        1.0           create
*
*  Description:
*  Main memory.
*******************************************************************************/

#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>


#include "../common/memoryAllocator.h"

namespace Csp
{
using namespace std;

/*******************************************************************************
  Function:  memoryAlloc::memoryAlloc
  Description: create a new simple malloc
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
memoryAlloc::memoryAlloc()
{
	size = DEFAULT_ALLOC_SIZE;
}

/*******************************************************************************
  Function:  memoryAlloc::malloc
  Description: create a new simple malloc
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
void *memoryAlloc::malloc(int size)
{
	void *block;
	block = sbrk(size);
	if (block == (void*) -1)
		return NULL;
	return block;
}

}

