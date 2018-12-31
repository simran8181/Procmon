

#include<stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<string.h>
#include<io.h>

using namespace std;
//  Forward declarations:

BOOL HardwareInfo();
void DisplayHelp();





typedef struct LogFile
{
	char ProcessName[100];
	unsigned int pid;
	unsigned int ppid;
	unsigned int thread_cnt;

}LOGFILE;

class ThreadInfo
{
	private :
				DWORD PID;
				HANDLE hThreadSnap;
				THREADENTRY32 te32;
	public :
				ThreadInfo(DWORD);
				BOOL ThreadDisplay();

};

class DLLInfo
{
	private :
				DWORD PID;
				HANDLE hProcessSnap;
				MODULEENTRY32 me32;
	public :
				DLLInfo(DWORD);
				BOOL DependentDLLDisplay();

};


class ProcessInfo
{
	private :
				DWORD PID;
				DLLInfo *pdobj;
				ThreadInfo *ptobj;
				HANDLE hProcessSnap;
				PROCESSENTRY32 pe32;
	public :
				ProcessInfo();
				BOOL ProcessDisplay(char *);
				BOOL ProcessLog();
				BOOL ReadLog(DWORD,DWORD,DWORD,DWORD);
				BOOL ProcessSearch(char *);
				BOOL KillProcess(char *);
};





