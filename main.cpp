#include <iostream>
#include <stack>
#include <map>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#define out std::cout
#define end std::endl

int main()
{
  int iResult=0;
  //初始化网络模块
  WSADATA data;
  int ret=WSAStartup(MAKEWORD(2,2),&data);
  if (ret) {
    out << "初始化网络错误！" << end;
    return -1;
  }

  //创建套接字
  int lfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(lfd== INVALID_SOCKET) {
    out<<"套接字创建失败"<<end;
    return -1;
  }

  //绑定本地ip
  sockaddr_in saddr;

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  saddr.sin_port = htons(5150);
  iResult=bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
  if(iResult==SOCKET_ERROR) {
    out<<"绑定失败"<<end;
    return -1;
  }

  //设置监听
  iResult=listen(lfd,128);
  if(iResult==SOCKET_ERROR) {
    out<<"监听失败"<<end;
    return -1;
  }

  //阻塞并等待客户端链接
  sockaddr_in caddr;
  int caddr_len=sizeof(caddr);
  int cfd=connect(lfd,(sockaddr*)&caddr,caddr_len);
  if(cfd==INVALID_SOCKET) {
    out<<"接受失败"<<end;
    return -1;
  }

  //链接建立成功，打印客户端的IP和端口信息
  //out<<"客户端的地址："<<inet_ntoa(caddr)<<end;

  out<<"complie success !"<<end;
//关闭套接字
  closesocket(lfd);
  closesocket(cfd);
  //释放网络模块
  WSACleanup();
  return 0;
}
