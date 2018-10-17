/*******************************************************************************
*  Copyleft (C), 2018,
*  FileName:      memory_allocator.cpp
*  Author:        benqiang.yu
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*    benqiang.yu      2018-10-15        1.0           create
*
*  Description:
*  Main memory.
*
*******************************************************************************/
#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <sys/resource.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define DEFAULT_ALLOC_SIZE 16;

namespace Csp
{

class memoryAlloc
{
public:
	memoryAlloc();
    void *malloc(int size);

private:
    int size;
};

};

/* end of namespace Csp */
#endif/* MEMORY_ALLOCATOR_H */
/* end of file*/
