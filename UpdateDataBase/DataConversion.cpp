#include "DataConversion.h"

#include <Windows.h>

using namespace std;

string toANSI(const wstring& unicodeString)
{
	int size = WideCharToMultiByte
	(
		CP_ACP,
		NULL,
		unicodeString.data(),
		-1,
		nullptr,
		NULL,
		NULL,
		NULL
	);

	string resultString;
	resultString.resize(size - 1);

	WideCharToMultiByte
	(
		CP_ACP,
		NULL,
		unicodeString.data(),
		-1,
		&resultString[0],
		size,
		NULL,
		NULL
	);

	return resultString;
}

wstring toOEM866(const string& string)
{
	int size = MultiByteToWideChar
	(
		CP_OEMCP,
		NULL,
		string.data(),
		-1,
		nullptr,
		NULL
	);

	wstring resultString;
	resultString.resize(size - 1);

	MultiByteToWideChar
	(
		CP_OEMCP,
		NULL,
		string.data(),
		-1,
		&resultString[0],
		size
	);

	return resultString;
}