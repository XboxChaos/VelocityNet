#pragma once

#pragma unmanaged
#include "XboxInternals\Stfs\StfsPackage.h"
#pragma managed

#include "MarshalUtil.h"

using namespace System::Collections::Generic;

namespace VelocityNet
{
	namespace Stfs
	{
		/// <summary>
		/// STFS package loading/creation flags.
		/// </summary>
		[System::Flags]
		public enum class StfsPackageFlags
		{
			/// <summary>
			/// Profile-embedded content package
			/// </summary>
			PEC,

			/// <summary>
			/// Create a new package
			/// </summary>
			Create,

			/// <summary>
			/// Read-only package (LIVE/PIRS)
			/// </summary>
			Female
		};

		/// <summary>
		/// STFS package types.
		/// </summary>
		public enum class StfsPackageType
		{
			Con = 0x434F4E20,
			Live = 0x4C495645,
			Pirs = 0x50495253
		};

		/// <summary>
		/// STFS package content types.
		/// </summary>
		public enum struct StfsContentType
		{
			ArcadeGame = 0xD0000,
			AvatarAssetPack = 0x8000,
			AvatarItem = 0x9000,
			CacheFile = 0x40000,
			CommunityGame = 0x2000000,
			GameDemo = 0x80000,
			GameOnDemand = 0x7000,
			GamerPicture = 0x20000,
			GamerTitle = 0xA0000,
			GameTrailer = 0xC0000,
			GameVideo = 0x400000,
			InstalledGame = 0x4000,
			Installer = 0xB0000,
			IPTVPauseBuffer = 0x2000,
			LicenseStore = 0xF0000,
			MarketPlaceContent = 2,
			Movie = 0x100000, 
			MusicVideo = 0x300000,
			PodcastVideo = 0x500000,
			Profile = 0x10000,
			Publisher = 3,
			SavedGame = 1, 
			StorageDownload = 0x50000,
			Theme = 0x30000,
			Video = 0x200000,
			ViralVideo = 0x600000,
			XboxDownload = 0x70000,
			XboxOriginalGame = 0x5000,
			XboxSavedGame = 0x60000,
			Xbox360Title = 0x1000,
			XNA = 0xE0000
		};

		/// <summary>
		/// Describes a file or a folder in an STFS package.
		/// </summary>
		public ref class StfsFileEntry
		{
		public:
			/// <summary>
			/// Gets or sets the entry's index.
			/// </summary>
			property System::UInt32 Index;

			/// <summary>
			/// Gets or sets the name of the file.
			/// </summary>
			property System::String^ Name;

			/// <summary>
			/// Gets or sets whether or not the entry represents a folder.
			/// </summary>
			property bool IsFolder;

			/// <summary>
			/// Gets or sets whether or not the file's blocks are consecutive in the package.
			/// </summary>
			property bool IsConsecutive;

			/// <summary>
			/// Gets or sets the number of blocks that the file occupies.
			/// </summary>
			property System::Int32 BlocksForFile;

			/// <summary>
			/// Gets or sets the number of the first block in the file.
			/// </summary>
			property System::Int32 StartingBlock;

			/// <summary>
			/// Gets or sets the entry index of the file's parent folder. Can be -1 if the file is at the root of the package.
			/// </summary>
			property System::Int16 ParentEntryIndex;

			/// <summary>
			/// Gets or sets the size of the file in bytes.
			/// </summary>
			property System::UInt32 FileSize;

			/// <summary>
			/// Gets or sets the time at which the file was created.
			/// </summary>
			property System::DateTime^ CreatedTime;

			/// <summary>
			/// Gets or sets the time at which the file was last accessed.
			/// </summary>
			property System::DateTime^ AccessedTime;

			/// <summary>
			/// Gets or sets the offset of the file entry.
			/// </summary>
			property System::UInt32 EntryOffset;

		internal:
			StfsFileEntry(const ::StfsFileEntry& entry);
			::StfsFileEntry ToNativeEntry();
		};

		/// <summary>
		/// A listing of files and subfolders in a folder in an STFS container.
		/// </summary>
		public ref class StfsFileListing
		{
		public:
			StfsFileListing();

			/// <summary>
			/// Gets a list of files in the folder.
			/// </summary>
			property List<StfsFileEntry^>^ Files
			{
				List<StfsFileEntry^>^ get() { return files; }
			}

			/// <summary>
			/// Gets a list of subfolders in this folder.
			/// </summary>
			property List<StfsFileListing^>^ Folders
			{
				List<StfsFileListing^>^ get() { return folders; }
			}

			/// <summary>
			/// Gets the file entry corresponding to this listing.
			/// </summary>
			property StfsFileEntry^ Entry;

		internal:
			StfsFileListing(const ::StfsFileListing& listing);

		private:
			List<StfsFileEntry^>^ files;
			List<StfsFileListing^>^ folders;
		};

