/**
  ******************************************************************************
  * @file           : MmapFile.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/15
  ******************************************************************************
  */

#ifndef DISTRIBUTEDMESSAGEQUEUE_SRC_UTILS_MMAPFILE_H_
#define DISTRIBUTEDMESSAGEQUEUE_SRC_UTILS_MMAPFILE_H_

#include <string>
#include "gtest/gtest.h"

class MmapFile {
 public:
  explicit MmapFile(std::string filePath, size_t startOffset, size_t endOffset);
  ~MmapFile();
  std::string readContent(int startOffset, int size);
  void writeContent(const std::string &content, bool fsync = false);
 private:
  void loadFile();
 private:
  std::string filePath_;
  size_t size_;
  int fd_;
  char *data_;
 public:
  FRIEND_TEST(MmapFileTest, MmapFile);
};

#endif //DISTRIBUTEDMESSAGEQUEUE_SRC_UTILS_MMAPFILE_H_
