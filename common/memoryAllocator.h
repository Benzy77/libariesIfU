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

namespace Csp
{

class memoryAlloc
{
public:
	memoryAlloc();
    void *malloc(size_t size);

private:
    size_t size;
};

};

/* end of namespace Csp */
#endif/* MEMORY_ALLOCATOR_H */
/* end of file*/
