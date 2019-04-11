
#include "stdafx.h"
#include "Huffman.h"
#include <algorithm>
#include <iostream>

void Huffman::displayBits(std::vector<bool>& bits)
{
	for (int k = 0; k<bits.size(); k++) // for each bit
	{
		if (bits[k])
			std::cout << "1";
		else
			std::cout << "0";
	}
}

void Huffman::createNodesFromLoadedCodeBook(std::vector<HuffmanFile::CodeBookNode>& cbook)
{
	nodes.clear();

	HelpFunctions hf;
	unsigned int byte = 0;

	for (int i = 0; i < cbook.size(); i++)
	{
		std::shared_ptr<HuffmanNode> newNode = std::make_shared<HuffmanNode>(cbook[i].c,cbook[i].cLen);
		auto code = hf.intToBit(byte, cbook[i].cLen);
		std::reverse(code.begin(), code.end());
		displayBits(code);
		std::cout << std::endl;
		newNode->canonicalCode = code;
		nodes.push_back(newNode);

		if (i == cbook.size() - 1)
			break;

		byte++;
		byte <<= (cbook[i + 1].cLen - cbook[i].cLen);
		unsigned int mask = 0;
		mask = ~mask;
		mask <<= cbook[i + 1].cLen;
		byte &= ~mask;
	}
}

void Huffman::makeNodesAndIntoHeap(std::string & str)
{
	text = str;
	for (int i = 0; i < str.length(); i++)
	{
		char cStr = str.at(i);

		int j = 0;
		while (j<nodes.size())
		{
			if (nodes[j]->c == cStr)
			{
				break;
			}
			j++;
		}
		if (j < nodes.size())
		{
			nodes[j]->ratio++;
		}
		else
		{
			nodes.push_back(std::make_shared<HuffmanNode>(cStr,1));
		}
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		pHeap.insert(nodes[i],nodes[i]->ratio);
	}
	int pom = 10;
}

void Huffman::buildTree()
{
	// inserting dummy node to heap
	// could also just do the first move as n'=n-1
	/*if (n>2 && pHeap.getSize()%(n-1) == 0)
	{
		std::shared_ptr<HuffmanNode> dummy = std::make_shared<HuffmanNode>('\0', 0);
		dummy->dummy = true;
		pHeap.insert(dummy,0);
	}*/

	if (withDummies && n>2) // checking if dummy nodes are needed
	{
		int N = pHeap.getSize(); // amount of alphabet nodes
		int A = N % (n - 1);
		int dAmount;
		if (A == 1)
		{
			dAmount = 0;
		}
		else if (A == 0)
		{
			dAmount = 1;
		}
		else // A>1 && A<n-1
		{
			dAmount = n - A;
		}
		for (int i = 0; i < dAmount; i++)
		{
			auto dummy = std::make_shared<HuffmanNode>('\0', 0);
			dummy->dummy = true;
			pHeap.insert(dummy, 0);
		}
	}

	codesPatt = buildCodesPattern(n);

	while (pHeap.getSize() != 1)
	{
		std::shared_ptr<HuffmanNode> sumNode = std::make_shared<HuffmanNode>('\0',0);
		sumNode->dummy = true;
		if (!withDummies && pHeap.getSize() < n) // case with no dummies
		{
			int actSize = pHeap.getSize();
			auto codesPattSpecial = buildCodesPattern(pHeap.getSize());
			for (int i = 0; i < actSize; i++)
			{
				auto minNode = pHeap.getMin();
				sumNode->ratio += minNode->ratio;
				sumNode->addChild(minNode, codesPattSpecial);
			}
			pHeap.insert(sumNode, sumNode->ratio);
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				auto minNode = pHeap.getMin();
				sumNode->ratio += minNode->ratio;
				sumNode->addChild(minNode, codesPatt);
			}
			pHeap.insert(sumNode, sumNode->ratio);
		}
	}

	root = pHeap.getMin();
}

std::vector<std::vector<bool>> Huffman::buildCodesPattern(int len)
{
	numOfBits = log2((float)len - 1) + 1.0;
	HelpFunctions hf;

	std::vector<std::vector<bool>> pattern;
	for (int i = 0; i < len; i++)
	{
		pattern.push_back(hf.intToBit(i, numOfBits));
	}
	return pattern;
}

void Huffman::buildFinalCodesNodes()
{
	int s = nodes.size();
	for (int i = 0; i < s; i++) // for each node
	{
		auto actNode = nodes[i].get();
		while (actNode->parent!=nullptr)
		{
			nodes[i]->finalCode.push_back(actNode->codeParent);
			actNode = actNode->parent;
		}
		std::reverse(nodes[i]->finalCode.begin(), nodes[i]->finalCode.end());
	}
}

void Huffman::encodeText()
{
	for (int i = 0; i < text.length(); i++)
	{
		char cStr = text.at(i);

		int s = nodes.size();
		for (int w = 0; w < s; w++) // for each node
		{
			auto actNode = nodes[w];
			
			if (actNode->c == cStr)
			{
				auto finCode = actNode->finalCode;
				int cs = finCode.size();
				for (int j = 0; j < cs; j++) // for each fragment
				{
					auto codeFrag = finCode[j];
					int fs = codeFrag.size();
					for (int k = 0; k<fs; k++) // for each bit
					{
						encodedText.push_back(codeFrag[k]);
					}
				}
			}
		}
	}
}

void Huffman::encodeTextCanonical()
{
	for (int i = 0; i < text.length(); i++)
	{
		char cStr = text.at(i);

		int s = nodes.size();
		for (int w = 0; w < s; w++) // for each node
		{
			auto actNode = nodes[w];

			if (actNode->c == cStr)
			{
				auto code = nodes[w]->canonicalCode;
				for (int r = 0; r < code.size(); r++)
					encodedTextCanonical.push_back(code[r]);
				break;
			}
		}
	}
}

