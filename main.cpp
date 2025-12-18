#include <iostream>
#include <stack>
#include <map>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")



#define out std::cout
#define end std::endl

int main()
{
  int iResult=0;
  //初始化网络模块
  WSADATA data;
  int ret=WSAStartup(MAKEWORD(2,2),&data);
  if (ret) {
    out << "Internet Init Failed." << end;
    return -1;
  }

  //创建套接字
  int lfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(lfd== INVALID_SOCKET) {
    out<<"socket create Failed."<<end;
    return -1;
  }

  //绑定本地ip
  sockaddr_in saddr;

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  saddr.sin_port = htons(5150);
  iResult=bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
  if(iResult==SOCKET_ERROR) {
    out<<"bind failed."<<end;
    closesocket(lfd);
    return -1;
  }

  //设置监听
  iResult=listen(lfd,128);
  if(iResult==SOCKET_ERROR) {
    out<<"listen failed."<<end;
    closesocket(lfd);
    return -1;
  }

  //阻塞并等待客户端链接
  sockaddr_in caddr;
  int caddr_len=sizeof(caddr);
  int cfd=accept(lfd,(sockaddr*)&caddr,&caddr_len);
  if(cfd==INVALID_SOCKET) {
    out<<"accept failed."<<end;
    closesocket(lfd);
    return -1;
  }

  //链接建立成功，打印客户端的IP和端口信息
  out<<"client ip addr:"<<inet_ntoa(caddr.sin_addr);
  out<<",";
  out<<"client port :"<<ntohs(caddr.sin_port)<<end;

  char buf[1024];

  while(1){
        memset(buf, 0, sizeof(buf));
        int len = recv(cfd, buf, sizeof(buf),0);
        if(len >0)
        {
            printf("客户端say: %s\n", buf);
            send(cfd, buf, len,0);
        }
        else if(len  == 0)
        {
            printf("客户端断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }

  }

  out<<"complie success !"<<end;
//关闭套接字
  closesocket(lfd);
  closesocket(cfd);
  //释放网络模块
  WSACleanup();
  return 0;
}
