#pragma once

// Required for TCHAR
#include <tchar.h>

// Required for sfile streams
#include <fstream>

// Required for assertions
#include <assert.h>

// Contains all the error codes
#include "Errors.h"

#define FM_EXTEND 1

class FileManager
{
	std::fstream File;
	int lastErr = ERR_SUCCESS;

	void SetLastError (int errCode);
	const TCHAR* FilePath = nullptr;

public:

	// Returns wether a file is open.
	bool IsOpen();

	FileManager();

	// Retrieves the last error.
	int  GetLastError ();
	int  GetLastError (int& err);
	
	// Opens the file stream.
	// When using the `Open` function to open a file, you can pass a boolean determining wether the file should be created, 
	// if it doesn't exist, therefore, you can use this function to check if a file exists. This option is not present when 
	// useing the constructor. The constructor will create a new file if it's not present.
	bool Open   (const TCHAR* FilePath, bool create = true);
	FileManager (const TCHAR* FilePath);

	// Closes the file stream.
	bool Close   ();
	~FileManager ();

	// Retrieves the current cursor position in the file stream, counting from the begining of the file.
	// If the function has executed successfully, the new cursor position is returned, otherwise, BADVALUE is returned.
	long long GetCursorPos ();
	long long GetCursorPos (long long& Offset);

	// Sets the current cursor position in the file stream.
	// If a negative value is given, the offset is calculated from the end of the file.
	// If the absolute value of the offset is larger than the file size, offset is set to either the end or the begining of
	// the file stream depending on wether a positive or a negative value was given.
	// If the function has executed successfully, the new cursor position is returned, otherwise, BADVALUE is returned.
	long long SetCursorPos (const long long Offset); 
	long long SetCursorPos (long long* Offset);

	// Retrieves the file size.
	// If the function has executed successfully, the file size is returned, otherwise, BADVALUE is returned.
	long long GetFileSize (); 
	long long GetFileSize (long long& FileSize);

	// Reads n bytes from the current stream position into the buffer, and updates the offset.
	// Returns the ammound of bytes red.
	// If end of the file has been reached, the return value is 0.
	long long Read (char* Buffer, const long long n); 
	long long Read (char* Buffer, long long* n);

	// Writes n bytes from the buffer into the file and updates the offset.
	// Returns the ammount of bytes writen.
	// If FM_EXTEND is passed amongst the flags, the file can get extended, if the end of the file has been reached and there
	// are still bytes to be writen. Otherwise the function returns.
	long long Write (char* Buffer, const long long n,  int Flags = 0);
	long long Write (char* Buffer, long long* n, int Flags = 0);

	bool Delete ();
	bool Move   (const TCHAR* szPath);
};