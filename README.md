# chatserver
可以工作在nginx,tcp负载均衡中的集群聊天服务器和客户端源码，基于muduo实现

编译方式：
cd build
rm -rf *
cmake ..
make
需要nginx，tcp负载均衡，需启动redis服务和mysql服务
