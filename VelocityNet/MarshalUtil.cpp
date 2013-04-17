#include "MarshalUtil.h"

using namespace System::Runtime::InteropServices;

std::string ToNativeString(System::String^ str)
{
	System::IntPtr strPtr = Marshal::StringToHGlobalAnsi(str);
	std::string result(static_cast<const char*>(strPtr.ToPointer()));
	Marshal::FreeHGlobal(strPtr);

	return result;
}

std::wstring ToNativeWString(System::String^ str)
{
	System::IntPtr strPtr = Marshal::StringToHGlobalUni(str);
	std::wstring result(static_cast<const wchar_t*>(strPtr.ToPointer()));
	Marshal::FreeHGlobal(strPtr);

	return result;
}

System::String^ ToManagedString(const std::string& str)
{
	return gcnew System::String(str.c_str());
}

System::String^ ToManagedString(const std::wstring& str)
{
	return gcnew System::String(str.c_str());
}

unsigned long ToFatDateTime(System::DateTime^ dateTime)
{
	unsigned long result = 0;
	result |= (dateTime->Second >> 1) & 0x1F;
	result |= (dateTime->Minute & 0x3F) << 5;
	result |= (dateTime->Hour & 0x1F) << 11;
	result |= (dateTime->Day & 0x1F) << 16;
	result |= (dateTime->Month & 0x0F) << 21;
	result |= (dateTime->Year - 1980) << 25;
	return result;
}

System::DateTime^ ConvertFatDateTime(unsigned long dateTime)
{
	int second = (dateTime & 0x1F) << 1;
	int minute = (dateTime >> 5) & 0x3F;
	int hour = (dateTime >> 11) & 0x1F;
	int day = (dateTime >> 16) & 0x1F;
	int month = (dateTime >> 21) & 0x0F;
	int year = (dateTime >> 25) + 1980;

	// If the date or time is invalid, just return the current time
	if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
		return System::DateTime::Now;

	return gcnew System::DateTime(year, month, day, hour, minute, second);
}