#include<stdio.h>
#include "header.h"

int main()
{
	int fd2,fd3;		// count is calculating the number of times process 1 has called
	int tyre,bodyparts,paint,glasses,painted=0,y=0,t=0;
	static int count=0;


	pid_t pid,mypid,mainpid,id3;
	mypid=getpid();
	int fdr,fd;
		
	FILE *fl;
//	fl = fopen("log.txt","a+");	

		time_t mytime;
		fl = fopen("log.txt","a+");
                mytime = time(NULL);
		char* tmp=ctime(&mytime);
		tmp=tmp+24;
		*tmp='\0';
		tmp=tmp-24;
		fprintf(fl,"\nat %s time second process has started\n",tmp);
		fclose(fl);

	int signo;
	static struct sigaction act;
	void catchint(int signo)
	{
		//printf("\nhello\n");
		if(signo==SIGUSR1)
		{
			fl = fopen("log.txt","a+");
			fprintf(fl,"\n%s\n","signal is received in process 2 that iron body has received");
			fclose(fl);
			//fprintf(fl,"")
			count++;
			//printf("\ncount:%d\n",count);		
		}


	}

	sigfillset(&(act.sa_mask));
	act.sa_handler=catchint;
	sigaction(SIGUSR1, &act, NULL);




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

	if((fd2=open("/home/ayush/Desktop/fifo2",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo2.\n");
                exit(5);
        }
	write(fd2,&mypid,4);
	printf("\nmypid is %d \n", mypid);
	sleep(30);
	read(fd2,&mainpid,4);
	printf("\npid of main process is %d \n",mainpid);

	write(fd2,&mypid,4);
	close(fd2);

//	printf("\n%d\n",*(shm+9));
	count = *(shm+9);


	if((fd3=open("/home/ayush/Desktop/fifo3",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo3.\n");
                exit(5);
        }
	sleep(5);
	read(fd3,&id3,4);
//	printf("\nid3 = %d\n", id3);
	close(fd3);

	sleep(5);



	void *input()
	{
		int paint;
		paint=*(shm+4);
		//printf("\ninput\n");
		if(paint<20)
		{
			//printf("\nhello\n");
			printf("\nNot enough paint\n");

			

			printf("\n1 --> want to stop the process\n2 --> want to add raw material\n");
			printf("choose:");
			scanf("%d",&y);
			
			if(y==1)
			{
				printf("\ntotal painted body created: %d\n",painted);
				if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
		        	{
		        	        printf("fatal error in opening fifo1.\n");
		        	        exit(5);
		        	}
				write(fd,&mypid,4);
				close(fd);
				sleep(2);
				fl = fopen("log.txt","a+");
				fprintf(fl,"\n%s\n","process 2 sending signal to main process to kill itself");
				fprintf(fl,"\ntotal painted body created: %d\n",painted);
				fclose(fl);
	                       	kill(mainpid, SIGINT);
				*(shm+9)=count;
			}
			
			if(y==2)
			{
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
				printf("\nenter the number of cans of paint:");
				scanf("%d",&paint);
				*(shm+4)=paint;
				fl = fopen("log.txt","a+");
				fprintf(fl,"\nupdating the value of no. of paint cans to %d\n",paint);
				fclose(fl);
				}
			}		
			
		}

		
		else if(count<1)
		{
	
			printf("\nNo Body available\n");
			//sleep(2);
			printf("want to exit enter the value 2\n");
			scanf("%d",&t);
			if(t==2)
			{
				printf("\ntotal painted body created: %d\n",painted);
				exit(1);
			}
		}		
	}




	void *output()
	{
		sleep(5);
		//count--;
		if(count>0)
		{
		printf("\nthe body is painted\n");
		painted++;
		*(shm+4)=*(shm+4)-20;
		printf("\nSending the painted body for mirror factory. \n");
		printf("\nCans of paint left: %d\n",*(shm+4));
		fl = fopen("log.txt","a+");
		fprintf(fl,"\n%d painted body is created\n",painted);
		fprintf(fl,"\n%s","process 2 sending signal to process 3 that they are sending painted body to glass factory\n");
		fclose(fl);
		count--;
		kill(id3,SIGUSR1);
		
		//printf("\noutput count:%d\n",count);
		}

	}
		

	while(1)
	{
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,input,NULL);	
	pthread_join(tid1,NULL);	
	pthread_create(&tid2,NULL,output,NULL);
	pthread_join(tid2,NULL);
	sleep(5);
	}
	
fclose(fl);

}
