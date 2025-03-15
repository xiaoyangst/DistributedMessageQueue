cmake_minimum_required(VERSION 3.16)
project(Test)


add_executable(MmapFileTest ${CMAKE_SOURCE_DIR}/src/utils/MmapFile.cpp MmapFileTest.cpp)
target_link_libraries(MmapFileTest PRIVATE GTest::GTest GTest::Main pthread)
target_include_directories(MmapFileTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 设置生成 XML 报告
set(GTEST_OUTPUT "xml:${CMAKE_BINARY_DIR}/test_reports")

# 注册测试
include(GoogleTest)
gtest_discover_tests(MmapFileTest)