		/// <summary>
		/// An STFS package.
		/// </summary>
		public ref class StfsPackage
		{
		public:
			/// <summary>
			/// Opens an STFS package from a file using the default settings.
			/// </summary>
			/// <param name="path">The path to the STFS container to open.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while loading the package.</exception>
			StfsPackage(System::String^ path);

			/// <summary>
			/// Opens or creates an STFS package based upon flags that are specified.
			/// </summary>
			/// <param name="path">The path to the STFS container to open.</param>
			/// <param name="flags">Flags to use to determine how to open or create the package.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while loading the package.</exception>
			StfsPackage(System::String^ path, StfsPackageFlags flags);

			/// <summary>
			/// Gets the package's type (CON, LIVE, or PIRS).
			/// </summary>
			/// <seealso cref="StfsPackageType" />
			property StfsPackageType Type
			{
				StfsPackageType get() { return static_cast<StfsPackageType>(package->metaData->magic); }
			}

			/// <summary>
			/// Gets or sets the package's content type.
			/// </summary>
			/// <seealso cref="StfsContentType" />
			property StfsContentType ContentType
			{
				StfsContentType get() { return static_cast<StfsContentType>(package->metaData->contentType); }
				void set(StfsContentType value) { package->metaData->contentType = static_cast<::ContentType>(value); }
			}

			/// <summary>
			/// Gets or sets the package's media ID.
			/// </summary>
			property System::UInt32 MediaID
			{
				System::UInt32 get() { return package->metaData->mediaID; }
				void set(System::UInt32 value) { package->metaData->mediaID = value; }
			}

			/// <summary>
			/// Gets or sets the package's title ID.
			/// </summary>
			property System::UInt32 TitleID
			{
				System::UInt32 get() { return package->metaData->titleID; }
				void set(System::UInt32 value) { package->metaData->titleID = value; }
			}

			/// <summary>
			/// Gets or sets the package's displayed name.
			/// </summary>
			property System::String^ Name
			{
				System::String^ get() { return ToManagedString(package->metaData->displayName); }
				void set(System::String^ value) { package->metaData->displayName = ToNativeWString(value); }
			}

			/// <summary>
			/// Gets or sets the package's displayed description.
			/// </summary>
			property System::String^ Description
			{
				System::String^ get() { return ToManagedString(package->metaData->displayDescription); }
				void set(System::String^ value) { package->metaData->displayDescription = ToNativeWString(value); }
			}

			/// <summary>
			/// Gets or sets the package's publisher.
			/// </summary>
			property System::String^ Publisher
			{
				System::String^ get() { return ToManagedString(package->metaData->publisherName); }
				void set(System::String^ value) { package->metaData->publisherName = ToNativeWString(value); }
			}

			/// <summary>
			/// Gets or sets the package's title name.
			/// </summary>
			property System::String^ TitleName
			{
				System::String^ get() { return ToManagedString(package->metaData->titleName); }
				void set(System::String^ value) { package->metaData->titleName = ToNativeWString(value); }
			}

			/// <summary>
			/// Gets or sets the package's thumbnail image data.
			/// </summary>
			property array<System::Byte>^ ThumbnailImage
			{
				array<System::Byte>^ get() { return thumbnailImage; }
				void set(array<System::Byte>^ value) { thumbnailImage = value; }
			}

			/// <summary>
			/// Gets or sets the package's title thumbnail image data.
			/// </summary>
			property array<System::Byte>^ TitleThumbnailImage
			{
				array<System::Byte>^ get() { return titleThumbnailImage; }
				void set(array<System::Byte>^ value) { titleThumbnailImage = value; }
			}

			/// <summary>
			/// Retrieves a listing of all files in the package.
			/// </summary>
			/// <returns>A listing containing information for every file in the package.</returns>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while retrieving the listing.</exception>
			StfsFileListing^ GetFileListing();

			/// <summary>
			/// Retrieves a listing of all files in the package, optionally forcing a re-read from the package.
			/// </summary>
			/// <param name="forceUpdate">true if the file listing should be re-read from the package file.</param>
			/// <returns>A listing containing information for every file in the package.</returns>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while retrieving the listing.</exception>
			StfsFileListing^ GetFileListing(bool forceUpdate);

			/// <summary>
			/// Extracts a file in the package to a designated path.
			/// </summary>
			/// <param name="pathInPackage">The path to the file to extract in the package. Path components should be separated with backslashes.</param>
			/// <param name="outPath">The path to the file to extract it to.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while extracting the file.</exception>
			void ExtractFile(System::String^ pathInPackage, System::String^ outPath);

			/// <summary>
			/// Extracts a file in the package to a designated path.
			/// </summary>
			/// <param name="file">The file entry of the file to extract.</param>
			/// <param name="outPath">The path to the file to extract it to.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while extracting the file.</exception>
			void ExtractFile(StfsFileEntry^ file, System::String^ outPath);

			/// <summary>
			/// Gets the first four bytes of a file in the package. Useful for verifying a file's format.
			/// </summary>
			/// <param name="pathInPackage">The path to the file to inspect in the package. Path components should be separated with backslashes.</param>
			/// <returns>The first four bytes of the file as a <see cref="System::UInt32" />. If the file is less than four bytes large, 0 will be returned.</returns>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while reading the file.</exception>
			System::UInt32 GetFileMagic(System::String^ pathInPackage);

			/// <summary>
			/// Gets the first four bytes of a file in the package. Useful for verifying a file's format.
			/// </summary>
			/// <param name="entry">The file entry of the file to inspect.</param>
			/// <returns>The first four bytes of the file as a <see cref="System::UInt32" />. If the file is less than four bytes large, 0 will be returned.</returns>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while reading the file.</exception>
			System::UInt32 GetFileMagic(StfsFileEntry^ entry);

			/// <summary>
			/// Determines whether or not a file exists in the package.
			/// </summary>
			/// <param name="pathInPackage">The path to the file to check for in the package. Path components should be separated with backslashes.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while searching for the file.</exception>
			bool FileExists(System::String^ pathInPackage);

			/// <summary>
			/// Injects an external file into the package.
			/// </summary>
			/// <param name="externalFilePath">The path to the external file to inject.</param>
			/// <param name="pathInPackage">The file's target path in the package. Path components should be separated with backslashes.</param>
			/// <returns>The <see cref="StfsFileEntry"/> for the injected file.</returns>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while injecting the file.</exception>
			StfsFileEntry^ InjectFile(System::String^ externalFilePath, System::String^ pathInPackage);

			/// <summary>
			/// Replaces a file in the package with a new one.
			/// </summary>
			/// <param name="externalFilePath">The path to the external file to replace the file in the package with.</param>
			/// <param name="pathInPackage">The path to the file to replace in the package. Path components should be separated with backslashes.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while replacing the file.</exception>
			void ReplaceFile(System::String^ externalFilePath, System::String^ pathInPackage);

			/// <summary>
			/// Saves any changes made to the package's metadata (name, description, etc.) and writes them back to the package file.
			/// Note that <see cref="Rehash"/> does this automatically.
			/// </summary>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while saving the package's metadata.</exception>
			void SaveMetadata();

			/// <summary>
			/// Recomputes all hashes in the package in order to ensure its validity.
			/// The package's metadata will also be saved.
			/// </summary>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while rehashing the package.</exception>
			void Rehash();

			/// <summary>
			/// Resigns the package using data stored in a KV.
			/// </summary>
			/// <param name="kvPath">The path to the KV file to use to resign the package.</param>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while resigning the package.</exception>
			void Resign(System::String^ kvPath);

			/// <summary>
			/// Rehashes the package, resigns it using data stored in a KV, and then writes any changed metadata back to the package file.
			/// This is a shortcut method for calling <see cref="Rehash"/> and <see cref="Resign"/> together.
			/// </summary>
			/// <exception cref="System::InvalidOperationException">Thrown if an error occurs while saving changes made to the package.</exception>
			void SaveChanges(System::String^ kvPath);

			/// <summary>
			/// Closes the package, releasing any I/O resources it acquired.
			/// After the package has been closed, no further I/O operations may be performed on it.
			/// </summary>
			void Close();

			~StfsPackage() { this->!StfsPackage(); }
			!StfsPackage();

		private:
			::StfsPackage* package;
			bool closed;

			array<System::Byte>^ thumbnailImage;
			array<System::Byte>^ titleThumbnailImage;

			void Initialize(System::String^ path, StfsPackageFlags flags);
			void LoadImages();
			void SaveImages();
			void AssertPackageIsOpen();
		};
	}
}
