#定义一个变量，包含该目录下所有的源文件
aux_source_directory(. SRC_LIST)
#指定可执行生成文件
add_executable(ChatClient ${SRC_LIST})
#指定可执行文件链接时依赖的库文件
target_link_libraries(ChatClient pthread)