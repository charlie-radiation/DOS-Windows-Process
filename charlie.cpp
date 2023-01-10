#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

BOOL Process_List();

int main(void)
{
		system("Color 0A");
		 Process_List();  
    	return 0;
}

BOOL Process_List()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("CreateToolhelp32Snapshot (of processes)"));
        return(FALSE);
    }

	  pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32))
    {
        _tprintf(TEXT("Process32First")); // show cause of failure
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return(FALSE);
    }

    do
    {
       
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 threadEntry; 
    threadEntry.dwSize = sizeof(THREADENTRY32);
    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == pe32.th32ProcessID)
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,threadEntry.th32ThreadID);
            
            SuspendThread(hThread);
            CloseHandle(hThread);
        }
    } 
	while (Thread32Next(hThreadSnapshot, &threadEntry));

    CloseHandle(hThreadSnapshot);

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return(TRUE);
}


