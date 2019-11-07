#include <iostream>
#include <fstream>
#include <cstring>

const int ZERO = 0;
const int MAX_SIZE = 257;
const int CHECK_SIZE = 13;
const char CHECK[CHECK_SIZE] = "ThisIsMyDict";

struct TNode{
	char *key;
	unsigned long long value;
	int height;
	TNode* left;
	TNode* right;
	TNode(char* k, unsigned long long v, int h) {
		int sz = strlen(k);
		key = new char[sz + 1];
		for (int i = 0; i <= sz; ++i){
			key[i] = k[i];
		}
		value = v;
		left = nullptr;
		right = nullptr;
		height = h;
	}
	~TNode(){
		delete[] key;
	}
};

void DoToLower(char *str) {
	int size = strlen(str);
	for (int i = 0; i < size; ++i) {
		str[i] = tolower(str[i]);
	}
}

int Max(int a, int b) {
	return a > b ? a : b;
}

int GetHeight(TNode* p) {
	return p ? p -> height : ZERO;
}

int GetBalance(TNode* p) {
	return GetHeight(p -> right) - GetHeight(p -> left);
}

void CulcHeight(TNode* p) { 
	p -> height = 1 + Max(GetHeight(p -> left), GetHeight(p -> right));
}

TNode* RightTurn(TNode* p) {
	TNode* tmp = p -> left;
	p -> left = tmp -> right;
	tmp -> right = p;
	CulcHeight(p); 
	CulcHeight(tmp);
	return tmp;
}

TNode* LeftTurn(TNode* p) {
	TNode* tmp = p -> right;
	p -> right = tmp -> left;
	tmp -> left = p;
	CulcHeight(p); 
	CulcHeight(tmp);
	return tmp;
}

TNode* DoBalance(TNode* p){
	CulcHeight(p);
	if (GetBalance(p) == 2) {
		if (GetBalance(p -> right) < ZERO) {	
			p -> right = RightTurn(p -> right);
		}
		return LeftTurn(p);
	}
	if (GetBalance(p) == -2) {
		if (GetBalance(p -> left) > ZERO) {
			p -> left = LeftTurn(p -> left);
		}
		return RightTurn(p);
	}
	return p;
}

TNode* Insert(TNode* p, char* key, unsigned long long value){
	if (p == nullptr) {
		p = new TNode(key, value, ZERO);
		if (p == nullptr) {
			std::cout << "ERROR: NOT ENOUGH MEMORY TO CREATE VERTEX\n";
		} else { 
			std::cout << "OK\n";
		}
		return p;
	}
	if (strcmp(key, p -> key) == ZERO) {
		std::cout << "Exist\n";
		return DoBalance(p);
	}
	if (strcmp(key, p -> key) > ZERO) {
		p -> left = Insert(p -> left, key, value);
	} else {
		p -> right = Insert(p -> right, key, value);
	}
	return DoBalance(p);
}

TNode* FindMin(TNode* p) {
	return p -> left ? FindMin(p -> left) : p;
}

TNode* FindSonOfNewRoot(TNode* p) { 
	if (p -> left == nullptr) {
		return p -> right;
	}
	p -> left = FindSonOfNewRoot(p -> left);
	return DoBalance(p);
}

TNode* Erase(TNode* p, char* key) {
	if (p == nullptr) {
		std::cout << "NoSuchWord\n";
		return nullptr;
	}
	if (strcmp(key, p -> key) > ZERO) { 
		p -> left = Erase(p -> left, key);
	} else if (strcmp(key, p -> key) < ZERO) {
		p -> right = Erase(p -> right, key);	
	} else { 
		std::cout << "OK\n";
		TNode* l = p -> left;
		TNode* r = p -> right;
		delete p;        
		if(r == nullptr){ 
			return l;
		}
		TNode* min = FindMin(r);
		min -> right = FindSonOfNewRoot(r);
		min -> left = l;
		return DoBalance(min);
	}
	return p;
}

void FindElement(TNode* p, char* key) {
	if (p == nullptr) {
		std::cout << "NoSuchWord\n";
		return;
	}
	if (strcmp(key, p -> key) == ZERO) {
		std::cout << "OK: ";
		std::cout << p -> value << '\n';
		return;
	}
	if (strcmp(key, p -> key) > ZERO) {
		FindElement(p -> left, key);
	}
	if (strcmp(key, p -> key) < ZERO) {
		FindElement(p -> right, key);
	}
}

void printt(TNode* p, int l) {
	if (p == nullptr) return;
	printt(p -> right, l + 2);
	for (int i = 0; i < l; ++i) {
		std::cout << "  ";
	}
	std::cout << p -> key << '\n';
	printt(p -> left, l + 2);
}

void DeleteTree(TNode* p) {
	if (p != nullptr){
		DeleteTree(p -> left);
		DeleteTree(p -> right);
		delete p;
		p = nullptr;
	}
}

bool WriteReminder(std::ofstream& file) {
	if (!file.write(CHECK, CHECK_SIZE)){
		return false;
	}
	if (file.fail()){
		return false;
	} else {
		return true;
	}
}

