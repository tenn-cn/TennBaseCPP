// =======================================
// Unit   : File management routines (.h)
// Version: 3.0.1.0 (build 2012.06.29)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFileUtils_H_
#define _KYFileUtils_H_

#include <windows.h>
#include "KYDateTime.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量及结构定义 */

// 文件打开模式(file open mode)
enum TFileMode
{
   fmCreate          = 0xFFFF,
   fmOpenRead        = 0x0000,
   fmOpenWrite       = 0x0001,
   fmOpenReadWrite   = 0x0002,
   fmShareCompat     = 0x0000,
   fmShareExclusive  = 0x0010,
   fmShareDenyWrite  = 0x0020,
   fmShareDenyRead   = 0x0030,
   fmShareDenyNone   = 0x0040,

   // File mode magic numbers
   fmClosed          = 0xD7B0,
   fmInput           = 0xD7B1,
   fmOutput          = 0xD7B2,
   fmInOut           = 0xD7B3,
};

// 文件属性(file attribute constants)
enum TFileAttrib
{
   faReadOnly        = 0x00000001,
   faHidden          = 0x00000002,
   faSysFile         = 0x00000004,
   faVolumeID        = 0x00000008,
   faDirectory       = 0x00000010,
   faArchive         = 0x00000020,
   faAnyFile         = 0x0000003F,

   // Special file attribute
   faSpecial         = faHidden | faSysFile | faVolumeID | faDirectory,
};

// 文件定位位置(file seek origin)
enum TFileOrigin
{
   foSeekBegin       = 0,
   foSeekCurrent     = 1,
   foSeekEnd         = 2,
};

// 文件查找项(search record used by FindFirst, FindNext, and FindClose)
typedef struct
{
   Longword          Time;
   Longword          Size;
   Longword          Attr;
   KYString          Name;
   Longword          ExcludeAttr;
   THandle           FindHandle;
   WIN32_FIND_DATA   FindData;
} TSearchRec, *PSearchRec;

// 操作系统版本信息
extern Longword      Win32Platform;
extern Longword      Win32MajorVersion;
extern Longword      Win32MinorVersion;
extern Longword      Win32BuildNumber;
extern KYString      Win32CSDVersion;

// 当前可执行程序信息
extern THandle       HInstApp;
extern KYString      CurrAppFileName;
extern KYString      CurrAppFilePath;

// 当前模块信息
extern THandle       HInstance;
extern KYString      CurrModuleFileName;
extern KYString      CurrModuleFilePath;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 设置模块句柄, 当模块为 DLL 等不独立运行模块时使用
void     InitHInstance(THandle AHandle);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 文件读/写操作 */

/* FileOpen opens the specified file using the specified access mode. The
   access mode value is constructed by OR-ing one of the fmOpenXXXX constants
   with one of the fmShareXXXX constants. If the return value is positive,
   the function was successful and the value is the file handle of the opened
   file. A return value of INVALID_HANDLE_VALUE indicates that an error
   occurred. */

THandle FileOpen(const KYString& AFileName, unsigned AMode);

/* FileCreate creates a new file by the specified name. If the return value
   is positive, the function was successful and the value is the file handle
   of the new file. A return value of INVALID_HANDLE_VALUE indicates that an
   error occurred. */

THandle FileCreate(const KYString& AFileName);

/* FileRead reads Count bytes from the file given by Handle into the buffer
   specified by Buffer. The return value is the number of bytes actually
   read; it is less than Count if the end of the file was reached. The return
   value is -1 if an error occurred. */

long FileRead(THandle AHandle, void* ABuffer, long ACount);

/* FileWrite writes Count bytes to the file given by Handle from the buffer
   specified by Buffer. The return value is the number of bytes actually
   written, or -1 if an error occurred. */

long FileWrite(THandle AHandle, const void* ABuffer, long ACount);

