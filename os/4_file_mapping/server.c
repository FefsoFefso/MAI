#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

int main(int argc, char const *argv[]){
    const unsigned char* fName = "file.map";
    HANDLE hFileMap;
    HANDLE hFile;
    const int lSize = 9;
    HANDLE MySemaphore;
    HANDLE rSemaphore;
    HANDLE wSemaphore;
    hFile = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		fprintf(stdout,"CreateFile: Error %ld\n", GetLastError());
	    return 0;
    }
    hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, lSize, fName);
    if (hFileMap == NULL) {
        fprintf(stdout,"CreateFileMapping: Error %ld\n", GetLastError());
        return 0;
    }
    MySemaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, "MySemaphore");
    if (MySemaphore == NULL) {
        fprintf(stdout, "Open Semaphore: Error %ld\n", GetLastError());
        return 0;
    } 
    rSemaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, "MessegeSemaphore1");
    if (rSemaphore == NULL) {
        fprintf(stdout,"Semaphore messege: Error %ld\n", GetLastError());
        return 0;
    }
    wSemaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, "MessegeSemaphore2");
    if (wSemaphore == NULL) {
        fprintf(stdout,"Semaphore messege: Error %ld\n", GetLastError());
        return 0;
    }
    ReleaseSemaphore(MySemaphore, 1, NULL);
    while (1) {
        while (1) {
            if (!WaitForSingleObject(rSemaphore, 0)) {
                break;
            }
        }
        unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, lSize);
        if (dataPtr == NULL) {
            fprintf(stdout,"GetPointer2: Error %ld\n", GetLastError());
            CloseHandle(hFileMap);
            CloseHandle(hFile);
            return 0;
        }
        int a = ((dataPtr[0] << 24) + (dataPtr[1] << 16) + (dataPtr[2] << 8) + (dataPtr[3]));
        char c = dataPtr[4];
        int b = ((dataPtr[5] << 24) + (dataPtr[6] << 16) + (dataPtr[7] << 8) + (dataPtr[8]));
        int res;
        if (c == '*') {
            res = a * b;
        } else {
            if (b == 0) {
                res = 0;
            } else {
                res = a / b;
            }
        }
        dataPtr[0] = (int)((res >> 24) & 0xFF) ;
        dataPtr[1] = (int)((res >> 16) & 0xFF) ;
        dataPtr[2] = (int)((res >> 8) & 0XFF);
        dataPtr[3] = (int)((res & 0XFF));
        ReleaseSemaphore(wSemaphore, 1, NULL);
    }
    CloseHandle(hFile);
    return 0;
}