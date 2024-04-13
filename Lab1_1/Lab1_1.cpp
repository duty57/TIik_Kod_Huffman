#include <iostream>
#include<string>
#include<fstream>
#include <queue>
#include<vector>
using namespace std;

vector<int> chars; 
vector<int> freq;


struct TreeNode {
	int data;
	int freq;

	TreeNode* left, * right, *parent;
};
   
//creating balanced huffman tree

struct CompareNodes {
	bool operator()(TreeNode* const& a, TreeNode* const& b) {
		return a->freq > b->freq; // Min-heap based on frequency
	}
};

void ileBajtow(string file){

	int ileBajtow = 0;

	ifstream plik(file, ios::binary);
	if (plik.is_open()) {
		plik.seekg(ios::beg, ios::end);
		int rozmiar = plik.tellg();
		ileBajtow = rozmiar;
		string namebase = file.substr(0, file.find("."));
		ofstream plik2(namebase + ".ileBajtow");
		if (plik2.is_open()) {
			plik2<<rozmiar;
			plik2.close();
		}
		else {
			cout<<"Nie udalo sie otworzyc pliku\n";
		}
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout<<"Z pliku "<<file<<" odczytano "<<ileBajtow<<" bajtow\n";
	cout<<"Zapisano do pliku "<<file.substr(0, file.length()-4)<<".ileBajtow\n";


}

void model(string file) {
	ifstream plik(file, ios::binary);
	 
	if (plik.is_open()) {
		string namebase = file.substr(0, file.find("."));
		

		//counting chars
		char c;
		while (plik.get(c)) {
			int i = 0;
			for (i = 0; i < chars.size(); i++) {
				if (chars[i] == c) {
					freq[i]++;
					break;
				}
			}
			if (i == chars.size()) {
				chars.push_back(c);
				freq.push_back(1);
			}
		}
		//writing to file
		ofstream plik(namebase + ".model");
		if (plik.is_open()) {
			for (int i = 0; i < chars.size(); i++) {
				plik<<chars[i]<<" "<<freq[i]<<endl;
			}
			plik.close();
		}
		else {
			cout<<"Nie udalo sie otworzyc pliku\n";
		}
	
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout << "Model nieposortowany zrodla\n";
	for (int i = 0; i < chars.size(); i++) {
		cout << i << " indeks tablicy: " << chars[i] << " liczebnosc " << freq[i] << endl;
	}
	cout << "Model Shannona nieposortowany zapisano do pliku " << file.substr(0, file.length() - 4) << ".model\n";
}

void modelSort(string file) {
	
	for (int i = 0; i < chars.size(); i++) {
		for (int j = 0; j < chars.size() - 1; j++) {
			if (freq[j] < freq[j + 1]) {
				int temp = freq[j];
				freq[j] = freq[j+1];
				freq[j+1] = temp;
				int temp2 = chars[j];
				chars[j] = chars[j+1];
				chars[j+1] = temp2;
			}
		}
	}
	string namebase = file.substr(0, file.find("."));

	ofstream plik(namebase + ".modelSort");
	if (plik.is_open()) {
		for (int i = 0; i < chars.size(); i++) {
			plik<<chars[i]<<" "<<freq[i]<<endl;
		}
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout << "Model posortowany zrodla\n";
	for (int i = 0; i < chars.size(); i++) {
		cout << i << " indeks tablicy: " << chars[i] << " liczebnosc " << freq[i] << endl;
	}
	cout << "Model Shannona posortowany zapisano do pliku " << file.substr(0, file.length() - 4) << ".modelSort\n";

}


void createHuffmanTree(TreeNode*& root) {
	// Create a priority queue (min-heap) of TreeNodes
	priority_queue<TreeNode*, vector<TreeNode*>, CompareNodes> minHeap;
	int k = 0; // iterator for the chars and freq vectors
	// Create leaf nodes for each character with its frequency
	for (int i = 0; i < chars.size(); i++) {
		TreeNode* node = new TreeNode;
		node->data = chars[i];
		node->freq = freq[i];
		node->left = node->right = node->parent = nullptr;
		minHeap.push(node);
	}

	// Build the Huffman tree from the min-heap
	while (minHeap.size() > 1) {
		// Extract the two nodes with the lowest frequencies
		TreeNode* left = minHeap.top();
		minHeap.pop();
		TreeNode* right = minHeap.top();
		minHeap.pop();

		// Create a new parent node with the combined frequency
		TreeNode* parent = new TreeNode;
		parent->data = 256 + k; // Not a leaf node, data is irrelevant
		parent->freq = left->freq + right->freq;
		parent->left = left;
		parent->right = right;
		left->parent = parent;
		right->parent = parent;
		k++;
		// Push the new parent node back into the min-heap
		minHeap.push(parent);
	}

	// The last remaining node in the min-heap is the root of the Huffman tree
	root = minHeap.top();
}



void printTree(TreeNode* root,vector<vector<int>>& treeStruct, int level) {
	if (root != NULL && root->left != NULL && root->right != NULL) {
		treeStruct.push_back({ root->data, root->left->data, root->right->data });
		printTree(root->right,treeStruct, level + 1);
		for (int i = 0; i < level; i++) {
			cout << "   ";
		}
		//cout << root->data << " " << root->freq << endl;
		printTree(root->left, treeStruct, level + 1);
	}
}

void saveTree(TreeNode* root, const vector<vector<int>>& treeStruct ,string file ) {
	string namebase = file.substr(0, file.find("."));

	cout<<"Drzewo Huffmana"<<endl;

	ofstream plik(namebase + ".drzewo");
	for (int i = 0; i < treeStruct.size(); i++) {
		for (int j = 0; j < treeStruct[i].size(); j++) {
			plik<<treeStruct[i][j]<<" ";
			cout << treeStruct[i][j] << " ";;

		}
		plik<<endl;
		cout << endl;
	}
	

	plik.close();
}

void deleteTree(TreeNode* root) {
	if (root != NULL) {
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}


void createHuffmanCode(TreeNode* root, string code, vector<pair<int, string>>& codes) {
	if (root == NULL) {
		return;
	}

	if (root->left == NULL && root->right == NULL) {
		codes.push_back({ root->data, code });
	}

	createHuffmanCode(root->left, code + "0", codes);
	createHuffmanCode(root->right, code + "1", codes);
}

void saveHuffmanCode(const vector<pair<int, string>>& codes, string file) {
	string namebase = file.substr(0, file.find("."));



	vector<pair<pair<int, string>, int>> coding;//char, code, freq
	for (int i = 0; i < codes.size(); i++) {
		for (int j = 0; j < freq.size(); j++) {
			if (codes[i].first == chars[j]) {
				coding.push_back({ {codes[i].first, codes[i].second}, freq[j] });
				break;
			}
		}
	}

	cout<<"Tabela kodu Huffmana"<<endl;

	ofstream plik(namebase + ".TabelaKodu");
	for (int i = 0; i < codes.size(); i++) {
		plik<<codes[i].first<<" "<<codes[i].second<<" "<< coding[i].second << endl;
		cout<<codes[i].first<<" "<<codes[i].second<<" "<< coding[i].second << endl;
	}
	plik.close();
}

void codeString(string file, const vector<pair<int, string>>& codes, string & codedString) {
	ifstream plik(file, ios::binary);
	if (plik.is_open()) {
		string namebase = file.substr(0, file.find("."));
		ofstream plik2(namebase + ".kod");
		if (plik2.is_open()) {
			while (!plik.eof()) {
				char c;
				plik.get(c);
				for (int i = 0; i < codes.size(); i++) {
					if (codes[i].first == c) {
						codedString += codes[i].second;
						break;
					}
				}
			}
			plik2 << codedString;
		}
		else {
			cout<<"Nie udalo sie otworzyc pliku\n";
		}
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}
}

//binary string to char
//then compress file
//and decompress file
void convertStringToChar(string codedString, string file) {
	string namebase = file.substr(0, file.find("."));
	ofstream plik(namebase + ".compressed", ios::binary);

	if (plik.is_open()) {
		string byte;
		for (int i = 0; i < codedString.length(); i++) {
			byte += codedString[i];

			if (byte.length() == 8 || i == codedString.length() - 1) {
				// Convert binary string 'byte' to a char and write to file
				char c = 0;
				for (int j = 0; j < byte.length(); j++) {
					if (byte[j] == '1') {
						c |= (1 << (7 - j));
					}
				}
				plik.put(c);
				byte.clear(); // Clear 'byte' for next set of bits
			}
		}

		cout << "File compressed to " << namebase << ".compressed and occupies " << codedString.length() << " bits." << endl;
		plik.close();
	}
	else {
		cout << "Failed to open file for writing." << endl;
	}
}

//count freq of chars and decrement it value
string convertFileToBinary(string file) {
	string namebase = file.substr(0, file.find("."));
	ifstream plik(namebase + ".compressed", ios::binary);
	string binaryString;

	if (plik.is_open()) {
		char c;
		while (plik.get(c)) {
			for (int i = 7; i >= 0; i--) {
				binaryString += ((c >> i) & 1) + '0';
			}
		}
		plik.close();
	}
	else {
		cout << "Failed to open file for reading." << endl;
	}

	return binaryString;
}

//read freq(last element of string) from TabelaKodu and save it to vector
void readFreq(string file, vector<pair<pair<int, string>, int>>& coding) {
	string namebase = file.substr(0, file.find("."));
	ifstream plik(namebase + ".TabelaKodu");
	if (plik.is_open()) {
		while (!plik.eof()) {
			int c;
			string code;
			int _freq;
			plik >> c >> code >> _freq;
			coding.push_back({ {c, code}, _freq });

		}
		plik.close();
	}
	else {
		cout << "Failed to open file for reading." << endl;
	}
}


void decodeString(TreeNode* root, string file, const vector<pair<int, string>>& codes, vector<pair<pair<int, string>, int>>& coding) {


	string codedString = convertFileToBinary(file);

	string namebase = file.substr(0, file.find("."));
	ofstream plik(namebase + ".recovery");

	if (plik.is_open()) {
		TreeNode* current = root;
		for (char bit : codedString) {//and check freq
			if (bit == '0') {
				current = current->left;
			}
			else if (bit == '1') {
				current = current->right;
			}

			if (current->left == nullptr && current->right == nullptr) {
				int k = 0; //show position of char in coding
				// Leaf node reached, write character to output file
				for (int i = 0; i < coding.size(); i++) {
					if (coding[i].first.first == current->data) {
						k = i;
						break;
					}
				}
				/*cout << "freq: " << endl;
				cout << coding[k].first.first << " " << coding[k].first.second << " " << coding[k].second << endl;
				*/
				if (coding[k].second > 0) {
					coding[k].second--;
					plik.put(current->data);
				}
				/*else {
					cout<<"Error: frequency of char is 0\n";
				}*/
				current = root; // Reset to the root for next character
			}
		}
		plik.close();
		cout << "Decoding complete. Result written to " << namebase << ".recovery" << endl;
	}
	else {
		cout << "Failed to open file for writing." << endl;
	}
}



int main(int argc, char* argv[])
{
	string file;
	TreeNode *root = nullptr;
	vector<vector<int>> treeStruct;
	vector<pair<int, string>> codes;
	string codedString;
	vector<pair<pair<int, string>, int>> coding;//char, code, freq

	if (argc == 2) {
		file = argv[1];
	}
	else {
		cout << " " << argc << endl;
		cout<<"Dopuszczalna liczba argumentow to 1\n";
		return 1;
	}

	ileBajtow(file);
	model(file);
	modelSort(file);
	createHuffmanTree(root);
	printTree(root,treeStruct, 0);
	saveTree(root, treeStruct, file);
	createHuffmanCode(root, "", codes);
	saveHuffmanCode(codes, file);
	codeString(file, codes, codedString);
	convertStringToChar(codedString, file);
	readFreq(file, coding);
	decodeString(root, file, codes, coding);

	return 0;
}

