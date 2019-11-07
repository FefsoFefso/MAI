#include <iostream>
#include <cstdlib>

const int INDEX_SIZE = 6;

struct Input {
    int key;
    int numSize;
    unsigned long long value;
};

int main() {
    std::ios::sync_with_stdio(false);
    Input* input = nullptr;
    Input* answer = nullptr;
    long long curSize = 0;
    int inKey = 0;
    long long maxValue = 0;
    long long* prefix = nullptr;
    unsigned long long inValue = 0;
    long long localMemory = 1;
    while (std::cin >> inKey) {
        std::cin >> inValue;
        long long localKey = inKey;
        int localNumSize = 0;
        while (localKey > 0) {
            localNumSize += 1;
            localKey /= 10;
        }
        curSize += 1;
        if (curSize >= localMemory){
            localMemory *= 2;
            input = (Input*) realloc (input, (localMemory) * sizeof(Input));
        }
        input[curSize - 1].key = inKey;
        input[curSize - 1].numSize = localNumSize;
        input[curSize - 1].value = inValue;
        if (inKey > maxValue) {
            maxValue = inKey;
        } 
    }
    prefix = new long long[maxValue + 1];
    for (long long i = 0; i <= maxValue; ++i) {
        prefix[i] = 0;
    }
    answer = new Input[curSize + 1];
    for (long long i = 0; i < curSize; ++i) {
        prefix[input[i].key] += 1;
    }
    for (long long i = 1; i <= maxValue; ++i) {
        prefix[i] = prefix[i - 1] + prefix[i];
    }
    for (long long i = curSize - 1; i >= 0; --i) {
        answer[--prefix[input[i].key]] = input[i];
    }
    for (long long i = 0; i < curSize; ++i) {
        if (answer[i].key == 0) {
            std::cout << "000000";
        }else {
            for (int j = 0; j < INDEX_SIZE - answer[i].numSize; ++j){
                std::cout << "0";
            }
            std::cout << answer[i].key;
        }
        std::cout << " " << answer[i].value << '\n';
    }
   free(input);
   delete[] prefix;
   delete[] answer;
   return 0;
}
