#pragma once
#include <Windows.h>
#include <limits.h>
#include <string>
#include <mutex>
#include <vector>

using std::string;

class SysThread
{
public:
	SysThread(SOCKET);
	~SysThread();
	static const unsigned int INFINIT_WAIT;
	virtual bool start(void);
	virtual bool stop(unsigned int timeout = 0);
	virtual void run(void);
	std::string username;
	static std::vector <SysThread*> ThreadList;
	SOCKET acceptSocket;
	std::mutex mtx;
	void setPrivatePartener(std::string, std::string message);
	int userindex;

	inline volatile bool& isRunning(void)
	{
		return m_bRunning;
	}
	inline volatile bool& isExited(void)
	{
		return m_bExited;
	}
private:
	friend DWORD WINAPI runStub(LPVOID mthread);
	volatile bool m_bRunning;
	volatile bool m_bExited;
	HANDLE m_thread;
	void send_msg(std::string message, int dest_user_index);
	void getUsers();
	bool isMessageTypeDefinition(std::string message);
	void sendMessageToAll(std::string message);
	string privateMessageSet();
};

