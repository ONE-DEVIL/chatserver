#include"chatserver.hpp"
#include"chatservice.hpp"
#include<iostream>
#include<string>
#include<signal.h>
using namespace std;
void resetHandler(int){
    Chatservice::instance()->reset();
    exit(0);
}
int main(int argc,char** argv){
    if(argc<3){
        cerr<<"command invalid! example: ./ChatServer 127.0.0.1 6000"<<endl;
        exit(1);
    }
    char* ip=argv[1];
    uint16_t port=atoi(argv[2]);
    signal(SIGINT,resetHandler);
    EventLoop loop;
    InetAddress addr(ip,port);
    Chatserver server(&loop,addr,"chatserver");
    server.start();
    loop.loop();

    return 0;
}