void Huffman::decodeText()
{
	decodedText = "";
	HelpFunctions hf;

	int s = encodedText.size();

	int i = 0;
	while(i<s)
	{
		std::shared_ptr<HuffmanNode> actNode = root;
		while (actNode->dummy)
		{
			int j = 0;
			int step;
			while(j < actNode->children.size())
			{
				auto actChild = actNode->children[j];
				step = actNode->children[j]->codeParent.size();
				bool same = true;
				int pom = 10;
				for (int k = 0; k < actChild->codeParent.size(); k++) 
				{
					if (actChild->codeParent[k] != encodedText[i + k])
						same = false;
				}
				if (same)
					break;
				j++;
			}
			actNode = actNode->children[j];
			i += step;
		}
		decodedText += actNode->c;
	}
}

void Huffman::decodeTextCanonical()
{
	int i = 0;
	int len = encodedTextCanonical.size();
	decodedTextCanonical = "";
	HelpFunctions hf;

	while (i < len)
	{
		int j = 0;
		std::vector<bool> partCode;
		bool found = false;
		while (i + j < len && !found)
		{
			partCode.push_back(encodedTextCanonical[i+j]);
			for (auto node : nodes)
			{
				if (partCode.size()==node->canonicalCode.size() && hf.compareVec(partCode, node->canonicalCode))
				{
					found = true;
					decodedTextCanonical += node->c;
					i = i + j;
				}
			}
			j++;
		}
		i++;
	}

	int pom = 10;
}

void Huffman::displayNodes()
{
	int s = nodes.size();
	for (int i = 0; i < s; i++) // for each node
	{
		auto actNode = nodes[i];
		std::cout << actNode->c << " - " << actNode->ratio << " - ";

		auto finCode = nodes[i]->finalCode;
		int cs = finCode.size();
		for (int j = 0; j < cs; j++) // for each fragment
		{
			displayBits(finCode[j]);
		}
		std::cout << std::endl;
	}
}

void Huffman::displayNodesCanonical()
{
	for (auto node : nodes)
	{
		std::cout << node->c << " - ";
		displayBits(node->canonicalCode);
		std::cout << std::endl;
	}
}

void Huffman::displayEncodedText()
{
	displayBits(encodedText);
}

void Huffman::displayEncodedTextCanonical()
{
	displayBits(encodedTextCanonical);
}

void Huffman::displayDecodedTextCanonical()
{
	std::cout << decodedTextCanonical << std::endl;
}

void Huffman::displayStats()
{
	int sizeSum = 0;
	for (auto node : nodes)
	{
		for(auto part : node->finalCode)
			sizeSum += part.size();
	}
	std::cout << "STATYSTYKI " << n << "-KROTNE KODOWANIE HUFFMANA:" << std::endl;
	if(withDummies)
		std::cout << "Z UZUPELNIENIEM DUMMIES" << std::endl;
	else
		std::cout << "BEZ UZUPELNIENIA" << std::endl;
	std::cout << "Lacznie znakow w tekscie: " << text.size() << " | Roznych znakow: " << nodes.size() << std::endl; 
	std::cout << "Zapisanych na: " << encodedText.size() << " bitach" << " | Przy stalym: " << 8*text.size() << std::endl;
	std::cout << "Srednia bitow na znak: " << (float)sizeSum / (float)nodes.size() << std::endl;
}

void Huffman::createCanonicalCodeBook()
{
	int am = nodes.size();
	PriorityHeap<std::shared_ptr<HuffmanNode>> phLen;

	for (auto node : nodes)
	{
		int lenSum = 0;

		for (std::vector<bool> part : node->finalCode)
		{
			lenSum += part.size();
		}
		node->fcLen = lenSum;
		phLen.insert(node,lenSum);
	}
	
	nodes.clear();

	for(int i = 0; i<am; i++)
		nodes.push_back(phLen.getMin());

	HelpFunctions hf;
	unsigned int bit = 0;
	
	for (int i = 0; i < am; i++)
	{
		auto bCode = hf.intToBit(bit, nodes[i]->fcLen);
		std::reverse(bCode.begin(), bCode.end());
		nodes[i]->canonicalCode = bCode;
		if (i == am - 1)
			break;
		//bit &;
		bit++;
		bit <<= (nodes[i + 1]->fcLen - nodes[i]->fcLen);
		unsigned int mask = 0;
		mask = ~mask;
		mask <<= nodes[i + 1]->fcLen;
		bit &= ~mask;
	}
}

void Huffman::saveToFile(const std::string & name)
{
	std::vector<HuffmanFile::CodeBookNode> cbook;
	for (auto node : nodes)
	{
		HuffmanFile::CodeBookNode cb {node->c, node->fcLen};
		cbook.push_back(cb);
	}
	HuffmanFile hf;
	hf.saveToFile(name,n,cbook,encodedTextCanonical);
}

void Huffman::loadFromFile(const std::string & fName)
{
	HuffmanFile hf;
	std::vector<HuffmanFile::CodeBookNode> cbook;
	encodedTextCanonical.clear();
	n = hf.loadFromFile(fName,cbook,encodedTextCanonical);
	
	createNodesFromLoadedCodeBook(cbook);

	std::cout << "Wczytano kodowanie " << n << "-krotne" << std::endl;
}

void Huffman::HuffmanNode::addChild(std::shared_ptr<HuffmanNode> child, std::vector<std::vector<bool>> & codesPatt)
{
	children.push_back(child);
	child->parent = this;
	child->codeParent = codesPatt[children.size()-1];
}
