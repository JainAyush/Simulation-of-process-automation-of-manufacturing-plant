#include<stdio.h>
#include "header.h"
#include<time.h>
int main()
{
	
	pid_t pid,id1,id2,id3,id4,id;	
	int signo,fd,fd2,fd3,fd4,y=0,x=0,a=0;
	int iron,rearglass,frontglass,sideglass,paint,tyre,engine,ackit,furnkit;

	FILE *fl;	

// below is code for entering time when process is starting in log file
		time_t mytime;
		fl = fopen("log.txt","a+");
		fprintf(fl,"\n%s\n","LOG FILE IS STARTING");
                mytime = time(NULL);
		char* tmp=ctime(&mytime);
		tmp=tmp+24;
		*tmp='\0';
		tmp=tmp-24;
		fprintf(fl,"\nat %s time main process has started\n",tmp);
		fclose(fl);

// initialisation of shared memory
	int *shm;
	int shmid;
	key_t key=5;
	if((shmid=shmget(key,(15*sizeof(int)),IPC_CREAT|0666))<0)
	{
	perror("shmget");
	exit(1);
	}
	if((shm=shmat(shmid,NULL,0))==(int *)-1){
	perror("shmat");
	exit(1);
	}
	*(shm+12)=0;

//implementation of signals and their handlers

	static struct sigaction act;
	void catchint(int signo)
	{
		if(signo==SIGINT)
		{
		//sleep(5);	
	
		// using fifo reading the pid of file which is sending the signal 

			if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
		        {
		                printf("fatal error in opening fifo1.\n");
		                exit(5);
		        }
			
			read(fd,&id,4);
			//printf("\nid received: %d\n",id);

		
			fl = fopen("log.txt","a+");			
			fprintf(fl,"\nsignal received from %d to stop it\n",id);		// entering info into log file

			mytime = time(NULL);							// for time 
			char* tmp=ctime(&mytime);
			tmp=tmp+24;
			*tmp='\0';
			tmp=tmp-24;

			fprintf(fl,"\nat %s time %d process is terminating\n",tmp,id);
			fclose(fl);		

			if(id==id1)
			{
				printf("\n process 1 stopped working \n");
			}
	
			if(id==id2)
			{
				printf("\n process 2 stopped working \n");
			}
	
			if(id==id3)
			{
				printf("\n process 3 stopped working \n");
			}
	
			if(id==id4)
			{
				printf("\n process 4 stopped working \n");
			}
				
			kill(id,SIGINT);				// sending signal interrupt to close the process gracefully
			close(fd);
		}


		if(signo==SIGUSR1)
		{
			
			kill(id1,SIGKILL);
//			kill(id2,SIGKILL);
//			kill(id3,SIGKILL);
//			kill(id4,SIGKILL);
		}

	}

	sigfillset(&(act.sa_mask));
	act.sa_handler=catchint;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);


	
	pid=getpid();			// pid of main process


		// making fifo for transfer of the pid's to all the four processes

	 if((fd=mkfifo("/home/ayush/Desktop/fifo1",0666))==-1)
        {
                if(errno!=EEXIST)
                {
                        printf("fatal error in making fifo1\n");
                        exit(4);
                }	
	}

	if((fd2=mkfifo("/home/ayush/Desktop/fifo2",0666))==-1)
        {
                if(errno!=EEXIST)
                {
                        printf("fatal error in making fifo2\n");
                        exit(4);
                }	
	}

	if((fd3=mkfifo("/home/ayush/Desktop/fifo3",0666))==-1)
        {
                if(errno!=EEXIST)
                {
                        printf("fatal error in making fifo3\n");
                        exit(4);
                }	
	}
	
	if((fd4=mkfifo("/home/ayush/Desktop/fifo4",0666))==-1)
        {
                if(errno!=EEXIST)
                {
                        printf("fatal error in making fifo4\n");
                        exit(4);
                }	
	}

	fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK);		// opening of the fifo using read-write permission
	fd2=open("/home/ayush/Desktop/fifo2",O_RDWR|O_NONBLOCK);
	fd3=open("/home/ayush/Desktop/fifo3",O_RDWR|O_NONBLOCK);
	fd4=open("/home/ayush/Desktop/fifo4",O_RDWR|O_NONBLOCK);	

	printf("\nmy pid is %d\n",pid);
	sleep(20);
	read(fd,&id1,4);						// reading process id's of differnt processe
	read(fd2,&id2,4);
	read(fd3,&id3,4);
	read(fd4,&id4,4);

	
	printf("\npid of first process is %d \n",id1);
	printf("\npid of second process is %d \n",id2);
	printf("\npid of third process is %d \n",id3);
	printf("\npid of fourth process is %d \n",id4);
	
	write(fd,&pid,4);						// wrirng main process id to all fifo to send them to differnt processes
	write(fd2,&pid,4);
	write(fd3,&pid,4);
	write(fd4,&pid,4);
	


	while(1)
	{
		printf("\n\t Menu \n 1 --> Change data of raw materials \n 2 --> Change temperature of of first machine \n 3 --> POWER fAILURE\n 4 --> Exit\n");
		scanf("%d",&x);
		//printf("\njay\n");
		if(x==1)
		{

	/** crirical section **/

			
			if(*(shm+12)==1)			// here i am using the concept of semaphore to lock the shared variable memory
								//   							using a shared variable.
			{
				printf("shared memory is being used");
				while(1)
				{
					//sleep(2);
					//printf("hi\n");
					if(*(shm+12)==0)
					break;
				}
			}

			if(*(shm+12)==0)
			{
			*(shm+12)=1;
			printf("\nEnter data of raw materials\n");


			printf("\nenter value of iron(in kgs):");	
			scanf("%d",&iron);
			printf("\nenter number of rearglass:");
			scanf("%d",&rearglass);
			printf("\nenter number of frontglass:");
			scanf("%d",&frontglass);
			printf("\nenter number of sideglass:");
			scanf("%d",&sideglass);
			printf("\nenter number of paint(no. of cans):");
			scanf("%d",&paint);
			printf("\nenter number of tyre:");
			scanf("%d",&tyre);
			printf("\nenter number of engine:");
			scanf("%d",&engine);
			printf("\nenter number of ackit:");
			scanf("%d",&ackit);
			printf("\nenter number of furniturekit:");
			scanf("%d",&furnkit);

		fl = fopen("log.txt","a+");
		fprintf(fl,"\n%s\n","entering the value of raw material");
		fprintf(fl,"\niron:%d\nrearglass:%d\nfrontglass:%d\nsideglass:%d\npaint:%d\ntyre:%d\nengine:%d\nackit:%d\nfurnkit:%d\n",iron,rearglass,frontglass,sideglass,paint,tyre,engine,ackit,furnkit);
		fclose(fl);
		

		// entering the values of raw materials used
			*(shm)=iron;					
			*(shm+1)=rearglass;
			*(shm+2)=frontglass;
			*(shm+3)=sideglass;
			*(shm+4)=paint;
			*(shm+5)=tyre;
			*(shm+6)=engine;
			*(shm+7)=ackit;
			*(shm+8)=furnkit;
				x=0;
				*(shm+12)=0;
			}
		}	
		
		else if(x==2)			// entering the temp of the machine 1
		{
			int temp;
			printf("\nenter the temp. of the machine:\n temp=");
			scanf("%d",&temp);
			*(shm+10)=temp;
			fl = fopen("log.txt","a+");
			fprintf(fl,"\nthe temp of machine 1: %d\n",temp);
			fclose(fl);
				x=0;
		}
		
		else if(x==3)			// power failure
		{
			printf("power failure!!!!");
			printf("power failure!!!!");
			printf("power failure!!!!");
			fl = fopen("log.txt","a+");
			fprintf(fl,"\n%s\n","power failure!!!");
			fclose(fl);
			kill(id1,SIGKILL);
			kill(id2,SIGKILL);
			kill(id3,SIGKILL);
			kill(id4,SIGKILL);
			kill(pid,SIGKILL);
				x=0;
		}
		
		else if(x==4)
		{
			printf("\nmain process exiting\n");
			fl = fopen("log.txt","a+");
			mytime = time(NULL);
			char* tmp=ctime(&mytime);
			tmp=tmp+24;
			*tmp='\0';
			tmp=tmp-24;
			fprintf(fl,"\nat %s %s\n",tmp,"Main process exiting!!!!");
			fclose(fl);
			exit(5);
		}
		
		else
		{
		}
	}


	close(fd);
	close(fd2);
	close(fd3);
	close(fd4);
//		fclose(fl);
return(0);
}