/* FileSeek changes the current position of the file given by Handle to be
   Offset bytes relative to the point given by Origin. Origin = 0 means that
   Offset is relative to the beginning of the file, Origin = 1 means that
   Offset is relative to the current position, and Origin = 2 means that
   Offset is relative to the end of the file. The return value is the new
   current position, relative to the beginning of the file, or -1 if an error
   occurred. */

long    FileSeek(THandle AHandle, long AOffset, long AOrigin);
__int64 FileSeek(THandle AHandle, const __int64 AOffset, long AOrigin);

/* FileFlush clears the buffers for the specified file and causes all buffered
   data to be written to the file. */

void FileFlush(THandle AHandle);

/* FileClose closes the specified file. */

void FileClose(THandle AHandle);

/* FileSize gets the specified file size. */

__int64 FileGetSize(THandle AHandle);

/* FileSetEOF moves the end-of-file (EOF) position for the specified file to
   the current position of the file pointer. */

bool FileSetEOF(THandle AHandle);

/* FileLockRead locks from Offset read Count */

bool FileLockRead(THandle AHandle, Longword AOffset, Longword ACount);
bool FileLockRead(THandle AHandle, const __int64 AOffset, const __int64 ACount);

/* FileLockWrite locks from Offset write Count */

bool FileLockWrite(THandle AHandle, Longword AOffset, Longword ACount);
bool FileLockWrite(THandle AHandle, const __int64 AOffset, const __int64 ACount);

/* FileUnlock unlocks from Offset read or write Count */

bool FileUnlock(THandle AHandle, Longword AOffset, Longword ACount);
bool FileUnlock(THandle AHandle, const __int64 AOffset, const __int64 ACount);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 文件属性相关操作 */

/* FileDateToDateTime converts a DOS date-and-time value to a TDateTime
   value. The FileAge, FileGetDate, and FileSetDate routines operate on DOS
   date-and-time values, and the Time field of a TSearchRec used by the
   FindFirst and FindNext functions contains a DOS date-and-time value. */

TDateTime FileDateToDateTime(Longword AFileDate);

/* DateTimeToFileDate converts a TDateTime value to a DOS date-and-time
   value. The FileAge, FileGetDate, and FileSetDate routines operate on DOS
   date-and-time values, and the Time field of a TSearchRec used by the
   FindFirst and FindNext functions contains a DOS date-and-time value. */

Longword DateTimeToFileDate(TDateTime ADateTime);

/* FileGetDate returns the DOS date-and-time stamp of the file given by
   Handle. The return value is -1 if the handle is invalid. */

Longword FileGetDate(THandle AHandle);

/* FileSetDate sets the DOS date-and-time stamp of the file given by Handle
   to the value given by Age. The return value is zero if the function was
   successful. Otherwise the return value is a Windows error code. */

Longword FileSetDate(THandle AHandle, Longword Age);

/* FileAge returns the date-and-time stamp of the specified file. The return
   value can be converted to a TDateTime value using the FileDateToDateTime
   function. The return value is -1 if the file does not exist. */

Longword FileAge(const KYString& AFileName);

/* FileGetAttr returns the file attributes of the file given by FileName. The
   attributes can be examined by AND-ing with the faXXXX constants defined
   above. A return value of -1 indicates that an error occurred. */

Longword FileGetAttr(const KYString& AFileName);

/* FileSetAttr sets the file attributes of the file given by FileName to the
   value given by Attr. The attribute value is formed by OR-ing the
   appropriate faXXXX constants. The return value is zero if the function was
   successful. Otherwise the return value is a Windows error code. */

Longword FileSetAttr(const KYString& AFileName, Longword Attrib);

/* FileExists returns a boolean value that indicates whether the specified
   file exists. */

bool FileExists(const KYString& AFileName);

/* DeleteFile deletes the file given by FileName. The return value is true if
   the file was successfully deleted, or false if an error occurred. */

bool DeleteFile(const KYString& AFileName);

/* RenameFile renames the file given by OldName to the name given by NewName.
   The return value is true if the file was successfully renamed, or false if
   an error occurred. */

