#pragma once
#include <string>
#include <math.h>
#include "PriorityHeap.h"
#include "HuffmanFile.h"

struct HelpFunctions
{
	static std::vector<bool> intToBit(int value, int len)
	{
		std::vector<bool> bitRep(len);
		for (int i = len-1; i >= 0; i--) // reverse
		{
			int p = pow(2, i);
			if (value - p >= 0)
			{
				bitRep[i] = 1;
				value -= p;
			}
		}
		return bitRep;
	}

	static bool compareVec(std::vector<bool> & vec1, std::vector<bool> & vec2)
	{
		for (int i = 0; i < vec1.size(); i++)
		{
			if (vec1[i] != vec2[i])
				return false;
		}
		return true;
	}
};

class Huffman
{
private:
	class HuffmanNode
	{
	public:
		char c;
		int ratio;
		bool dummy;

		std::vector<std::vector<bool>> finalCode; // final work
		std::vector<bool> canonicalCode;
		int fcLen;

		// tree structure
		std::vector<std::shared_ptr<HuffmanNode>> children;
		HuffmanNode* parent;
		std::vector<bool> codeParent;

		HuffmanNode(char c, int ratio) : c(c), ratio(ratio), dummy(false), parent(nullptr) {};
		void addChild(std::shared_ptr<HuffmanNode> child, std::vector<std::vector<bool>> & codesPatt);
	};

	int n;
	int numOfBits;
	bool withDummies;

	PriorityHeap< std::shared_ptr<HuffmanNode> > pHeap;
	std::shared_ptr<HuffmanNode> root;
	std::vector<std::shared_ptr<HuffmanNode>> nodes;
	std::vector<std::vector<bool>> codesPatt;

	std::string text;

	std::vector<bool> encodedText;
	std::vector<bool> encodedTextCanonical;
	std::vector<bool> encodedTree;

	std::string decodedText;
	std::string decodedTextCanonical;

	void displayBits(std::vector<bool> & bits);
	void createNodesFromLoadedCodeBook(std::vector<HuffmanFile::CodeBookNode> & cbook);
public:
	Huffman(int n, bool withDummies) : n(n), withDummies(withDummies) {};
	void makeNodesAndIntoHeap(std::string & str);
	void buildTree();
	std::vector<std::vector<bool>> Huffman::buildCodesPattern(int len);
	void buildFinalCodesNodes();
	void encodeText();
	void encodeTextCanonical();
	void encodeTree() {};
	void decodeText();
	void decodeTextCanonical();

	void displayNodes();
	void displayNodesCanonical();
	void displayEncodedText();
	void displayEncodedTextCanonical();
	void displayDecodedTextCanonical();
	void displayStats();

	void createCanonicalCodeBook();
	void saveToFile(const std::string & name);
	void loadFromFile(const std::string & fName);
};