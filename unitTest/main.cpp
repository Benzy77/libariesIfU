/*******************************************************************************
*  Copyleft (C), 2018,
*  FileName:      memory_allocator.cpp
*  Author:        Ben.D
*  Version :      1.0
*  Date:          2018-10-15
*  History:
*      <author>       <time>        <version >      <desc>
*        Ben.D      2018-10-15        1.0           create
*
*  Description:
*  gtest.
*
*******************************************************************************/
#include <gtest/gtest.h>

#ifdef __CYGWIN__

extern "C" int main(int argc, char *argv[])
{
//    testing::FLAGS_gtest_filter = "testCInterface.testMem";
	testing::FLAGS_gtest_filter = "*.*";
	testing::GTEST_FLAG(output) = "xml:UnitTestingReport.xml";
    testing::FLAGS_gtest_repeat = 1;
    testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

#endif