bool RenameFile(const KYString& AOldName, const KYString& ANewName);

/* CopyFile copies an existing file to a new file */

bool CopyFile(const KYString& AFileName, const KYString& ANewFile, bool AIsOverlay);

/* DirExists returns a boolean value that indicates whether the specified
   directory exists. */

bool DirExists(const KYString& ADir);

/* CreateDir creates a new directory. The return value is true if a new
   directory was successfully created, or false if an error occurred. */

bool CreateDir(const KYString& ADir);
bool ForceCreateDir(const KYString& ADir);

/* RemoveDir deletes an existing empty directory. The return value is
   true if the directory was successfully deleted, or false if an error
   occurred. */

bool RemoveDir(const KYString& ADir);

/* GetCurrentDir returns the current directory. */

KYString GetCurrentDir();

/* SetCurrentDir sets the current directory. The return value is true if
   the current directory was successfully changed, or false if an error
   occurred. */

bool SetCurrentDir(const KYString& ADir);

/* DiskFreeSpace retrieves information about the amount of space available on
   the specified drive number, where 0 = Current, 1 = A, 2 = B, etc. return
   the total amount of space and the total amount of free space to the user
   associated with the calling thread. */

bool DiskFreeSpace(Byte ADrive, __int64& ATotalSpace, __int64& AFreeSpace);
bool DiskFreeSpace(const char* APath, __int64& ATotalSpace, __int64& AFreeSpace);

/* DiskFree returns the number of free bytes on the specified drive number,
   where 0 = Current, 1 = A, 2 = B, etc. DiskFree returns -1 if the drive
   number is invalid. */

__int64 DiskFree(Byte ADrive);
__int64 DiskFree(const char* APath = NULL);

/* DiskSize returns the size in bytes of the specified drive number, where
   0 = Current, 1 = A, 2 = B, etc. DiskSize returns -1 if the drive number
   is invalid. */

__int64 DiskSize(Byte ADrive);
__int64 DiskSize(const char* APath = NULL);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 文件名相关操作 */

// 得到带路径 APath 的文件名
KYString GetFileName(const KYString& APath, const KYString& AFileName);

/* IsPathDelimiter returns true if the character at byte AStr[AIndex]
   is '\', and it is not a MBCS lead or trail byte. */

bool IsPathDelimiter(const KYString& AStr, long AIndex);

/* IsDelimiter returns true if the character at byte AStr[AIndex] matches any
   character in the Delimiters string, and the character is not a MBCS lead or
   trail byte.  AStr may contain multibyte characters; Delimiters must contain
   only single byte characters. */

bool IsDelimiter(const KYString& ADelimiters, const KYString& AStr, long AIndex);

/* IncludeTrailingBackslash returns the path with a '\' at the end.
   This function is MBCS enabled. */

KYString IncludeTrailingBackslash(const KYString& AStr);

/* ExcludeTrailingBackslash returns the path without a '\' at the end.
   This function is MBCS enabled. */

KYString ExcludeTrailingBackslash(const KYString& AStr);

/* LastDelimiter returns the byte index in AStr of the rightmost whole
   character that matches any character in Delimiters (except null (#0)).
   AStr may contain multibyte characters; Delimiters must contain only single
   byte non-null characters.
   Example: LastDelimiter('\.:', 'c:\filename.ext') returns 12. */

long LastDelimiter(const KYString& ADelimiters, const KYString& AStr);

/* ChangeFileExt changes the extension of a filename. FileName specifies a
   filename with or without an extension, and Extension specifies the new
   extension for the filename. The new extension can be a an empty string or
   a period followed by up to three characters. */

KYString ChangeFileExt(const KYString& AFileName, const KYString& AExtension);

/* ExtractFilePath extracts the drive and directory parts of the given
   filename. The resulting string is the leftmost characters of FileName,
   up to and including the colon or backslash that separates the path
   information from the name and extension. The resulting string is empty
   if FileName contains no drive and directory parts. */

KYString ExtractFilePath(const KYString& AFileName);

