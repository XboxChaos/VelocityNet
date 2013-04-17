#pragma once
#include <string>

/// <summary>
/// Converts a managed System::String to a native std::string.
/// </summary>
/// <param name="str">The managed System::String to marshal.</param>
/// <returns>The native std::string.</returns>
std::string ToNativeString(System::String^ str);

/// <summary>
/// Converts a managed System::String to a native std::wstring.
/// </summary>
/// <param name="str">The managed System::String to marshal.</param>
/// <returns>The native std::wstring.</returns>
std::wstring ToNativeWString(System::String^ str);

/// <summary>
/// Converts a native std::string to an managed System::String.
/// </summary>
/// <param name="str">The native std::string to marshal.</param>
/// <returns>The managed System::String.</returns>
System::String^ ToManagedString(const std::string& str);

/// <summary>
/// Converts a native std::wstring to an managed System::String.
/// </summary>
/// <param name="str">The native std::wstring to marshal.</param>
/// <returns>The managed System::String.</returns>
System::String^ ToManagedString(const std::wstring& str);

/// <summary>
/// Converts a System::DateTime to a FAT date/time value.
/// </summary>
/// <param name="dateTime">The System::DateTime to convert.</param>
/// <returns>The FAT date/time corresponding to the input date/time.</returns>
unsigned long ToFatDateTime(System::DateTime^ dateTime);

/// <summary>
/// Converts a FAT date/time value to a System::DateTime.
/// </summary>
/// <param name="dateTime">The FAT date/time to convert.</param>
/// <returns>The System::DateTime corresponding to the FAT date/time.</returns>
System::DateTime^ ConvertFatDateTime(unsigned long dateTime);