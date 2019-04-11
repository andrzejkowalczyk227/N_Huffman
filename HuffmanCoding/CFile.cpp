#include "stdafx.h"

#include "CFile.h"


void CFile::close()
{
	fclose(uFile.get());
	if (ferror(uFile.get()))
	{
		throw std::exception("Closing file failed");
	}
}

void CFile::open(const std::string name, const std::string mode)
{
	if (opened)
	{
		close();
		opened = false;
	}

	FILE* file = fopen(name.c_str(), mode.c_str());
	if (file == NULL)
	{
		throw std::exception("Opening file failed : FILE==NULL");
	}
	opened = true;
	uFile = std::unique_ptr<FILE>(file);
}

CFile::~CFile()
{
	if (opened)
	{
		close();
	}
}