/* ExtractFileDir extracts the drive and directory parts of the given
   filename. The resulting string is a directory name suitable for passing
   to SetCurrentDir, CreateDir, etc. The resulting string is empty if
   FileName contains no drive and directory parts. */

KYString ExtractFileDir(const KYString& AFileName);

/* ExtractFileDrive extracts the drive part of the given filename.  For
   filenames with drive letters, the resulting string is '<drive>:'.
   For filenames with a UNC path, the resulting string is in the form
   '\\<servername>\<sharename>'.  If the given path contains neither
   style of filename, the result is an empty string. */

KYString ExtractFileDrive(const KYString& AFileName);

/* ExtractFileName extracts the name and extension parts of the given
   filename. The resulting string is the leftmost characters of FileName,
   starting with the first character after the colon or backslash that
   separates the path information from the name and extension. The resulting
   string is equal to FileName if FileName contains no drive and directory
   parts. */

KYString ExtractFileName(const KYString& AFileName);

/* ExtractFileExt extracts the extension part of the given filename. The
   resulting string includes the period character that separates the name
   and extension parts. The resulting string is empty if the given filename
   has no extension. */

KYString ExtractFileExt(const KYString& AFileName);

/* ExpandFileName expands the given filename to a fully qualified filename.
   The resulting string consists of a drive letter, a colon, a root relative
   directory path, and a filename. Embedded '.' and '..' directory references
   are removed. */

KYString ExpandFileName(const KYString& AFileName);

/* ExpandUNCFileName expands the given filename to a fully qualified filename.
   This function is the same as ExpandFileName except that it will return the
   drive portion of the filename in the format '\\<servername>\<sharename> if
   that drive is actually a network resource instead of a local resource.
   Like ExpandFileName, embedded '.' and '..' directory references are
   removed. */

KYString ExpandUNCFileName(const KYString& AFileName);

/* ExtractRelativePath will return a file path name relative to the given
   BaseName.  It strips the common path dirs and adds '..\' for each level
   up from the BaseName path. */

KYString ExtractRelativePath(const KYString& ABaseName, const KYString& ADestName);

/* ExtractShortPathName will convert the given filename to the short form
   by calling the GetShortPathName API.  Will return an empty string if
   the file or directory specified does not exist */

KYString ExtractShortPathName(const KYString& AFileName);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 文件查找操作 */

/* FindFirst searches the directory given by Path for the first entry that
   matches the filename given by Path and the attributes given by Attr. The
   result is returned in the search record given by SearchRec. The return
   value is zero if the function was successful. Otherwise the return value
   is a Windows error code. FindFirst is typically used in conjunction with
   FindNext and FindClose as follows:

   result = FindFirst(Path, Attr, ARec);
   while (result == 0)
   {
      ProcessSearchRec(ARec);
      result = FindNext(ARec);
   }
   FindClose(ARec);

   where ProcessSearchRec represents user-defined code that processes the
   information in a search record. */

Longword FindFirst(const KYString& APath, unsigned Attr, TSearchRec& ARec);

/* FindNext returs the next entry that matches the name and attributes
   specified in a previous call to FindFirst. The search record must be one
   that was passed to FindFirst. The return value is zero if the function was
   successful. Otherwise the return value is a Windows error code. */

Longword FindNext(TSearchRec& ARec);

/* FindClose terminates a FindFirst/FindNext sequence. FindClose does nothing
   in the 16-bit version of Windows, but is required in the 32-bit version,
   so for maximum portability every FindFirst/FindNext sequence should end
   with a call to FindClose. */

void FindClose(TSearchRec& ARec);

/* FileSearch searches for the file given by Name in the list of directories
   given by DirList. The directory paths in DirList must be separated by
   semicolons. The search always starts with the current directory of the
   current drive. The returned value is a concatenation of one of the
   directory paths and the filename, or an empty string if the file could not
   be located. */

KYString FileSearch(const KYString& AName, const KYString& ADirList);

}

#endif
