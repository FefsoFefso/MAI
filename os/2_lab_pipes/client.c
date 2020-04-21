#include <stdio.h>
#include <windows.h>

int main(int argc, char const *argv[]) {
    LPCTSTR pipeName = "\\\\.\\pipe\\mynamedpipe";
    STARTUPINFO cInf;
    ZeroMemory(&cInf,sizeof(STARTUPINFO));
    PROCESS_INFORMATION pInf;
    HANDLE namedPipe;
    BOOL process;
    BOOL connection;
    DWORD bitesReaded;
    DWORD bitesWrited;
    namedPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, 
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 
                                101, 101, 0.5, NULL);
    if (namedPipe == INVALID_HANDLE_VALUE) {
        fprintf(stdout, "CreateNamedPipe: Error %ld\n", GetLastError());
        return 0;
    }
    process = CreateProcess("serv.exe",NULL,
		                        NULL,NULL,FALSE,NULL,NULL,NULL,&cInf,&pInf);
    if (process == FALSE){
        fprintf(stdout,"CreateProcess: Error %ld\n", GetLastError());
        return 0;
    }
    connection = ConnectNamedPipe(namedPipe, NULL);
    if (connection == FALSE){
        fprintf(stdout, "Connection: Error %ld\n", GetLastError());
        return 0;
    }
    while (1){
        printf("Enter expression> ");
        int a, b;
        char c;
        scanf("%d",&a);
        scanf("%c",&c);
        scanf("%d", &b);
        unsigned char byteC[1];
        unsigned char byteA[4];
        unsigned char byteB[4];
        byteC[0] = c;
        byteA[0] = (int)((a >> 24) & 0xFF) ;
        byteA[1] = (int)((a >> 16) & 0xFF) ;
        byteA[2] = (int)((a >> 8) & 0XFF);
        byteA[3] = (int)((a & 0XFF));
        byteB[0] = (int)((b >> 24) & 0xFF) ;
        byteB[1] = (int)((b >> 16) & 0xFF) ;
        byteB[2] = (int)((b >> 8) & 0XFF);
        byteB[3] = (int)((b & 0XFF));
        if (WriteFile(namedPipe, byteA, 4, &bitesWrited, NULL) == FALSE){
            fprintf(stdout, "WriteFile: %ld\n", GetLastError());
            break;
        }
        if (WriteFile(namedPipe, byteC, 1, &bitesWrited, NULL) == FALSE){
            fprintf(stdout, "WriteFile: %ld\n", GetLastError());
            break;
        }
        if (WriteFile(namedPipe, byteB, 4, &bitesWrited, NULL) == FALSE){
            fprintf(stdout, "WriteFile: %ld\n", GetLastError());
            break;
        }
        unsigned char byteGets[4];
        if(ReadFile(namedPipe, byteGets, 4, &bitesReaded, NULL)) {
			int anotherInt = ((byteGets[0] << 24) 
                   + (byteGets[1] << 16) 
                   + (byteGets[2] << 8) 
                   + (byteGets[3] ) );
			printf("Answer is : %d\n", anotherInt);
        }else{
			fprintf(stdout,"ReadFile: Error %ld\n", GetLastError());
			break;
		}
    }
    connection = DisconnectNamedPipe(namedPipe);
    CloseHandle(namedPipe); 
	TerminateProcess(pInf.hProcess,NO_ERROR);	
    return 0;
}