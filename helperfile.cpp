
#include"header.h"



ThreadInfo :: ThreadInfo(DWORD no)
{
	PID = no;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,PID);

	if(hThreadSnap == INVALID_HANDLE_VALUE)
	{
		cout<<"Unable to create the snapshot of current thread pool"<<endl;
		return;
	}
	te32.dwSize = sizeof(THREADENTRY32);
}

BOOL ThreadInfo :: ThreadDisplay()
{
	if(!Thread32First(hThreadSnap , &te32))
	{
		cout<<"Error : In getting first thread"<<endl;
		CloseHandle(hThreadSnap);
		return FALSE;
	}

	cout<<endl<<"Thread of this process : "<<endl;
	do
	{
		if(te32.th32OwnerProcessID == PID)
		{
			cout<<"\tThread ID : " << te32.th32ThreadID<<endl;
		}
	}while(Thread32Next(hThreadSnap , &te32));

	CloseHandle(hThreadSnap);
	return TRUE;
}


DLLInfo :: DLLInfo(DWORD no)
{
	PID = no;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,PID);

	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout<<"Unable to create the snapshot of current thread pool"<<endl;
		return;
	}
	me32.dwSize = sizeof(MODULEENTRY32);
}


BOOL DLLInfo :: DependentDLLDisplay()
{
	char arr[200];

	if(!Module32First(hProcessSnap , &me32))
	{
		cout<<"FAILED to get DLL Information :"<<endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	cout<<endl<<"Dependent DLL of this process : "<<endl;
	do
	{
		wcstombs_s(NULL,arr,200,me32.szModule,200);
		cout<<arr<<endl;

	}while(Module32Next(hProcessSnap , &me32));

	CloseHandle(hProcessSnap);
	return TRUE;
}



ProcessInfo :: ProcessInfo()
{
	ptobj   = NULL;
	pdobj = NULL;


	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout<<"Unable to create the snapshot of current thread pool"<<endl;
		return;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
}


BOOL ProcessInfo :: ProcessLog()
{
	char arr[512] , FileName[50];
	char* month[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEPT","OCT","NOV","DEC"};
	int ret = 0 , fd = 0 , count = 0;
	SYSTEMTIME it;
	LOGFILE fobj;
	FILE *fp;

	GetLocalTime(&it);

	sprintf_s(FileName , "G://MarvellousLog %02d_%02d_%02d %s.txt",it.wHour,it.wMinute,it.wDay,month[it.wMonth-1]);

	fp = fopen(FileName, "wb");
	if(fp==NULL)
	{
		cout<<"Unable to create log file"<<endl;
	}
	else
	{
		cout<<"LogFile successfully created as :"<<FileName<<endl;
		cout<<"Time of log file crearion is :"<<it.wHour<<":"<<it.wMinute<<":"<<it.wDay<<"th"<<month[it.wMonth-1]<<endl;
	}

	if(!Process32First(hProcessSnap , &pe32))
	{
		cout<<"error in findinf=g first process :"<<endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
		strcpy_s(fobj.ProcessName , arr);
		fobj.pid = pe32.th32ProcessID;
		fobj.ppid = pe32.th32ParentProcessID;
		fwrite(&fobj,sizeof(fobj),1,fp);

	}while(Process32Next(hProcessSnap , &pe32));

	CloseHandle(hProcessSnap);
	fclose(fp);
	return TRUE;
}


BOOL ProcessInfo :: ProcessDisplay(char *option)
{
	char arr[200];

	if(!Process32First(hProcessSnap , &pe32))
	{
		cout<<"Errror in finding first process :"<<endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	
	do
	{
		cout<<endl<<"-------------------------------------------------------------------------------------------------";
		wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
		cout<<endl<<"Process Name :"<<arr;
		cout<<endl<<"Pid :"<<pe32.th32ProcessID;
		cout<<endl<<"Parent pid :"<<pe32.th32ParentProcessID;
		cout<<endl<<"No. of thread :"<<pe32.cntThreads;

		if((_stricmp(option,"-a")==0) || (_stricmp(option,"-d")==0) || (_stricmp(option,"-t")==0))
		{
			if((_stricmp(option,"-t")==0) || (_stricmp(option,"-a")==0))
			{
				ptobj = new ThreadInfo(pe32.th32ProcessID);
				ptobj ->ThreadDisplay();
				delete ptobj;
			}
			if((_stricmp(option,"-d")==0) || (_stricmp(option,"-a")==0))
			{
				pdobj = new DLLInfo(pe32.th32ProcessID);
				pdobj ->DependentDLLDisplay();
				delete ptobj;
			}
		}
		cout<<endl<<"-------------------------------------------------------------------------------------------------";

	}while(Process32Next(hProcessSnap , &pe32));

	CloseHandle(hProcessSnap);
	return TRUE;
}

BOOL ProcessInfo :: ReadLog(DWORD hr,DWORD min,DWORD date,DWORD month)
{
	char FileName[50];
	char* montharr[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEPT","OCT","NOV","DEC"};
	int ret = 0 , count = 0;
	LOGFILE fobj;
	FILE *fp;

	sprintf_s(FileName , "C://MarvellousLog %02d_%02d_%02d %s.txt",hr,min,date,montharr[month-1]);

	fp = fopen(FileName, "rb");
	if(fp==NULL)
	{
		cout<<"Unable to create log file"<<endl;
		return FALSE;
	
	}
	while((ret = fread(&fobj , 1,sizeof(fobj),fp)) != 0)
	{
		cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
		cout<<endl<<"Process Name :"<<fobj.ProcessName;
		cout<<endl<<"Pid of current process :"<<fobj.pid;
		cout<<endl<<"Parent pid :"<<fobj.ppid;
		cout<<endl<<"No. of thread :"<<fobj.thread_cnt;
	}

	return TRUE;
}


BOOL ProcessInfo ::ProcessSearch(char *name)
{
	char arr[200];
	BOOL flag = FALSE;

	if(!Process32First(hProcessSnap , &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL,arr,200,pe32.szExeFile,200);

		if(_stricmp(arr,name) == 0)
		{
			cout<<endl<<"Process Name :"<<arr;
			cout<<endl<<"Pid :"<<pe32.th32ProcessID;
			cout<<endl<<"Parent pid :"<<pe32.th32ParentProcessID;
			cout<<endl<<"No. of thread :"<<pe32.cntThreads;

			flag = TRUE;
			break;
		}
	}while(Process32Next(hProcessSnap , &pe32));

	CloseHandle(hProcessSnap);
	return flag;
}


BOOL ProcessInfo ::KillProcess(char * name)
{
	char arr[200];
	BOOL bret ;
	int pid = -1;
	HANDLE hprocess;

	if(!Process32First(hProcessSnap , &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL,arr,200,pe32.szExeFile,200);

		if(_stricmp(arr,name) == 0)
		{
			pid = pe32.th32ProcessID;
			break;
		}
	}while(Process32Next(hProcessSnap , &pe32));

	CloseHandle(hProcessSnap);

	if(pid == -1)
	{
		cout<<"ERROR : There is no such process"<<endl;
		return FALSE;
	}

	hprocess = OpenProcess(PROCESS_TERMINATE ,FALSE , pid);
	if(hprocess == NULL)
	{
		cout<<"ERROR : There is no access to terminate :"<<endl;
		return FALSE;
	}

	bret = TerminateProcess(hprocess,0);
	if(bret == FALSE)
	{
		cout<<"ERROR : Unable to terminate process :";
		return FALSE;
	}

}


BOOL HardwareInfo()
{
	SYSTEM_INFO siSysInfo;

	GetSystemInfo(&siSysInfo);
	
	cout<<"OEM ID :"<<siSysInfo.dwOemId<<endl;
	cout<<"No. of Processors :"<<siSysInfo.dwNumberOfProcessors<<endl;
	cout<<"Page Size :"<<siSysInfo.dwPageSize<<endl;
	cout<<"Processor Type :"<<siSysInfo.dwProcessorType<<endl;
	cout<<"Minimum Allocation Address :"<<siSysInfo.lpMinimumApplicationAddress<<endl;
	cout<<"Maximum Allocation Address :"<<siSysInfo.lpMaximumApplicationAddress<<endl;
	cout<<"Active Process Mask :"<<siSysInfo.dwActiveProcessorMask<<endl;

	return TRUE;
}

void DisplayHelp()
{
	cout<<"ps : Dispaly all information of process"<<endl;
	cout<<"ps -t : Dispaly all information of threads"<<endl;
	cout<<"ps -d: Dispaly all information about DLL"<<endl;
	cout<<"cls : Clear the contents of console"<<endl;
	cout<<"log : create log of current running process on C drive"<<endl;
	cout<<"readlog : Dispaly information of specified log file"<<endl;
	cout<<"sysinfo : Dispaly the current hardware configration"<<endl;
	cout<<"search : Search and Dispaly information of specific running 1process"<<endl;
	cout<<"kill : Terminate the specific process"<<endl;
	cout<<"exit : Terminate Marvellous Procmon"<<endl;
}