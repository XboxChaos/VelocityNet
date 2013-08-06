#include "DotNetIO.h"

using namespace System::Runtime::InteropServices;

void DotNetIO::SetPosition(UINT64 position, std::ios_base::seek_dir dir)
{
	switch (dir)
	{
	case std::ios_base::beg:
		baseStream->Seek(position, SeekOrigin::Begin);
		break;
	case std::ios_base::cur:
		baseStream->Seek(position, SeekOrigin::Current);
		break;
	case std::ios_base::end:
		baseStream->Seek(position, SeekOrigin::End);
		break;
	}
}

UINT64 DotNetIO::GetPosition()
{
	return baseStream->Position;
}

UINT64 DotNetIO::Length()
{
	return baseStream->Length;
}

void DotNetIO::ReadBytes(BYTE *outBuffer, DWORD len)
{
	array<System::Byte>^ byteArray = gcnew array<System::Byte>(len);
	baseStream->Read(byteArray, 0, len);
	Marshal::Copy(byteArray, 0, System::IntPtr(outBuffer), len);
	delete byteArray;
}

void DotNetIO::WriteBytes(BYTE *buffer, DWORD len)
{
	array<System::Byte>^ byteArray = gcnew array<System::Byte>(len);
	Marshal::Copy(System::IntPtr(buffer), byteArray, 0, len);
	baseStream->Write(byteArray, 0, len);
	delete byteArray;
}

void DotNetIO::Flush()
{
	baseStream->Flush();
}

void DotNetIO::Close()
{
	baseStream->Close();
}