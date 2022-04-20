#include<stdio.h>
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
}recive;
void handler(int sig)
{
	if(sig=SIGINT) fl=true;
}

int main(int argc, char* argv[])
{
	printf("********HELLO I'M RECIPIENT********\n");
	signal(SIGINT,handler);
	key_t key;
	key=ftok("/home/kali/progi/lab02/ftok",2);
	
	int msqid;
	msqid=msgget(key,0);
	printf("ид-р очереди : %d\n",msqid);
	
	int semid;
	semid=semget(key,0,0);
	printf("ид-р мас-ва семафора : %d\n\n",semid);
	
	int fal1;
	while(fl!=true)
	{	
		sleep(1);
		semops.sem_num=2;
		down(semid);
		
		semops.sem_num=0;
		down(semid);
		
		fal1=msgrcv(msqid,&recive,10,0,0);
		printf("принято %d  %s \n",fal1,recive.data);
		
		up(semid);
		semops.sem_num=1;
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