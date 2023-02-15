
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

int main()
{
	cout << "Testing GetComputerNameEx()" << endl;
	callGetComputerNameExA();

	cout << "Testing GetNetworkParams()" << endl;
	callGetNetworkParams();

	cout << "Testing 2-step GetNetworkParams()" << endl;
	callGetNetworkParams2();
	
	return 0;
}
