#pragma once
#include "CFile.h"
class HuffmanFile
{
private:
	struct HuffmanHeader
	{
		unsigned int n;
		unsigned int codebookLen;
		unsigned int encTextLen;
	};

	uint8_t poluteByte(const std::vector<bool>& encodedText, int i);

public:
	struct CodeBookNode
	{
		char c;
		unsigned int cLen;
	};

	void saveToFile(const std::string & fName, int n, const std::vector<CodeBookNode> & cBook, const  std::vector<bool> & encodedText);
	int loadFromFile(const std::string & fName, std::vector<CodeBookNode>& cBook, std::vector<bool>& encodedText);
};