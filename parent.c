#include<stdio.h>
#include<stdbool.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(int argc, char* argv[])
{
	bool flag;
	key_t key;
	key=ftok("/home/kali/progi/lab02/ftok",2);
	int msqid;
	msqid=msgget(key,(IPC_CREAT|0666));
	int semid;
	semid=semget(key,3,(IPC_CREAT|0666));
	
	union semun opt;
	int val;
	
	opt.val=1;//mutex
	val=semctl(semid,0,SETVAL,opt);	
	
	opt.val=10;//size queue
	val=semctl(semid,1,SETVAL,opt);	
	
	opt.val=0;//items
	val=semctl(semid,2,SETVAL,opt);	

	pid_t pid=9999999;
	pid_t pid1=999999;
	
	pid_t mpid[15];
	pid_t mpid1[15];
	struct msqid_ds* buf;
	int ret,ret1,stat,stat1;
	char a;
	flag=true;
	int i=0;
	int i1=0;
	for(int T=0;;T++)
	{	
		a=getchar();	
		if(a=='p')//+producer
		{
			pid=fork();
			if(pid>0){ mpid[i]=pid; i++;}
			if(!pid) ret=execve("/home/kali/progi/lab02/child",NULL,NULL);
			printf("\n%d\n",i);
		}
		else if(a=='P')//+consumer
		{
			pid1=fork();
			if(pid1>0){ mpid1[i1]=pid1; i1++;}
			printf("\n%d\n",i1);
			if(!pid1) ret1=execve("/home/kali/progi/lab02/child1",NULL,NULL);
		}
		else if(a=='-')
		{
			printf("kill processes\n");
			for(int j=0;j<i;j++)
			{
				kill(mpid[j],SIGKILL);
				waitpid(pid,&stat,0);
			}
			for(int j=0;j<i1;j++){
				kill(mpid1[j],SIGKILL);
				waitpid(pid1,&stat1,0);
			}
			i=0;
			i1=0;
		}
		else if(a=='q') 
		{
			semctl(semid,0,IPC_RMID);
			msgctl(msqid,IPC_RMID,buf);
			printf("buy buy\n");
			for(int j=0;j<i;j++)
			{
				kill(mpid[j],SIGINT);
				waitpid(pid,&stat,0);
			}
			for(int j=0;j<i1;j++)
			{
				kill(mpid1[j],SIGINT);
				waitpid(pid1,&stat1,0);
			}
			i=0;
			i1=0;
			printf("buy buy\n");
		}
	}
	return 0;
}