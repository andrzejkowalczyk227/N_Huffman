#include "stdafx.h"
#include "HuffmanFile.h"
#include <iostream>
uint8_t HuffmanFile::poluteByte(const std::vector<bool>& encodedText, int i)
{
	uint8_t ui8 = 0;

	for (int j = 0; j < 8; j++)
	{
		int index =  i * 8 + j;
		if(index<encodedText.size()-1)
			ui8 |= encodedText[index] << j;
	}

	return ui8;
}

void HuffmanFile::saveToFile(const std::string & fName, int n, const std::vector<CodeBookNode>& cBook, const std::vector<bool>& encodedText)
{
	HuffmanHeader header = {n, cBook.size(), encodedText.size()};

	CFile cfile;
	cfile.open(fName,"wb");

	cfile.write<HuffmanHeader>(header);
	for (int i = 0; i < cBook.size(); i++)
	{
		cfile.write<CodeBookNode>(cBook[i]);
	}

	int bytesAmount = (float)encodedText.size() / 8.0;
	bytesAmount += 1;

	for (int i = 0; i < bytesAmount; i++)
	{
		auto byte = poluteByte(encodedText, i);
		cfile.write<uint8_t>(byte);
	}
}

int HuffmanFile::loadFromFile(const std::string & fName, std::vector<CodeBookNode>& cBook, std::vector<bool>& encodedText)
{
	HuffmanHeader header;

	CFile cfile;
	cfile.open(fName, "rb");

	header = cfile.read<HuffmanHeader>();

	for (int i = 0; i < header.codebookLen; i++)
	{
		cBook.push_back(cfile.read<CodeBookNode>());
	}

	int bytesAmount = (float)header.encTextLen / 8.0;
	bytesAmount++;

	for (int i = 0; i < bytesAmount; i++)
	{
		uint8_t byte = cfile.read<uint8_t>();

		for (int j = 0; j < 8; j++)
		{
			if (i*8+j < bytesAmount*8-1)
				encodedText.push_back( (byte >> j) & 1 );
		}
	}

	return header.n;
}
