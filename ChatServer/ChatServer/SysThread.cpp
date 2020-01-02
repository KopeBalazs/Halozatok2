#include "SysThread.h"
#include <Windows.h>
#include <limits.h>
#include <iostream>
#include <string.h>

#define SIZE 26
#define BufLen  1024
#define INVALID_HANDLE_VALUE 0

const unsigned int SysThread::INFINIT_WAIT = UINT_MAX;
std::vector <SysThread*> SysThread::ThreadList;
using namespace std;
const int dataLength = 1024;
int index = 0;
int mt = -1;
int messageType = 0;
SysThread::SysThread(SOCKET socket)
{
	index++;
	ThreadList.push_back(this);
	acceptSocket = socket;
	m_bRunning = false;
	m_bExited = true;
	m_thread = INVALID_HANDLE_VALUE;
	username = "User" + index;
	userindex = index - 1;
}


SysThread::~SysThread()
{
}

DWORD WINAPI runStub(LPVOID mthread)
{
	SysThread* pThread = static_cast<SysThread*>(mthread);
	pThread->m_bRunning = true;
	pThread->run();
	pThread->m_bRunning = false;
	pThread->m_bExited = true;
	return 0;
}

bool SysThread::start(void)
{
	if (m_bExited)
	{
		m_bExited = false;
		DWORD dw;
		if ((m_thread = CreateThread(NULL, 4096, runStub, this, 0, &dw)) == INVALID_HANDLE_VALUE)
		{
			m_bRunning = false;
			m_bExited = true;
			return false;
		}
	}
	return true;
}

bool SysThread::stop(unsigned int timeout)
{
	m_bRunning = false;
	if (!m_bExited)
	{
		for (unsigned int i = 0; (i <= timeout / 100) || (timeout == INFINIT_WAIT); i++)
		{
			m_bRunning = false;
			if (m_bExited)
			{
				break;
			}
			Sleep(10);
		}
	}
	if (m_thread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_thread);
		m_thread = INVALID_HANDLE_VALUE;
	}
	return m_bExited;
}

void SysThread::run(void)
{
	vector<string> userNames;
	char recvData[dataLength];
	int bytesReceived = recv(acceptSocket, recvData, dataLength, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		cout << "Recv error1: " << WSAGetLastError() << endl;
		closesocket(acceptSocket);
		WSACleanup();
		return;
	}
	recvData[bytesReceived] = '\0';
	username = recvData;
	username = username.substr(0, bytesReceived - 2);
	cout << "User " << username << " connected!" << endl;
	while (true)
	{
		cout << "Waiting for message" << endl;
		int bytesReceived = recv(acceptSocket, recvData, dataLength, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "Recv error2: " << WSAGetLastError() << endl;
			closesocket(acceptSocket);
			WSACleanup();
			return;
		}
		recvData[bytesReceived] = '\0';
		cout << "Received message: " << recvData << endl;
		string recivedMessage = recvData;
		if (recivedMessage == "LSTUSRS\r\n")
		{
			getUsers();
		}
		int nameLength;
		if (recivedMessage.substr(0, 7) == "PRVTMSG")
		{
			nameLength = atoi(recivedMessage.substr(7, 1).c_str());
			cout << nameLength << "  " << recivedMessage.substr(8,nameLength) <<"  "<< recivedMessage.substr(8 + nameLength, recivedMessage.length())<< endl;
			setPrivatePartener(recivedMessage.substr(8, nameLength), recivedMessage.substr(8 + nameLength, recivedMessage.length()));
		}
		if (recivedMessage.substr(0, 2) == "LL")
		{
			sendMessageToAll(recivedMessage.substr(2, recivedMessage.length()));
		}
		int lastIndex=4;
		if (recivedMessage.substr(0, 3) == "GRP")
		{
			int howManyUsers = atoi(recivedMessage.substr(3, 1).c_str());
			for (int i = 0; i < howManyUsers; i++)
			{
				nameLength = atoi(recivedMessage.substr(lastIndex, 1).c_str());
				lastIndex++;
				userNames.push_back(recivedMessage.substr(lastIndex, nameLength));
				cout << howManyUsers << "  " << recivedMessage.substr(lastIndex, nameLength) << endl;
				lastIndex = lastIndex + nameLength;
			}
			for (int i = 0; i < howManyUsers; i++)
			{
				setPrivatePartener(userNames.back(), recivedMessage.substr(lastIndex, recivedMessage.length()));
				userNames.pop_back();
			}
		}
	}
}

void SysThread::send_msg(string message, int user_index)
{
	cout << "Sendig message: " << message << endl;
	int message_length = message.size() + 1;
	send(ThreadList[user_index]->acceptSocket, message.c_str(), message_length, 0);
}

void SysThread::getUsers()
{
	for (int i = 0; i < ThreadList.size(); i++)
	{
		Sleep(10);
		send_msg(ThreadList[i]->username + "USR\r\n", userindex);
		cout << ThreadList[i]->username << endl;
	}
}

void SysThread::setPrivatePartener(string partener, string message)
{

	for (int y = 0; y < ThreadList.size(); y++) {
		if (ThreadList[y]->username == partener) {
			mt = y;
			break;
		}
	}

	if (mt > -1) {
		send_msg(message, mt);
		cout << mt << endl;
	}
}

void SysThread::sendMessageToAll(string message)
{
	for (int y = 0; y < ThreadList.size(); y++) {
		if (y != userindex) {
			send_msg(message, y);
		}
	}

}