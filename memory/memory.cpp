/*******************************************************************************
*  Copyleft (C), 2018
*  FileName:      memory.cpp
*  Author:        Ben.D
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*      Ben.D     2018-10-15        1.0           create
*
*  Description:
*  memory.
*******************************************************************************/
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
template <typename newType>
memoryAlloc<newType>::memoryAlloc()
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
template <typename newType>
void* memoryAlloc<newType>::malloc(newType size)
{
	void *block;
	block = sbrk(size);
	if (block == (void*) -1)
		return NULL;
	return block;
}

/*******************************************************************************
  Function:  memoryAlloc::getType
  Description: get Type
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
template <typename newType>
newType memoryAlloc<newType>::getType()
{
	return type;
}

/*******************************************************************************
  Function:  memoryAlloc::getType
  Description: get Type
  Input:    none
  Output:   none
  Return:   none
  Others:   none
*******************************************************************************/
template <typename newType>
void memoryAlloc<newType>::setType(newType newValueType)
{
	type = newValueType;
}

}