bool DoWriteFile(TNode* p, std::ofstream& file) {
	if (p == nullptr) return true;
	bool hasLeft = (p -> left != nullptr);
	bool hasRight = (p -> right != nullptr);
	int size = strlen(p -> key);
	file.write((char*) &(size), sizeof(int));
	file.write(p -> key, size); 
    file.write((char*) &(p -> value), sizeof(unsigned long long));
    file.write((char*) &(p -> height), sizeof(int));
    file.write((char*) &(hasLeft), sizeof(bool));
    file.write((char*) &(hasRight), sizeof(bool));
	if (hasLeft == true) {
		DoWriteFile(p -> left, file);
	}
	if (hasRight == true) {
		DoWriteFile(p -> right, file);
	}
	if (file.fail() == true){
		return false;
	} else {
		return true;
	}
}

bool ReminderExist(std::ifstream& file, int& isCorrect) {
	if (file.peek() == std::ifstream::traits_type::eof()){
		isCorrect = 0;
		return true;
	}
	char* c = nullptr;
	bool ok = true;
	int res = 0;
	c = new char[CHECK_SIZE] {0};
	if (c == nullptr) {
		return false;
	}
	if (!file.read(c, CHECK_SIZE)) {
		delete[] c;
		return false;
	}
	try {
		res = strcmp(c, CHECK);
	} catch(const std::bad_alloc&){
		delete[] c;
		return false;
	}
	if (res == ZERO) {
		ok = true;
	} else {
		ok = false;
	}
	delete[] c;
	return ok;
}

TNode* DoReadFile(std::ifstream& file, bool& isCorrect) {				
	TNode* p = nullptr;
	int size = 0;
	char* key = nullptr;
	unsigned long long value = 0;
	int height = 0;
	bool hasLeft = false;
	bool hasRight = false;
	if (!file.read((char*) &(size), sizeof(int))) {
		if (size == ZERO) {
			return nullptr;  
		}
		isCorrect = false;
		return nullptr;
	}
	key = new char[size + 1] {0};
	if (!key){
		isCorrect = false;
		return nullptr; 
	}
	try {
		file.read(key, size);
		file.read((char*) &(value), sizeof(unsigned long long));
		file.read((char*) &(height), sizeof(int));
		file.read((char*) &(hasLeft), sizeof(bool));
    	file.read((char*) &(hasRight), sizeof(bool));
	} catch (const std::bad_alloc &) {
		delete[] key;
        isCorrect = false;
		return nullptr;
    }	
    p = new TNode(key, value, height);
	if (!p){
        delete[] key;
		isCorrect = false;
		return nullptr; 
    }
	delete[] key;
	if (hasLeft == true) {
		p -> left = DoReadFile(file ,isCorrect);
	}
	if (hasRight == true) {
		p -> right = DoReadFile(file, isCorrect);
	}
	if (isCorrect == ZERO) {
		delete p;
	}
	return p;
}

int main(){
	std::ios::sync_with_stdio(false);
	std::ifstream loadFile;
	std::ofstream saveFile;
	unsigned long long value;
	char input[MAX_SIZE];
	TNode* tree = nullptr;
	TNode* tmpLoadTree = nullptr;
	while (std::cin >> input) {
		if (strcmp(input, "+") == ZERO) {
			std::cin >> input;
			std::cin >> value;
			DoToLower(input);
			tree = Insert(tree, input, value);
		} else if (strcmp(input, "-") == ZERO) {
			std::cin >> input;
			DoToLower(input);
			tree = Erase(tree, input);
		} else if (strcmp(input, "!") == ZERO) { 
			std::cin >> input;
			if (strcmp(input, "Save") == ZERO) {
				std::cin >> input;
				saveFile.open(input,std::ios::binary | std::ios::out | std::ios::trunc);
				if (saveFile.is_open() == false) {
                    std::cout << "ERROR: CANT OPEN FILE\n";
                    continue;
                }
				if (WriteReminder(saveFile) && DoWriteFile(tree, saveFile)){
					std::cout << "OK\n";
				} else {
					std::cout << "ERROR: BAD WRITE TO FILE\n";
				}
				saveFile.close();
			} else if (strcmp(input, "Load") == ZERO) { 
				std::cin >> input;
				loadFile.open(input,std::ios::binary |  std::ios::in);
				if (loadFile.is_open() == false) {
					std::cout << "ERROR: CANT OPEN FILE\n";
                    continue;
				}
				int correct = 1;
				bool ok = ReminderExist(loadFile, correct);
				if (correct == ZERO) {
					std::cout << "ERROR: ITS NOT MY DICTIONARY\n";
					loadFile.close();
					continue;
				}
				if (ok == false) {
					std::cout << "ERROR: INCORRECT DICTIONARY FOR MY PERFORMANCE\n";
					loadFile.close();
					continue;
				}
				bool isCorrect = true;
				tmpLoadTree = DoReadFile(loadFile, isCorrect);
				if (isCorrect == true) {
					std::cout << "OK\n";
					DeleteTree(tree);
					tree = tmpLoadTree;
					tmpLoadTree = nullptr;
				} else {
					std::cout << "ERROR: ITS NOT A DICTIONARY OR SOMETHING ELSE\n";
					DeleteTree(tmpLoadTree);
					tmpLoadTree = nullptr;
				}
				loadFile.close();
			} else { 								
				std::cout << "ERROR: PATH NOT ENTERED\n";
				continue;
			}
		} else { 
			DoToLower(input);
			FindElement(tree, input);
		}
	}
	DeleteTree(tmpLoadTree);
	DeleteTree(tree);
	return 0;
}