#include <Windows.h>

#include "FileManager.h"

FileManager::FileManager()
{
	this->SetLastError(ERR_SUCCESS);
}

bool FileManager::Open(const TCHAR* szFile, bool create)
{
	this->SetLastError(ERR_SUCCESS);

	if (this->IsOpen())
	{
		this->SetCursorPos(ERR_ISOPEN);
		return false;
	}

	if (create)
	{
		std::ofstream createdFile (szFile, std::ios::app);
		if (!createdFile.is_open() || !createdFile.good())
		{
			this->SetLastError(ERR_FAILOPEN);
			return false;
		}

		createdFile.close();
	}

	assert(szFile);

	this->FilePath = szFile;
	this->File.open(szFile, std::ios::binary | std::ios::in | std::ios::out);

	if (!this->IsOpen())
	{
		assert(0);

		this->SetLastError(ERR_FAILOPEN);
		return false;
	}

	return true;
}

FileManager::FileManager(const TCHAR* szFile)
{
	this->SetLastError(ERR_SUCCESS);
	this->Open(szFile);
}

bool FileManager::Close()
{
	this->SetLastError(ERR_SUCCESS);

	if (this->IsOpen()) this->File.close();

	this->FilePath = nullptr;
	return !this->IsOpen();
}

FileManager::~FileManager()
{
	this->SetLastError(ERR_SUCCESS);
	this->Close();
}


void FileManager::SetLastError(int errCode)
{
	this->lastErr = errCode;
}

int  FileManager::GetLastError()
{
	return this->lastErr;
}

int  FileManager::GetLastError(int& err)
{
	err = this->lastErr;
	return err;
}


long long FileManager::GetCursorPos()
{
	this->SetLastError(ERR_SUCCESS);

	if (!this->IsOpen())
	{
		assert(0);

		this->SetLastError(ERR_NOOPEN);
		return BADVALUE;
	}

	long long Offset = max(static_cast<long long>(this->File.tellg()), static_cast<long long>(this->File.tellp()));
	this->SetCursorPos(Offset);

	long long FileSize = this->GetFileSize();

	if (Offset < 0 || Offset > FileSize)
	{
		Offset = FileSize;
	}

	return Offset;
}

long long FileManager::GetCursorPos(long long& Offset)
{
	this->SetLastError(ERR_SUCCESS);

	Offset = this->GetCursorPos();
	return Offset;
}

long long FileManager::SetCursorPos(const long long Offset)
{
	this->SetLastError(ERR_SUCCESS);

	if (!this->IsOpen())
	{
		assert(0);

		this->SetLastError(ERR_NOOPEN);
		return BADVALUE;
	}

	long long FileSize = this->GetFileSize();
	if (FileSize == BADVALUE) return BADVALUE;
	
	long long newOffset = Offset;

	if (newOffset < 0)
	{
		newOffset = FileSize - newOffset;
		if (newOffset < 0) newOffset = 0;
	}
	else if (newOffset > FileSize) newOffset = FileSize;
	
	this->File.seekg(newOffset);
	this->File.seekp(newOffset);
	return newOffset;
}

long long FileManager::SetCursorPos(long long* Offset)
{
	if (Offset == nullptr)
	{
		assert(0);

		this->SetLastError(ERR_BADPTR);
		return BADVALUE;
	}

	*Offset = SetCursorPos((const long long)*Offset);
	return *Offset;

	return BADVALUE;
}


long long FileManager::GetFileSize()
{
	this->SetLastError(ERR_SUCCESS);

	if (!this->IsOpen())
	{
		assert(0);
		this->SetLastError(ERR_NOOPEN);
		return BADVALUE;
	}

	long long oCursor = static_cast<long long>(this->File.tellg());

	this->File.seekg(0, std::ios::end);
	long long FileSize = static_cast<long long>(this->File.tellg());
	this->File.seekg(static_cast<std::streamoff>(oCursor));

	return FileSize;
}

long long FileManager::GetFileSize(long long& FileSize)
{
	this->SetLastError(ERR_SUCCESS);

	FileSize = this->GetFileSize();
	return FileSize;
}


long long FileManager::Read(char* buffer, const long long n)
{
	this->SetLastError(ERR_SUCCESS);

	if (buffer == nullptr || n <= 0)
	{
		assert(buffer);
		assert(n > 0);

		this->SetLastError(ERR_BADPTR);
		return BADVALUE;
	}

	if (!this->IsOpen())
	{
		assert(0);

		this->SetLastError(ERR_NOOPEN);
		return BADVALUE;
	}
	this->File.read(buffer, n);

	this->File.clear();
	return this->File.gcount();
}

long long FileManager::Read(char* buffer, long long* n)
{
	this->SetLastError(ERR_SUCCESS);

	if (buffer == nullptr || n == nullptr)
	{
		assert(buffer);
		assert(n);

		this->SetLastError(ERR_BADPTR);
		return BADVALUE;
	}

	if (*n <= 0)
	{
		assert(*n > 0);

		this->SetLastError(ERR_BADOFFS);
		return BADVALUE;
	}
	
	*n = this->Read(buffer, (const long long)*n);
	return *n;
}

long long FileManager::Write(char* buffer, const long long n, int flags)
{
	this->SetLastError(ERR_SUCCESS);

	if (buffer == nullptr || n <= 0)
	{
		assert(buffer);
		assert(n > 0);

		this->SetLastError(ERR_BADPTR);
		return BADVALUE;
	}

	if (!this->IsOpen())
	{
		assert(0);

		this->SetLastError(ERR_NOOPEN);
		return BADVALUE;
	}

	long long Count = n;

	long long oCursor = this->GetCursorPos();
	if (!(flags & FM_EXTEND))
	{
		if (oCursor + Count >= this->GetFileSize())
			Count = this->GetFileSize() - oCursor;

		if (Count <= 0)
		{
			this->SetLastError(ERR_CONFUSED);
			return BADVALUE;
		}
	}
	
	this->File.write(buffer, Count);
	this->File.clear();

	return this->GetCursorPos() - oCursor;
}

long long FileManager::Write(char* buffer, long long* n, int flags)
{
	this->SetLastError(ERR_SUCCESS);

	if (buffer == nullptr || n == nullptr)
	{
		assert(buffer);
		assert(n);

		this->SetLastError(ERR_BADPTR);
		return BADVALUE;
	}

	if (*n >= 0)
	{
		assert(n > 0);

		this->SetLastError(ERR_BADOFFS);
		return BADVALUE;
	}

	*n = this->Write(buffer, (const long long)*n, flags);
	return *n;
}

bool FileManager::Move(const TCHAR* szPath)
{
	this->SetLastError(ERR_SUCCESS);
	bool open = this->IsOpen();
	long long oCursor = 0;
	const TCHAR* filePath = this->FilePath;

	if (open)
	{
		oCursor = this->GetCursorPos();
		if (!this->Close()) return false;
	}

	bool ret = (rename(filePath, szPath) == 0);
	
	if (open)
	{
		ret &= (this->Open(szPath));
		ret &= (this->GetLastError() == 0);
		ret &= (this->SetCursorPos(oCursor) == oCursor);
		ret &= (this->GetLastError() == 0);
	}

	return ret;
}

bool FileManager::Delete()
{
	this->SetLastError(ERR_SUCCESS);
	const TCHAR* szFile = this->FilePath;

	if (!this->Close()) return false;

	return (remove(szFile) == 0);
}

bool FileManager::IsOpen()
{
	return this->File.is_open();
}