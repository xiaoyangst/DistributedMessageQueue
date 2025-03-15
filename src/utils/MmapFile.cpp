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
	if (data_ != MAP_FAILED && data_ != nullptr) {
		msync(data_, size_, MS_SYNC);
		munmap(data_, size_);
	}
	if (fd_ != -1) {
		close(fd_);
	}
}


/**
 * @brief 打开文件 + mmap内存映射
 */
void MmapFile::loadFile() {
	fd_ = open(filePath_.c_str(), O_RDWR | O_CREAT, 0666);
	if (fd_ == -1) {
		throw std::runtime_error("open file failed");
	}

	// 预扩展文件大小
	struct stat fileStat{};
	if (fstat(fd_, &fileStat) == 0 && fileStat.st_size < size_) {
		if (ftruncate(fd_, size_) == -1) {
			throw std::runtime_error("Failed to extend file size");
		}
	}

	data_ = static_cast<char *>(mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0));
	if (data_ == MAP_FAILED) {
		throw std::runtime_error("mmap failed");
	}
}

/**
 * @brief 读取指定起始位置和要读取长度的数据
 * @param startOffset 读取内容的起始地址
 * @param size	读取长度
 * @return 读取到的数据
 */
std::string MmapFile::readContent(int startOffset, int size) {
	if (startOffset < 0 || size < 0 || startOffset + size > size_) {
		throw std::runtime_error("Invalid read range");
	}
	return std::string(data_ + startOffset, size);
}


/**
 * @brief 写入数据到磁盘
 * @param content
 */
void MmapFile::writeContent(const std::string &content, bool fsync) {
	if (content.size() > size_) {
		throw std::runtime_error("write out of range");
	}
	// 将内容写入内存映射区域
	std::memcpy(data_, content.c_str(), content.size());

	if (fsync) {
		if (msync(data_, size_, MS_SYNC) == -1) {
			throw std::runtime_error("Failed to sync data to disk");
		}
	}
}