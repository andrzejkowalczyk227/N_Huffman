#pragma once

#include "stdafx.h"

#include <memory>
#include <string>
#include <stdio.h>
#include <vector>

class CFile
{
private:
	std::unique_ptr<FILE> uFile;
	bool opened;

	void close();

public:

	CFile() : opened(false) {};
	CFile operator=(const CFile&) = delete;
	CFile(const CFile&) = delete;

	void open(const std::string name, const std::string mode);

	// writing
	template <typename T, size_t N> // array
	void write(const T (&arr)[N]);

	template <typename T>
	void write(const T & element); // single element

	// reading
	template <typename T, size_t N> // array
	void read(T* arr);

	template <typename T>
	T read(); // single element

	~CFile();
};

template<typename T, size_t N>
void CFile::write(const T(&arr)[N])
{
	if (!opened)
	{
		throw std::exception("File has not been opened");
	}

	fwrite(arr, sizeof(T), N, uFile.get());
	if (ferror(uFile.get()))
	{
		throw std::exception("Writing array of elements to file failed");
	}
}

template<typename T>
void CFile::write(const T & element)
{
	if (!opened)
	{
		throw std::exception("File has not been opened");
	}

	fwrite(&element, sizeof(element), 1, uFile.get());
	if (ferror(uFile.get()))
	{
		throw std::exception("Writing element to file failed");
	}
}

template<typename T, size_t N>
void CFile::read(T* arr)
{
	if (!opened)
	{
		throw std::exception("File has not been opened");
	}

	fread(arr, sizeof(T), N, uFile.get());
	if (ferror(uFile.get()))
	{
		throw std::exception("Reading array of elements from file failed");
	}
}

template<typename T>
T CFile::read()
{
	if (!opened)
	{
		throw std::exception("File has not been opened");
	}

	T element;
	fread(&element, sizeof(T), 1, uFile.get());
	if (ferror(uFile.get()))
	{
		throw std::exception("Reading element from file failed");
	}
	return element;
}
