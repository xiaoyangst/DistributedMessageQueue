/**
  ******************************************************************************
  * @file           : MmapFile.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/15
  ******************************************************************************
  */

#include "MmapFile.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <utility>

MmapFile::MmapFile(std::string filePath, size_t startOffset, size_t endOffset)
	: filePath_(std::move(filePath)), size_(endOffset - startOffset) {
	loadFile();
}

MmapFile::~MmapFile() {
	clean();
}

/**
 * @brief 打开文件 + mmap内存映射
 */
void MmapFile::loadFile() {
	fd_ = open(filePath_.c_str(), O_RDWR | O_CREAT, 0666);
	assert(fd_ != -1);

	// 预扩展文件大小
	struct stat fileStat{};
	if (fstat(fd_, &fileStat) == 0 && fileStat.st_size < size_) {
		auto err = ftruncate(fd_, size_);
		assert(err != -1);
	}

	data_ = static_cast<char *>(mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0));
	assert(data_ != MAP_FAILED);
}

/**
 * @brief 读取指定起始位置和要读取长度的数据
 * @param startOffset 读取内容的起始地址
 * @param size	读取长度
 * @return 读取到的数据
 */
std::string MmapFile::readContent(int startOffset, int size) {
	assert(startOffset >= 0);
	assert(size >= 0);
	assert(startOffset + size <= size_);

	return std::string(data_ + startOffset, size);
}

/**
 * @brief 写入数据到磁盘
 * @param content
 */
void MmapFile::writeContent(const std::string &content, bool fsync) {
	assert(content.size() <= size_);

	// 将内容写入内存映射区域
	std::memcpy(data_, content.c_str(), content.size());

	if (fsync) {
		auto err = msync(data_, size_, MS_SYNC);
		assert(err != -1);
	}
}

void MmapFile::clean() {
	if (data_ != MAP_FAILED && data_ != nullptr) {
		msync(data_, size_, MS_SYNC);
		munmap(data_, size_);
	}
	if (fd_ != -1) {
		close(fd_);
	}
}
