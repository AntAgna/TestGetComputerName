
#include <iostream>
#include <vector>

#include <Windows.h>
#include <iphlpapi.h>

using namespace std;

void callGetComputerNameExA()
{
	const int size = 255;
	char buffer[size] = {'\0'};
	DWORD resultSize = size;
	auto result = GetComputerNameExA(ComputerNameDnsHostname, buffer, &resultSize);

	if (result != 0)
		cout << "GetComputerNameEx() succeeded. Result : " << buffer << endl;
	else
		cout << "GetComputerNameEx() failed with error code : " << GetLastError() << endl;
}

void callGetNetworkParams()
{
	FIXED_INFO info;
	ULONG bufLen = sizeof(info);
	auto result = GetNetworkParams(&info, &bufLen);

	if (result == ERROR_SUCCESS)
		cout << "GetNetworkParams() succeeded. Result : " << info.HostName << endl;
	else
		cout << "GetNetworkParams() failed with error code " << result << endl;
}

void callGetNetworkParams2()
{
	PFIXED_INFO info = nullptr;
	ULONG bufLen = 0;
	auto result = GetNetworkParams(info, &bufLen);

	std::vector<char> buffer(bufLen, '\0');
	info = reinterpret_cast<FIXED_INFO*>(buffer.data());

	result = GetNetworkParams(info, &bufLen);

	if (result == ERROR_SUCCESS)
		cout << "Two-step GetNetworkParams() succeeded. Result : " << info->HostName << endl;
	else
		cout << "Two-step GetNetworkParams() failed with error code " << result << endl;
}

int cygwin_gethostname(char* name, size_t len)
{
	PFIXED_INFO info = NULL;
	ULONG size = 0;

	if (GetNetworkParams(info, &size) == ERROR_BUFFER_OVERFLOW
		&& (info = (PFIXED_INFO)alloca(size))
		&& GetNetworkParams(info, &size) == ERROR_SUCCESS)
	{
		strncpy(name, info->HostName, len);
		return 0;
	}

	return -1;
}

void call_cygwin_gethostname()
{
	const int NI_MAXHOST = 1025;
	char buf[NI_MAXHOST + 1];
	memset(buf, 0, sizeof buf);
	auto result = cygwin_gethostname(buf, sizeof buf - 1);

	if (result == 0)
		cout << "cygwin_gethostname() succeeded. Result : " << buf << endl;
	else
		cout << "cygwin_gethostname() failed with error code " << result << endl;
}

void call_git_hostname()
{
	auto result = system("\"c:\\Program Files\\Git\\usr\\bin\\hostname.exe\"");
	if (result == 0)
		cout << "Calling git hostname succeeded." << endl;
	else
		cout << "Calling git hostname failed with error code " << result << endl;
}

void call_git_uname()
{
	auto result = system("\"c:\\Program Files\\Git\\usr\\bin\\uname.exe\"");
	if (result == 0)
		cout << "Calling git uname succeeded." << endl;
	else
		cout << "Calling git uname failed with error code " << result << endl;
}

int main()
{
	cout << "Testing GetComputerNameEx()" << endl;
	callGetComputerNameExA();

	cout << "Testing GetNetworkParams()" << endl;
	callGetNetworkParams();

	cout << "Testing 2-step GetNetworkParams()" << endl;
	callGetNetworkParams2();

	cout << "Testing cygwin_gethostname()" << endl;
	call_cygwin_gethostname();

	cout << "Testing git hostname" << endl;
	call_git_hostname();

	cout << "Testing git uname" << endl;
	call_git_uname();
	
	return 0;
}
