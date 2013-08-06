#pragma once

#pragma unmanaged
#include "IO/BaseIO.h"
#pragma managed

#include <vcclr.h>

using namespace System::IO;

/// <summary>
/// Implementation of BaseIO that works with a .NET stream.
/// </summary>
class DotNetIO : public BaseIO
{
public:
	DotNetIO(Stream^ stream) : baseStream(stream) { }
	virtual ~DotNetIO() { }

	virtual void SetPosition(UINT64 position, std::ios_base::seek_dir dir = std::ios_base::beg);
	virtual UINT64 GetPosition();
	virtual UINT64 Length();
	virtual void ReadBytes(BYTE *outBuffer, DWORD len);
	virtual void WriteBytes(BYTE *buffer, DWORD len);
	virtual void Flush();
	virtual void Close();

private:
	gcroot<Stream^> baseStream;
};