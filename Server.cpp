#include <iostream>

#include <WS2tcpip.h>

#pragma comment (lib,"ws2_32.lib")

using namespace std;

void main()
{
    
//Initialza winsock

WSADATA wsData;
WORD ver =MAKEWORD(2,2);

int wsOk=WSAStartup(ver, &wsData);
if(wsOk!=0)
{
    cerr<<"Can't Initialize winsock! Quitting "<<endl;
    return;
}



//create a socket

SOCKET listening =socket(AF_INET,SOCK_STREAM,0);
if(listening==INVALID_SOCKET)
{
cerr<<"Can't create a socket! Quitting"<<endl;
return;

}


//bind an ip adress and port to a socket 

sockaddr_in hint;
hint.sin_family=AF_INET;
hint.sin_port=htons(54000);
hint.sin_addr.S_un.S_addr=INADDR_ANY;

bind(listening,(sockaddr*)&hint, sizeof(hint));



//Tell winsock the socket is lisstening 

listen(listening,SOMAXCONN);


//wait for a connection
sockaddr_in client;
int clientSize=sizeof(client);

SOCKET clientSocket =accept(listening, (sockaddr*)&client,&clientSize);

char host[NI_MAXHOST];
char service[NI_MAXHOST];


ZeroMemory(host,NI_MAXHOST);
ZeroMemory(service,NI_MAXSERV);


if (getnameinfo((sockaddr*)&client,sizeof(client),host,NI_MAXHOST,service ,NI_MAXSERV,0 )==0)
{
    cout<<host<<"Connected on port "<<service<<endl;


}
else
{

inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
cout<<host<<"conected on port "<<
ntohs(client.sin_port)<<endl;



}
//close listening socket 

closesocket(listening);


//while loop:accept and ehco message back to client

char buf[4096];

while(true)
{
    ZeroMemory(buf,4096);

//wait for client to send data 

int bytesRecived=recv(clientSocket,buf,4096,0);
if(bytesRecived==SOCKET_ERROR)
{
    cerr<<"Error in recv().Quitting"<<endl;
    break;
}

if(bytesRecived==0)
{
    cout<<"Client disconnected"<<endl;
    break;
}
//echo massage back to client

send(clientSocket,buf,bytesRecived+1,0);

}


//close the socket 

closesocket(clientSocket);


//cleanup winsock
WSACleanup();

}