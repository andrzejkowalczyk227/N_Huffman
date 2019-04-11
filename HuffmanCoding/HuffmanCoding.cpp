// HuffmanCoding.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Huffman.h"
#include <iostream>

void test(std::string & str, int n, bool dummy, bool showencoded = false)
{
	std::cout << std::endl;
	Huffman hf(n, dummy);
	hf.makeNodesAndIntoHeap(str);
	hf.buildTree();
	hf.buildFinalCodesNodes();
	hf.encodeText();
	hf.decodeText();
	hf.displayStats();
	hf.displayNodes();
	if (showencoded)
	{
		std::cout << "Zakodowany tekst (standardowo): " << std::endl;
		hf.displayEncodedText();
	}

	hf.createCanonicalCodeBook();
	hf.encodeTextCanonical();
	std::cout << std::endl;
	hf.displayNodesCanonical();
	if (showencoded)
	{
		std::cout << "Zakodowany tekst (canonical): " << std::endl;
		hf.displayEncodedTextCanonical();
	}
	std::cout << "\n";
	std::cout << "\n====================================================================";
}

int main()
{
	std::string str;
	/*std::cout << "Podaj tekst do zakodowania ('1' - tekst przykladowy): ";
	std::getline(std::cin, str);
	std::cin.clear();
	std::cin.sync();
	if (str == "1")
	{*/
	str = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio cumque nihil impedit quo minus id quod maxime placeat facere possimus, omnis voluptas assumenda est, omnis dolor repellendus. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat";
	std::cout << str << std::endl;

	/*int n;
	std::cout << "Podaj n-kodowania: ";
	std::cin >> n;

	bool dummy;
	char ans;
	std::cout << "Z uzupelnieniem? [Y/N]: ";
	std::cin >> ans;

	if (ans == 'Y')
		dummy = true;
	else
		dummy = false;
	
	std::string filename;
	std::cout << "Podaj nazwe pliku: ";
	std::cin >> filename;
	
	std::cout << std::endl;*/

	test(str, 2, true);
	test(str, 2, false);
	test(str, 3, true);
	test(str, 3, false, true);
	test(str, 4, true);
	test(str, 4, false);
	test(str, 8, true);
	test(str, 8, false);
	test(str, 16, true);
	test(str, 16, false);
	test(str, 29, true);
	test(str, 29, false);
	test(str, 40, true);
	test(str, 40, false);

	system("pause");
}

