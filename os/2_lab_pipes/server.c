#include <stdio.h>
#include <windows.h>

int main(int argc, char const *argv[]){
    HANDLE hNamedPipe;
    LPCTSTR pipeName = "\\\\.\\pipe\\mynamedpipe";
    DWORD bitesReaded;
    DWORD bitesWrited;
    hNamedPipe = CreateFile(
    	pipeName, GENERIC_READ | GENERIC_WRITE,
   		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(hNamedPipe == INVALID_HANDLE_VALUE) {
			fprintf(stdout,"CreateFile: Error %ld\n", 
			GetLastError());
	    return 0;
    }
    while (1){
        unsigned char byteA[4];
        if (ReadFile(hNamedPipe, byteA, 4, &bitesReaded, NULL) == FALSE){
            fprintf(stdout, "ReadFile: Error %ld\n", GetLastError());
            break;
        }
        int a = ((byteA[0] << 24) 
                   + (byteA[1] << 16) 
                   + (byteA[2] << 8) 
                   + (byteA[3] ) );
        unsigned char byteC[1];
        if (ReadFile(hNamedPipe, byteC, 1, &bitesReaded, NULL) == FALSE){
            fprintf(stdout, "ReadFile: Error %ld\n", GetLastError());
            break;
        }
        char c = byteC[0];
        unsigned char byteB[4];
        if (ReadFile(hNamedPipe, byteB, 4, &bitesReaded, NULL) == FALSE){
            fprintf(stdout, "ReadFile: Error %ld\n", GetLastError());
            break;
        }
        int b = ((byteB[0] << 24) 
                   + (byteB[1] << 16) 
                   + (byteB[2] << 8) 
                   + (byteB[3] ) );
        unsigned char bytePush[4];
        int result;
        if (c == '/'){
            if (b == 0){
                result = 0;
            }else{
                result = a / b;
            }
        }
        if (c == '*'){
            result = a * b;
        }
        bytePush[0] = (int)((result >> 24) & 0xFF) ;
        bytePush[1] = (int)((result >> 16) & 0xFF) ;
        bytePush[2] = (int)((result >> 8) & 0XFF);
        bytePush[3] = (int)((result & 0XFF));
        if (WriteFile(hNamedPipe, bytePush, 4, &bitesWrited, NULL) == FALSE){
            fprintf(stdout, "WriteFile: Error %ld\n", GetLastError());
            break;
        }
    }
    return 0;
}