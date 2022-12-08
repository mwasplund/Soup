﻿#pragma once 

namespace Monitor
{
	#ifdef SOUP_BUILD
	export
	#endif
	class IDetourCallback
	{
	public:
		virtual void OnInitialize() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnError(std::string_view message) = 0;

		// FileApi
		virtual void OnAreFileApisANSI(bool result) = 0;
		virtual void OnCompareFileTime(int32_t result) = 0;
		virtual void OnCreateDirectoryA(std::string_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnCreateDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnCreateFile2(std::wstring_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition, uint64_t result, bool wasBlocked) = 0;
		virtual void OnCreateFileA(std::string_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition, uint32_t flagsAndAttributes, uint64_t result, bool wasBlocked) = 0;
		virtual void OnCreateFileW(std::wstring_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition, uint32_t flagsAndAttributes, uint64_t result, bool wasBlocked) = 0;
		virtual void OnDefineDosDeviceW(uint32_t flags, std::wstring_view deviceName, std::wstring_view targetPath, bool result) = 0;
		virtual void OnDeleteFileA(std::string_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnDeleteFileW(std::wstring_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnDeleteVolumeMountPointW(std::wstring_view volumeMountPoint, bool result) = 0;
		virtual void OnFileTimeToLocalFileTime(bool result) = 0;
		virtual void OnFindClose(bool result) = 0;
		virtual void OnFindCloseChangeNotification(bool result) = 0;
		virtual void OnFindFirstChangeNotificationA(std::string_view pathName, bool watchSubtree, uint32_t notifyFilter) = 0;
		virtual void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool watchSubtree, uint32_t notifyFilter) = 0;
		virtual void OnFindFirstFileA(std::string_view fileName) = 0;
		virtual void OnFindFirstFileW(std::wstring_view fileName) = 0;
		virtual void OnFindFirstFileExA(std::string_view fileName) = 0;
		virtual void OnFindFirstFileExW(std::wstring_view fileName) = 0;
		virtual void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t flags) = 0;
		virtual void OnFindFirstStreamW(std::wstring_view fileName) = 0;
		virtual void OnFindFirstVolumeW(std::wstring_view fileName) = 0;
		virtual void OnFindNextChangeNotification(bool result) = 0;
		virtual void OnFindNextFileA(bool result) = 0;
		virtual void OnFindNextFileW(bool result) = 0;
		virtual void OnFindNextFileNameW(bool result) = 0;
		virtual void OnFindNextStreamW(bool result) = 0;
		virtual void OnFindNextVolumeW(bool result) = 0;
		virtual void OnFindVolumeClose(bool result) = 0;
		virtual void OnFlushFileBuffers(bool result) = 0;
		virtual void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t result) = 0;
		virtual void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t result) = 0;
		virtual void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool result) = 0;
		virtual void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool result) = 0;
		virtual void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool result) = 0;
		virtual void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool result) = 0;
		virtual void OnGetDriveTypeA(std::string_view rootPathName, uint32_t result) = 0;
		virtual void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t result) = 0;
		virtual void OnGetFileAttributesA(std::string_view fileName, uint32_t result, bool wasBlocked) = 0;
		virtual void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result, bool wasBlocked) = 0;
		virtual void OnGetFileAttributesExA(std::string_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnGetFileAttributesExW(std::wstring_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnGetFileInformationByHandle(bool result) = 0;
		virtual void OnGetFileSize(uint32_t result) = 0;
		virtual void OnGetFileSizeEx(uint32_t result) = 0;
		virtual void OnGetFileTime(bool result) = 0;
		virtual void OnGetFileType(uint32_t result) = 0;
		virtual void OnGetFinalPathNameByHandleA(uint32_t result) = 0;
		virtual void OnGetFinalPathNameByHandleW(uint32_t result) = 0;
		virtual void OnGetFullPathNameA(std::string_view fileName, uint32_t result) = 0;
		virtual void OnGetFullPathNameW(std::wstring_view fileName, std::wstring_view buffer, uint32_t result) = 0;
		virtual void OnGetLogicalDrives(uint32_t result) = 0;
		virtual void OnGetLogicalDriveStringsW(std::wstring_view buffer, uint32_t result) = 0;
		virtual void OnGetLongPathNameA(std::string_view shortPath, std::string_view longPath, uint32_t result) = 0;
		virtual void OnGetLongPathNameW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) = 0;
		virtual void OnGetShortPathNameW(std::wstring_view longPath, std::wstring_view shortPath, uint32_t result) = 0;
		virtual void OnGetTempFileNameA(std::string_view pathName, std::string_view prefixString, uint32_t unique, std::string_view tempFileName, uint32_t result) = 0;
		virtual void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view prefixString, uint32_t unique, std::wstring_view tempFileName, uint32_t result) = 0;
		virtual void OnGetTempPathA(std::string_view buffer, uint32_t result) = 0;
		virtual void OnGetTempPathW(std::wstring_view buffer, uint32_t result) = 0;
		virtual void OnGetVolumeInformationA(bool result) = 0;
		virtual void OnGetVolumeInformationW(bool result) = 0;
		virtual void OnGetVolumeInformationByHandleW(bool result) = 0;
		virtual void OnGetVolumeNameForVolumeMountPointW(bool result) = 0;
		virtual void OnGetVolumePathNamesForVolumeNameW(bool result) = 0;
		virtual void OnGetVolumePathNameW(std::wstring_view filename, bool result) = 0;
		virtual void OnLocalFileTimeToFileTime(bool result) = 0;
		virtual void OnLockFile(bool result) = 0;
		virtual void OnLockFileEx(bool result) = 0;
		virtual void OnQueryDosDeviceW(std::wstring_view deviceName, uint32_t result) = 0;
		virtual void OnReadFile(bool result) = 0;
		virtual void OnReadFileEx(bool result) = 0;
		virtual void OnReadFileScatter(bool result) = 0;
		virtual void OnRemoveDirectoryA(std::string_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnRemoveDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnSetEndOfFile(bool result) = 0;
		virtual void OnSetFileApisToANSI() = 0;
		virtual void OnSetFileApisToOEM() = 0;
		virtual void OnSetFileAttributesA(std::string_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnSetFileAttributesW(std::wstring_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnSetFileInformationByHandle(bool result) = 0;
		virtual void OnSetFileIoOverlappedRange(bool result) = 0;
		virtual void OnSetFilePointer(uint32_t result) = 0;
		virtual void OnSetFilePointerEx(bool result) = 0;
		virtual void OnSetFileTime(bool result) = 0;
		virtual void OnSetFileValidData(bool result) = 0;
		virtual void OnUnlockFile(bool result) = 0;
		virtual void OnUnlockFileEx(bool result) = 0;
		virtual void OnWriteFile(bool result) = 0;
		virtual void OnWriteFileEx(bool result) = 0;
		virtual void OnWriteFileGather(bool result) = 0;

		// LibLoaderApi
		virtual void OnLoadLibraryA(std::string_view libFileName) = 0;
		virtual void OnLoadLibraryW(std::wstring_view libFileName) = 0;
		virtual void OnLoadLibraryExA(std::string_view libFileName) = 0;
		virtual void OnLoadLibraryExW(std::wstring_view libFileName) = 0;

		// ProcessEnv
		virtual void OnSearchPathA(std::string_view path, std::string_view fileName, std::string_view extension, uint32_t result) = 0;
		virtual void OnSearchPathW(std::wstring_view path, std::wstring_view fileName, std::wstring_view extension, uint32_t result) = 0;

		// ProcessThreadsApi
		virtual void OnCreateProcessA(bool wasDetoured, std::string_view applicationName, bool result) = 0;
		virtual void OnCreateProcessW(bool wasDetoured, std::wstring_view applicationName, bool result) = 0;
		virtual void OnCreateProcessAsUserA(std::string_view applicationName, bool result) = 0;
		virtual void OnCreateProcessAsUserW(std::wstring_view applicationName, bool result) = 0;
		virtual void OnExitProcess(uint32_t exitCode) = 0;

		// UndocumentedApi
		virtual void OnPrivCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnPrivCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) = 0;

		// WinBase
		virtual void OnCopyFileA(std::string_view existingFileName, std::string_view newFileName, bool failIfExists, bool result, bool wasBlocked) = 0;
		virtual void OnCopyFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool failIfExists, bool result, bool wasBlocked) = 0;
		virtual void OnCopyFile2(std::wstring_view existingFileName, std::wstring_view newFileName, uint64_t result, bool wasBlocked) = 0;
		virtual void OnCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnCopyFileTransactedA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnCopyFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnCreateDirectoryExA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) = 0;
		virtual void OnCreateDirectoryExW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) = 0;
		virtual void OnCreateDirectoryTransactedA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) = 0;
		virtual void OnCreateDirectoryTransactedW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) = 0;
		virtual void OnCreateFileTransactedA(std::string_view fileName, uint32_t desiredAccess, uint32_t shareMode, uint64_t result, bool wasBlocked) = 0;
		virtual void OnCreateFileTransactedW(std::wstring_view fileName, uint32_t desiredAccess, uint32_t shareMode, uint64_t result, bool wasBlocked) = 0;
		virtual void OnCreateHardLinkA(std::string_view fileName, std::string_view existingFileName, bool result) = 0;
		virtual void OnCreateHardLinkW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) = 0;
		virtual void OnCreateHardLinkTransactedA(std::string_view fileName, std::string_view existingFileName, bool result) = 0;
		virtual void OnCreateHardLinkTransactedW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) = 0;
		virtual void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool result) = 0;
		virtual void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool result) = 0;
		virtual void OnCreateSymbolicLinkA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) = 0;
		virtual void OnCreateSymbolicLinkW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) = 0;
		virtual void OnCreateSymbolicLinkTransactedA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) = 0;
		virtual void OnCreateSymbolicLinkTransactedW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) = 0;
		virtual void OnDecryptFileA(std::string_view fileName, bool result) = 0;
		virtual void OnDecryptFileW(std::wstring_view fileName, bool result) = 0;
		virtual void OnDeleteFileTransactedA(std::string_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnDeleteFileTransactedW(std::wstring_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnEncryptFileA(std::string_view fileName, bool result) = 0;
		virtual void OnEncryptFileW(std::wstring_view fileName, bool result) = 0;
		virtual void OnFileEncryptionStatusA(std::string_view fileName, bool result) = 0;
		virtual void OnFileEncryptionStatusW(std::wstring_view fileName, bool result) = 0;
		virtual void OnFindFirstFileNameTransactedW(std::wstring_view fileName, bool result) = 0;
		virtual void OnFindFirstFileTransactedA(std::string_view fileName) = 0;
		virtual void OnFindFirstFileTransactedW(std::wstring_view fileName) = 0;
		virtual void OnFindFirstStreamTransactedW(std::wstring_view fileName) = 0;
		virtual void OnGetBinaryTypeA(std::string_view applicationName, bool result) = 0;
		virtual void OnGetBinaryTypeW(std::wstring_view applicationName, bool result) = 0;
		virtual void OnGetCompressedFileSizeTransactedA(std::string_view fileName, uint32_t result) = 0;
		virtual void OnGetCompressedFileSizeTransactedW(std::wstring_view fileName, uint32_t result) = 0;
		virtual void OnGetDllDirectoryA(uint32_t result) = 0;
		virtual void OnGetDllDirectoryW(uint32_t result) = 0;
		virtual void OnGetEnvironmentVariableA(std::string_view name, uint32_t result) = 0;
		virtual void OnGetEnvironmentVariableW(std::wstring_view name, uint32_t result) = 0;
		virtual void OnGetFileAttributesTransactedA(std::string_view fileName, uint32_t result, bool wasBlocked) = 0;
		virtual void OnGetFileAttributesTransactedW(std::wstring_view fileName, uint32_t result, bool wasBlocked) = 0;
		virtual void OnGetFileBandwidthReservation(bool result) = 0;
		virtual void OnGetFileInformationByHandleEx(bool result) = 0;
		virtual void OnGetFileSecurityA(std::string_view fileName, bool result, bool wasBlocked) = 0;
		virtual void OnGetFullPathNameTransactedA(std::string_view fileName, uint32_t result) = 0;
		virtual void OnGetFullPathNameTransactedW(std::wstring_view fileName, uint32_t result) = 0;
		virtual void OnGetLongPathNameTransactedA(std::string_view shortPath, std::string_view longPath, uint32_t result) = 0;
		virtual void OnGetLongPathNameTransactedW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) = 0;
		virtual void OnGetQueuedCompletionStatus(bool result) = 0;
		virtual void OnGetQueuedCompletionStatusEx(bool result) = 0;
		virtual void OnGetShortPathNameA(std::string_view longPath, std::string_view shortPath, uint32_t result) = 0;
		virtual void OnLoadModule(std::string_view moduleName, uint32_t result) = 0;
		virtual void OnLoadPackagedLibrary(std::string_view libFileName) = 0;
		virtual void OnMoveFileA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileExA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileTransactedA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileWithProgressA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnMoveFileWithProgressW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) = 0;
		virtual void OnOpenEncryptedFileRawA(std::string_view fileName, uint32_t flags, uint32_t result) = 0;
		virtual void OnOpenEncryptedFileRawW(std::wstring_view fileName, uint32_t flags, uint32_t result) = 0;
		virtual void OnOpenFile(std::string_view fileName, bool wasBlocked) = 0;
		virtual void OnOpenFileById() = 0;
		virtual void OnReadEncryptedFileRaw(uint32_t result) = 0;
		virtual void OnRemoveDirectoryTransactedA(std::string_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnRemoveDirectoryTransactedW(std::wstring_view pathName, bool result, bool wasBlocked) = 0;
		virtual void OnReOpenFile() = 0;
		virtual void OnReplaceFileA(std::string_view replacedFileName, std::string_view replacementFileName, std::string_view backupFileName, uint32_t replaceFlags, bool result) = 0;
		virtual void OnReplaceFileW(std::wstring_view replacedFileName, std::wstring_view replacementFileName, std::wstring_view backupFileName, uint32_t replaceFlags, bool result) = 0;
		virtual void OnSetCurrentDirectoryA(std::string_view pathName, bool result) = 0;
		virtual void OnSetCurrentDirectoryW(std::wstring_view pathName, bool result) = 0;
		virtual void OnSetDllDirectoryA(std::string_view pathName, bool result) = 0;
		virtual void OnSetDllDirectoryW(std::wstring_view pathName, bool result) = 0;
		virtual void OnSetFileAttributesTransactedA(std::string_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) = 0;
		virtual void OnSetFileAttributesTransactedW(std::wstring_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) = 0;
		virtual void OnSetFileBandwidthReservation(bool result) = 0;
		virtual void OnSetFileCompletionNotificationModes(bool result) = 0;
		virtual void OnSetFileSecurityA(std::string_view fileName, bool result) = 0;
		virtual void OnSetFileShortNameA(std::string_view shortName, bool result) = 0;
		virtual void OnSetFileShortNameW(std::wstring_view shortName, bool result) = 0;
		virtual void OnSetSearchPathMode(uint32_t flags, bool result) = 0;
		virtual void OnWriteEncryptedFileRaw(uint32_t result) = 0;
	};
}
