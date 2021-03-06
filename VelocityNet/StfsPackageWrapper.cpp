#include "StfsPackageWrapper.h"
#include "MarshalUtil.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace VelocityNet
{
	namespace Stfs
	{
		StfsFileEntry::StfsFileEntry(const ::StfsFileEntry& entry)
		{
			Index = entry.entryIndex;
			IsConsecutive = ((entry.flags & ConsecutiveBlocks) == ConsecutiveBlocks);
			IsFolder = ((entry.flags & Folder) == Folder);
			Name = ToManagedString(entry.name);
			BlocksForFile = entry.blocksForFile;
			StartingBlock = entry.startingBlockNum;
			ParentEntryIndex = entry.pathIndicator;
			FileSize = entry.fileSize;
			CreatedTime = ConvertFatDateTime(entry.createdTimeStamp);
			AccessedTime = ConvertFatDateTime(entry.accessTimeStamp);
			EntryOffset = entry.fileEntryAddress;
		}

		::StfsFileEntry StfsFileEntry::ToNativeEntry()
		{
			::StfsFileEntry result;
			result.entryIndex = Index;
			result.flags = static_cast<int>(IsConsecutive) | (static_cast<int>(IsFolder) << 1);
			result.blocksForFile = BlocksForFile;
			result.startingBlockNum = StartingBlock;
			result.pathIndicator = ParentEntryIndex;
			result.fileSize = FileSize;
			result.createdTimeStamp = ToFatDateTime(CreatedTime);
			result.accessTimeStamp = ToFatDateTime(AccessedTime);
			result.fileEntryAddress = EntryOffset;
			result.name = ToNativeString(Name);
			result.nameLen = result.name.length();
			return result;
		}

		StfsFileListing::StfsFileListing()
		{
			files = gcnew List<StfsFileEntry^>();
			folders = gcnew List<StfsFileListing^>();
		}

		StfsFileListing::StfsFileListing(const ::StfsFileListing& listing)
		{
			files = gcnew List<StfsFileEntry^>();
			folders = gcnew List<StfsFileListing^>();
			Entry = gcnew StfsFileEntry(listing.folder);

			// Add files
			for (const ::StfsFileEntry& file : listing.fileEntries)
				files->Add(gcnew StfsFileEntry(file));

			// Add folders
			for (const ::StfsFileListing& folder : listing.folderEntries)
				folders->Add(gcnew StfsFileListing(folder));
		}

		StfsPackage::StfsPackage(System::String^ path)
		{
			Initialize(path, 0);
		}

		StfsPackage::StfsPackage(Stream^ stream)
		{
			Initialize(stream, 0);
		}

		StfsPackage::StfsPackage(System::String^ path, StfsPackageFlags flags)
			: io(NULL)
		{
			Initialize(path, (DWORD)flags);
		}

		StfsPackage::StfsPackage(Stream^ stream, StfsPackageFlags flags)
		{
			Initialize(stream, (DWORD)flags);
		}

		StfsPackage::!StfsPackage()
		{
			if (package)
			{
				delete package;
				package = NULL;
			}
			if (io)
			{
				delete io;
				io = NULL;
			}
		}

		void StfsPackage::Initialize(System::String^ path, DWORD flags)
		{
			Stream^ stream = File::Open(path, FileMode::OpenOrCreate, FileAccess::ReadWrite);
			try
			{
				Initialize(stream, flags);
			}
			finally
			{
				delete stream;
			}
		}

		void StfsPackage::Initialize(Stream^ stream, DWORD flags)
		{
			try
			{
				io = new DotNetIO(stream);
				package = new ::StfsPackage(io, (DWORD)flags);

				closed = false;
				LoadImages();
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::LoadImages()
		{
			thumbnailImage = gcnew array<System::Byte>(package->metaData->thumbnailImageSize);
			Marshal::Copy(System::IntPtr(package->metaData->thumbnailImage), thumbnailImage, 0, thumbnailImage->Length);

			titleThumbnailImage = gcnew array<System::Byte>(package->metaData->titleThumbnailImageSize);
			Marshal::Copy(System::IntPtr(package->metaData->titleThumbnailImage), titleThumbnailImage, 0, titleThumbnailImage->Length);
		}

		static void SaveImage(array<System::Byte>^ managedImage, BYTE** nativeImage, DWORD* sizePtr)
		{
			if (*nativeImage)
				delete [] *nativeImage;

			*sizePtr = managedImage->Length;
			*nativeImage = new BYTE[*sizePtr];
			Marshal::Copy(managedImage, 0, System::IntPtr(*nativeImage), managedImage->Length);
		}

		void StfsPackage::SaveImages()
		{
			SaveImage(thumbnailImage, &package->metaData->thumbnailImage, &package->metaData->thumbnailImageSize);
			SaveImage(titleThumbnailImage, &package->metaData->titleThumbnailImage, &package->metaData->titleThumbnailImageSize);
		}

		StfsFileListing^ StfsPackage::GetFileListing()
		{
			return GetFileListing(false);
		}

		StfsFileListing^ StfsPackage::GetFileListing(bool forceUpdate)
		{
			if (forceUpdate)
				AssertPackageIsOpen();
			try
			{
				return gcnew StfsFileListing(package->GetFileListing(forceUpdate));
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::ExtractFile(System::String^ pathInPackage, System::String^ outPath)
		{
			// The default file IO implementation in XI is really slow, so just use a .NET stream
			Stream^ stream = File::Open(outPath, FileMode::Create, FileAccess::Write);
			try
			{
				ExtractFile(pathInPackage, stream);
			}
			finally
			{
				delete stream;
			}
		}

		void StfsPackage::ExtractFile(System::String^ pathInPackage, Stream^ outStream)
		{
			AssertPackageIsOpen();
			try
			{
				DotNetIO netStream(outStream);
				package->ExtractFile(ToNativeString(pathInPackage), &netStream);
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::ExtractFile(StfsFileEntry^ file, System::String^ outPath)
		{
			// The default file IO implementation in XI is really slow, so just use a .NET stream
			Stream^ stream = File::Open(outPath, FileMode::Create, FileAccess::Write);
			try
			{
				ExtractFile(file, stream);
			}
			finally
			{
				delete stream;
			}
		}

		void StfsPackage::ExtractFile(StfsFileEntry^ file, Stream^ outStream)
		{
			AssertPackageIsOpen();
			::StfsFileEntry entry = file->ToNativeEntry();
			try
			{
				DotNetIO netStream(outStream);
				package->ExtractFile(&entry, &netStream);
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		System::UInt32 StfsPackage::GetFileMagic(System::String^ pathInPackage)
		{
			AssertPackageIsOpen();
			try
			{
				return package->GetFileMagic(ToNativeString(pathInPackage));
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		System::UInt32 StfsPackage::GetFileMagic(StfsFileEntry^ entry)
		{
			AssertPackageIsOpen();
			try
			{
				return package->GetFileMagic(entry->ToNativeEntry());
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		bool StfsPackage::FileExists(System::String^ pathInPackage)
		{
			try
			{
				return package->FileExists(ToNativeString(pathInPackage));
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		StfsFileEntry^ StfsPackage::InjectFile(System::String^ externalFilePath, System::String^ pathInPackage)
		{
			// The default file IO implementation in XI is really slow, so just use a .NET stream
			Stream^ stream = File::OpenRead(externalFilePath);
			try
			{
				return InjectFile(stream, pathInPackage);
			}
			finally
			{
				delete stream;
			}
		}

		StfsFileEntry^ StfsPackage::InjectFile(Stream^ stream, System::String^ pathInPackage)
		{
			AssertPackageIsOpen();
			try
			{
				DotNetIO netStream(stream);
				::StfsFileEntry entry = package->InjectFile(&netStream, ToNativeString(pathInPackage));
				return gcnew StfsFileEntry(entry);
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::ReplaceFile(System::String^ externalFilePath, System::String^ pathInPackage)
		{
			// The default file IO implementation in XI is really slow, so just use a .NET stream
			Stream^ stream = File::OpenRead(externalFilePath);
			try
			{
				ReplaceFile(stream, pathInPackage);
			}
			finally
			{
				delete stream;
			}
		}

		void StfsPackage::ReplaceFile(Stream^ stream, System::String^ pathInPackage)
		{
			AssertPackageIsOpen();
			try
			{
				DotNetIO netStream(stream);
				package->ReplaceFile(&netStream, ToNativeString(pathInPackage));
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::SaveMetadata()
		{
			AssertPackageIsOpen();
			SaveImages();
			try
			{
				package->metaData->WriteMetaData();
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::Rehash()
		{
			AssertPackageIsOpen();
			SaveImages(); // Rehash() calls WriteMetaData(), so images need to be saved back
			try
			{
				package->Rehash();
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::Resign(System::String^ kvPath)
		{
			// The default file IO implementation in XI is really slow, so just use a .NET stream
			Stream^ stream = File::OpenRead(kvPath);
			try
			{
				Resign(stream);
			}
			finally
			{
				delete stream;
			}
		}

		void StfsPackage::Resign(Stream^ kvStream)
		{
			AssertPackageIsOpen();
			try
			{
				DotNetIO netStream(kvStream);
				package->Resign(&netStream);
			}
			catch (const std::string& e)
			{
				throw gcnew System::InvalidOperationException(ToManagedString(e));
			}
		}

		void StfsPackage::SaveChanges(System::String^ kvPath)
		{
			Rehash();
			Resign(kvPath);
		}

		void StfsPackage::SaveChanges(Stream^ kvStream)
		{
			Rehash();
			Resign(kvStream);
		}

		void StfsPackage::Close()
		{
			if (!closed)
			{
				package->Close();
				closed = true;
			}
		}

		void StfsPackage::AssertPackageIsOpen()
		{
			if (closed)
				throw gcnew System::InvalidOperationException("Cannot operate on a closed package file.");
		}
	}
}