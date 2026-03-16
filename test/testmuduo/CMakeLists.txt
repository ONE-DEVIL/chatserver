cmake_minimum_required(VERSION 3.0)
project(main)
#配置选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
#配置头文件搜索路径
#include_director()
#配置库文件搜索路劲
#link_director()
set(SRC_LIST ./test_muduo.cpp)

#设置可执行文件的最终存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#把.指定路径下的所有源文件名字放入变量SRC_LIST里面
#aux_source_directory(. SRC_LIST)
#表示生成的可执行文件server，由SRC_LIST变量所定义的源文件编译而来
add_executable(server ${SRC_LIST})
#表示server这个目标程序，需要链接muduo_net muduo_base pthread这三个库文件
target_link_libraries(server muduo_net muduo_base pthread)