#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

int main(int argc, char const *argv[]) {
    const unsigned char* fName = "file.map";
    BOOL process;
    STARTUPINFO cInf;
    PROCESS_INFORMATION pInf;
    HANDLE hFileMap;
    HANDLE hFile;
    ZeroMemory(&cInf, sizeof(STARTUPINFO));
    const int lSize = 9;
    unsigned char* pGet;
    HANDLE MySemaphore;
    HANDLE rSemaphore;
    HANDLE wSemaphore;
    rSemaphore = CreateSemaphoreA(NULL, 1, 1, "MessegeSemaphore1");
    if (rSemaphore == NULL) {
        fprintf(stdout,"Semaphore messege: Error %ld\n", GetLastError());
        return 0;
    }
    WaitForSingleObject(rSemaphore, 0);
    wSemaphore = CreateSemaphoreA(NULL, 1, 1, "MessegeSemaphore2");
    if (wSemaphore == NULL) {
        fprintf(stdout,"Semaphore messege: Error %ld\n", GetLastError());
        return 0;
    }
    MySemaphore = CreateSemaphoreA(NULL, 1, 1, "MySemaphore");
    if (MySemaphore == NULL) {
        fprintf(stdout,"Semaphore: Error %ld\n", GetLastError());
        return 0;
    }
    WaitForSingleObject(MySemaphore, 0);
    WaitForSingleObject(wSemaphore, 0);
    process = CreateProcess("serv.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &cInf, &pInf);
    if (process == FALSE) {
        fprintf(stdout,"CreateProcess: Error %ld\n", GetLastError());
        return 0;
    }
    while (1) {
        if (!WaitForSingleObject(MySemaphore, 0)) {
            break;
        }
    }
    hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, fName);
	if (hFileMap == NULL){
		fprintf(stderr, "Can't open memory mapped file: %ld\n", GetLastError());
		return 0;
	}
    while (1) {
        printf("Enter expression> ");
        int a, b;
        char c;
        scanf("%d", &a);
        scanf(" %c", &c);
        scanf("%d", &b);
        unsigned char bytes[9];
        bytes[0] = (int)((a >> 24) & 0xFF) ;
        bytes[1] = (int)((a >> 16) & 0xFF) ;
        bytes[2] = (int)((a >> 8) & 0XFF);
        bytes[3] = (int)((a & 0XFF));
        bytes[4] = c;
        bytes[5] = (int)((b >> 24) & 0xFF) ;
        bytes[6] = (int)((b >> 16) & 0xFF) ;
        bytes[7] = (int)((b >> 8) & 0XFF);
        bytes[8] = (int)((b & 0XFF));
        unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, lSize);
        if (dataPtr == NULL) {
            fprintf(stdout,"GetPointer1: Error %ld\n", GetLastError());
            CloseHandle(hFileMap);
            CloseHandle(hFile);
            return 0;
        }
        for (int i = 0; i < 9; ++i) {
            *((unsigned char*)dataPtr + i) = bytes[i];
        }
        ReleaseSemaphore(rSemaphore, 1, NULL);
        while (1) {
            if (!WaitForSingleObject(wSemaphore, 0)) {
                break;
            }
        }
        pGet = (unsigned char*) MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, lSize);
        if (pGet == NULL) {
            fprintf(stderr, "Error reading from memory (MapViewOfFile): %ld\n", GetLastError());
            return 0;
        }
        int ans = ((pGet[0] << 24) + (pGet[1] << 16) + (pGet[2] << 8) + (pGet[3] ));
        printf("Result is : %d\n", ans);
    }
    CloseHandle(MySemaphore);
    CloseHandle(wSemaphore);
    CloseHandle(rSemaphore);
    TerminateProcess(pInf.hProcess, NO_ERROR);	
    return 0;
}
