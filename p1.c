#include<stdio.h>
#include "header.h"
#include<pthread.h>
pid_t mainpid,id2;
int main()
{
	int iron,y=0,fbody=0;


	pid_t pid,mypid;
	mypid=getpid();
	int fdr,fd,fd2;

	FILE *fl;
	//fl = fopen("log.txt","a+");

		time_t mytime;					// for entering time in log file
		fl = fopen("log.txt","a+");
                mytime = time(NULL);
		char* tmp=ctime(&mytime);
		tmp=tmp+24;
		*tmp='\0';
		tmp=tmp-24;
		fprintf(fl,"\nat %s time first process has started\n",tmp);
		fclose(fl);

	// for shared memory
		int *shm;
		int shmid;
		key_t key=5;	
		if((shmid=shmget(key,(15*sizeof(int)),IPC_CREAT|0666))<0)
		{
		perror("shmget");
		exit(1);
		}
		if((shm=shmat(shmid,NULL,0))==(int *)-1)
		{
		perror("shmat");
		exit(1);
		}


	// for passing pid's

        if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo1.\n");
                exit(5);
        }
	write(fd,&mypid,4);						// writing own pid to pass it to main process
	printf("\nmypid is %d \n", mypid);
	sleep(40);
	read(fd,&mainpid,4);						// reading process id of main process
	printf("\npid of main process is %d \n",mainpid);
	close(fd);

	if((fd2=open("/home/ayush/Desktop/fifo2",O_RDWR|O_NONBLOCK)) < 0)		//this for getting process id of 2nd process 
	        {
	                printf("fatal error in opening fifo2.\n");
	                exit(5);
	        }
		read(fd2,&id2,4);
//		printf("\n%d\n",id2);

//		write(fd2,mypid,4);
		close(fd2);



	void *input()					// input function which will be keeping a check on raw materials
	{	
		
		iron=*shm;
		
		if(iron<200)
		{
			printf("\nnot enough iron\n");
			printf("%d",iron);
			

			printf("\n1 --> want to stop the process\n2 --> want to add raw material\n");
			printf("choose:");
			scanf("%d",&y);
			
			if(y==1)
			{
				printf("\ntotal iron body created: %d\n",fbody);
				if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
		        	{
		        	        printf("fatal error in opening fifo1.\n");
		        	        exit(5);
		        	}
				write(fd,&mypid,4);
				close(fd);
				fl = fopen("log.txt","a+");
				fprintf(fl,"\n%s\n","process 1 sending signal to main process to kill itself");
				fprintf(fl,"\ntotal iron body created: %d\n",fbody);
				fclose(fl);
	                       	kill(mainpid, SIGINT);
			}
			
			if(y==2)
			{

		// critical section

				if(*(shm+12)==1)
				{
					printf("shared memory is being used");
					while(1)
					{
						if(*(shm+12)==0)
						break;
					}
				}

				if(*(shm+12)==0)
				{
					*(shm+12)=1;
					printf("\nenter the amount of iron:");
					scanf("%d",&iron);
					*(shm)=iron;
					fl = fopen("log.txt","a+");
					fprintf(fl,"\nin process 1 updating value of iron= %d \n",iron);
					fclose(fl);
					*(shm+12)=0;
				}
			}		
			
		}	
		
	}



	void *output()
	{
		if(*shm>0)
		{
		*shm = *shm-200;
//		printf("\n iron:%d",iron);
		sleep(2);
		kill(id2,SIGUSR1);			//sending signal to next process
		fbody++;
		printf("\nthe body is created\n");	
		printf("\nnow sending the body for paint\n");
		fl = fopen("log.txt","a+");
		fprintf(fl,"\n%d iron body is created\n",fbody);
		fprintf(fl,"\n%s\n","process 1 sending signal to process 2 that they are sending iron body for paint");
		fclose(fl);
		printf("\niron left %d\n", *shm);
		}
//		send signal to next process;
	
	}




	void *frommain()				// function to keep check on the temp of the machine
	{
		int temp = *(shm+10);
		//printf("\ntemp=%d\n",temp);
		if(temp>1400)
		{
			printf("\ntemperature of machine is critical\n");
			printf("\nexiting the process\n");
			fl = fopen("log.txt","a+");
			fprintf(fl,"\n%s","temp. has rised above critical temp. so shutting down all processes\n");
			fclose(fl);
			printf("\ntotal iron body created: %d\n",fbody);
			kill(mainpid,SIGUSR1);
			*(shm+10)=0;
		}
	}
	

	while(1)				// threads used to implement the input,output,frommain function infinitely
	{
	pthread_t tid1,tid2,tid3;
	pthread_create(&tid1,NULL,input,NULL);	
	pthread_join(tid1,NULL);	
	pthread_create(&tid2,NULL,output,NULL);
	pthread_create(&tid3,NULL,frommain,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	
	sleep(5);
	}
	//fclose(fl);
	return(0);

}
