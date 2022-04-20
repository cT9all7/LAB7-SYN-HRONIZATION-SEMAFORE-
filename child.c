#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<stdbool.h>
#include<signal.h>

bool fl=false;
void up(int semid);
void down(int semid);
struct sembuf semops;
struct message
{
	char data[10];
}send;

void handler(int sig)
{
	if(sig=SIGINT) fl=true;
}

int main(int argc, char* argv[])
{
	printf("********HELLO I'M SENDER********\n");
	signal(SIGINT,handler);
	key_t key;
	key=ftok("/home/kali/progi/lab02/ftok",2);

	int msqid;
	msqid=msgget(key,0);
	printf("ид-р очереди : %d\n",msqid);
	
	int semid;
	semid=semget(key,0,0);
	printf("ид-р мас-ва семафоров : %d\n\n",semid);

	int fal1;
	while(fl!=true)
	{
		sleep(1);
		for(int i=0;i<10;i++)	send.data[i]=97+rand()%26;

		semops.sem_num=1;
		down(semid);
		
		semops.sem_num=0;
		down(semid);
		
		fal1=msgsnd(msqid,&send,10,0);
		printf("отправлено %d %s  \n",fal1,send.data);
		
		up(semid);
		
		semops.sem_num=2;
		up(semid);	
	}
	return 0;
}

void up(int semid)
{
	int fal;
	semops.sem_op=1;
	semops.sem_flg=0;
	fal=semop(semid,&semops,1);	
}

void down(int semid)
{
	int fal;
	semops.sem_op=-1;
	semops.sem_flg=0;
	fal=semop(semid,&semops,1);				
}