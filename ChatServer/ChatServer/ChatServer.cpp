#include <iostream>
#include <stdio.h>
#include <WS2tcpip.h>
#include "winsock2.h"
#include "windows.h"
#include <list>
#include <vector>
#include "SysThread.h"
#pragma comment (lib,"ws2_32.lib")
using namespace std;

int main()
{
	int port = 13000;
	char ip[15] = "192.168.43.41";
	WSADATA wsaData;
	int wsaTest;
	wsaTest = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (wsaTest != NO_ERROR)
	{
		cout << "WSAStartup error!" << endl;
		return 1;
	}

	SOCKET listenSocket;

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error at socket!" << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in service;
	inet_pton(AF_INET, ip, &service.sin_addr.s_addr);
	service.sin_family = AF_INET;
	service.sin_port = htons(port);

	if (::bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{		//:: winsock bind miatt
		cout << "BIND error!" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, 1) == SOCKET_ERROR)
	{
		cout << "Error listening on socket" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	int index = 0;
	SOCKET acceptSocket;
	vector<SOCKET*> socketVector;
	vector<SysThread*> ThreadList;
	socketVector.push_back(new SOCKET);
	while (true)
	{
		acceptSocket = *socketVector[index];
		cout << "Wait to connect the client..." << endl;
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			cout << "Failed to accept on socket!" << WSAGetLastError() << endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			//	ServerThread* serverThread = new ServerThread();
			cout << "Client connected!" << endl;
			ThreadList.push_back(new SysThread(acceptSocket));//add the new thread to the vector
			ThreadList[index]->start();//start the new thread
			index++;
			socketVector.push_back(new SOCKET);//registering the new socket

		}
	}
	cout << "Closing..." << endl;
	closesocket(acceptSocket);
	WSACleanup();
	return 0;
}
