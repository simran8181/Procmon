#include "header.h"


//EntryPoint Function :

int main(int argc,char *argv[])
{
	ProcessInfo *ppobj = NULL;
	char *ptr = NULL;
	BOOL bret ;
	char command[4][80];
	char str[80];       //Maximum size of command line is 80
	int count,hr,min,date,month;

	

	while(1)
	{
		fflush(stdin);
		strcpy_s(str,"");
		cout<<"Marvellous Procmon : > "<<endl;
		fgets(str,80,stdin);      // work as scanf

		count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);   // used to tokanise ie return the no.of tokens in this case it is 4

		if (count == 1)
		{
			if(_stricmp(command[0],"ps") == 0)
			{
				ppobj = new ProcessInfo();
				bret = ppobj -> ProcessDisplay("-a");
				if(bret == FALSE)
				
					cout<<"ERROR : Unable to dispaly process"<<endl;
					delete ppobj;
				
			}
			else if(_stricmp(command[0],"log") == 0)
			{
				ppobj = new ProcessInfo();
				bret = ppobj -> ProcessLog();
				if(bret == FALSE)
				
					cout<<"ERROR : Unable to create log file"<<endl;
					delete ppobj;
				
			}
			else if(_stricmp(command[0],"sysinfo") == 0)
			{
				bret = HardwareInfo();
				if(bret == FALSE)
				
					cout<<"ERROR : Unable to get hardware information"<<endl;
				
				cout<<"Hardware information of the system is : "<<endl;
			}
			else if(_stricmp(command[0],"readlog") == 0)
			{
				ProcessInfo *ppobj;
				ppobj = new ProcessInfo();
				cout<<"Enter log file details as :"<<endl;

				cout<<"Hour : ";
				cin>>hr;
				cout<<"Minute : ";
				cin>>min;
				cout<<"Date : ";
				cin>>date;
				cout<<"Month : ";
				cin>>month;

				bret = ppobj -> ReadLog(hr,min,date,month);
				if(bret == FALSE)
				
					cout<<"ERROR : Unable to read specified log file"<<endl;
				
				delete ppobj;
			}
			else if(_stricmp(command[0],"clear") == 0)
			{
				system("cls");
				continue;
			}
			else if(_stricmp(command[0],"help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if(_stricmp(command[0],"exit") == 0)
			{
				printf("Terminating the Marvellous Procmon\n");
				break;
			}
			else
			{
				printf("\nERROR : Command not found !!! \n");
				continue;
			}
		}

		else if(count == 2)
		{
				if(_stricmp(command[0],"ps")==0)
				{
					ppobj = new ProcessInfo();
					bret = ppobj -> ProcessDisplay(command[1]);
					if(bret == FALSE)
					
						cout<<"ERROR : Unable to dispaly process"<<endl;
					
						delete ppobj;
				}
		

			else if(_stricmp(command[0],"search") == 0)
			{
				ppobj = new ProcessInfo();
				bret = ppobj -> ProcessSearch(command[1]);
				if(bret == FALSE)
				
					cout<<"ERROR : There is no such process"<<endl;
					
				
				delete ppobj;
				continue;
			}

			else if(_stricmp(command[0],"kill") == 0)
			{
				ppobj = new ProcessInfo();
				bret = ppobj -> KillProcess(command[1]);
				if(bret == FALSE)
				{
					cout<<"ERROR : There is no such process"<<endl;
				}
				else
				{
					cout<<command[1]<<"Terminated successfully"<<endl;
					
				}
				delete ppobj;
				continue;
			}
		}

				else
				{
					cout<<endl<<"Command not found\n"<<endl;
					continue;
				}
			

			
		}//end of while

		

		return 0;

}//end of main
