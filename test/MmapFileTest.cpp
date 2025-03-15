#include "utils/MmapFile.h"
#include <fstream>
#include <memory>
#include <gtest/gtest.h>
using namespace std;



TEST(MmapFileTest, MmapFile) {
	std::string path = "test.txt";
	std::string content = "hello world";

	unique_ptr<MmapFile> mmapFile = make_unique<MmapFile>(path, 0, 1024*1024);
	mmapFile->writeContent(content, true);

	std::string readContent = mmapFile->readContent(0, content.size());
	cout << "readContent: " << readContent << endl;
	EXPECT_EQ(content, readContent);
}