#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string.h>

const int WORD_SIZE = 16;

std::map <std::string, std::vector<int>> GetBadSymbolTable(std::vector <std::string>& pattern) {
	std::map <std::string, std::vector<int>> table;
	int size = pattern.size();
	for (int i = 0; i < size; ++i) {
		table[pattern[i]].push_back(i);
	}
	return table;
}

std::vector <int> GetGreatSuffixTable(std::vector <std::string>& pattern) {
	int pSize = pattern.size();
	std::vector <int> suffShift(pSize + 1, pSize);
	std::vector <int> zFunc(pSize + 1, 0);
	int maxZidx = 0;
	int maxZ = 0;
 	for (int j = 1; j < pSize; ++j) {
   		if (j <= maxZ) { 
			zFunc[j] = std::min(maxZ - j + 1, zFunc[j - maxZidx]);
	 	}
		while (j + zFunc[j] < pSize && (pattern[pSize - 1 - zFunc[j]] == pattern[pSize - 1 - (j + zFunc[j])])) {
			zFunc[j]++;
		}
   		if (j + zFunc[j] - 1 > maxZ) {
     		maxZidx = j;
    		maxZ = j + zFunc[j] - 1;
   		}
 	}
	for (int j = pSize - 1; j > 0; j--) { 
		suffShift[pSize - zFunc[j]] = j; 
	}
	for (int j = 1; j < pSize; j++) {
		int r = 0;
   		if (j + zFunc[j] == pSize) {
     		for (; r <= j; r++) {
       			if (suffShift[r] == pSize) {
					   suffShift[r] = j;
				}
		    }
		}
	}
	return suffShift;
}

int GetBadSymbolShift(std::map <std::string, std::vector<int>>& badSymbolTable, std::string str, int& rightPos) {
	if (badSymbolTable[str].empty()) {
		return -1;
	}
	int ans = -1;
	int right = badSymbolTable[str].size();
	std::vector <int>& positions = badSymbolTable[str];
	int left = -1;
	int mid = 0;
	bool isFind = false;
	while (right - left > 1) {
		mid = (left + right) / 2;
		if (positions[mid] < rightPos) {
			left = mid;
		}
		if (positions[mid] > rightPos) {
			right = mid;
		}
		if (positions[mid] == rightPos) {
			isFind = true;
			break;
		}
	}
	if (isFind) {
		if (mid == 0) {
			ans = -1;
		} else {
			ans =  positions[mid - 1];
		}
	} else {
		if (right == 0) {
			ans = -1;
		} else {
			ans = positions[right - 1];
		}
	}
	return ans;
}

bool isChar(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return true;
	} else {
		return false;
	}
}

std::string GetNextWord(std::string &in, int& pos, int size, bool &ok) {
	char word[WORD_SIZE + 1] {' '}; 
	int cur = 0;
	int wasFind = false;
	while (!wasFind && pos + cur < size) {
		if (pos + cur >= size) {
			break;
		}
		if (!isChar(in[pos + cur])) {
			++pos;
			continue;
		}
		while (pos + cur < size && isChar(in[pos + cur])) {
			word[cur] = std::tolower(in[pos + cur]);
			++cur;
			wasFind = true;
		}
	}
	if (!wasFind) {
		ok = false;
	}
	pos += cur + 1;
	return (std::string)word;
}
int main(){
	std::ios::sync_with_stdio(false);
	std::vector <std::string> pattern;
	std::string input;  
	int currentPos = 0;
    	bool isEnter = false;
    	std::getline(std::cin, input);
	int sSize = input.size();
	while (currentPos < sSize) {
        std::string word; 
		bool isFind = true;
		word = GetNextWord(input, currentPos, sSize, isFind);
		if (isFind) {
            isEnter = true;
			pattern.push_back(word);
		}	
	}
    if (!isEnter) {
        while(std::getline(std::cin, input)) {}
        return 0;
    }
	std::map <std::string, std::vector<int>> badSymbolTable = GetBadSymbolTable(pattern);
	std::vector <int> shiftTable = GetGreatSuffixTable(pattern);
	int shiftTableSize = shiftTable.size();
	int rowPos = 1;
	int columnPos = 1;
	int needToWrite = shiftTableSize - 1;
	int curPos = 0;
	int it = 0;
	int textSize = shiftTableSize * 2;
	std::vector <std::pair<std::string, std::pair<int, int>>> text(textSize);
	while (std::getline(std::cin, input)) {
		currentPos = 0;
		sSize = input.size();
		while (currentPos < sSize) {
			if (needToWrite != 0) {
				bool isFind = true;
				std::string curTextWord = GetNextWord(input, currentPos, sSize, isFind);
				if (!isFind) {
					continue;
				}
				text[it] = std::make_pair(curTextWord, std::make_pair(rowPos, columnPos));
				++columnPos;
				--needToWrite;
				it = (it + 1) % textSize;
			}
			if (needToWrite == 0) {
				int j = shiftTableSize - 2;
				while (j >= 0 && pattern[j] == text[(curPos + j) % textSize].first) {
					--j;
					if (j < 0) {
				    	std::cout << text[curPos].second.first << ", " << text[curPos].second.second <<'\n';    
						break;
					}
				}
				int shift = 1;
				if (j < 0) {
					shift = shiftTable[j + 1];
				} else {
                    			int bsShift = GetBadSymbolShift(badSymbolTable, text[(curPos + j) % textSize].first, j);
					shift =  std::max(1, std::max(shiftTable[j + 1], j - bsShift));
				}
				needToWrite = shift;
				curPos = (curPos + shift) % textSize;
			}
		}
		++rowPos;
		columnPos = 1;
	}
	return 0;
}
