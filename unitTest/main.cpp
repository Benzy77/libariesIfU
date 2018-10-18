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
