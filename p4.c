#include<stdio.h>
#include "header.h"

int main()
{
	int fd4,count=0,y=0,t=0;
	int tyre,bodyparts,paint,glasses,compcar=0;

	pid_t pid,mypid,mainpid;
	mypid=getpid();
	int fdr,fd;

	FILE *fl;

		time_t mytime;
		fl = fopen("log.txt","a+");
                mytime = time(NULL);
		char* tmp=ctime(&mytime);
		tmp=tmp+24;
		*tmp='\0';
		tmp=tmp-24;
		fprintf(fl,"\nat %s time fourth process has started\n",tmp);
		fclose(fl);


		int signo;
	static struct sigaction act;
	void catchint(int signo)
	{
		//printf("\nhello\n");
		if(signo==SIGUSR1)
		{
			count++;
		//	printf("\ncount:%d\n",count);		
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
	//printf("\n%d\n",*(shm+11));
	count = *(shm+11);

	if((fd4=open("/home/ayush/Desktop/fifo4",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo4.\n");
                exit(5);
        }
	write(fd4,&mypid,4);
	printf("mypid is %d \n", mypid);
	sleep(35);
	read(fd4,&mainpid,4);
	printf("pid of main process is %d \n",mainpid);

	write(fd4,&mypid,4);
	close(fd4);
	sleep(10);

	void *input()
	{
		int tyre,ackit,furnkit,engine;

		tyre=*(shm+5);
		engine=*(shm+6);
		ackit=*(shm+7);
		furnkit=*(shm+8);

		if(tyre<4 || engine<1 || ackit<1 || furnkit<1)
		{
			if(tyre<4)
			{
				printf("\nnot enough tyre\n");
			}

			if(engine<1)
			{
				printf("\nno engine\n");
			}

			if(ackit<1)
			{
				printf("\nnot enough ackit\n");
			}


			if(furnkit<1)
			{
				printf("\nnot enough furnkit\n");
			}

			printf("\n1 --> want to stop the process\n2 --> want to add raw material\n");
			printf("choose:");
			scanf("%d",&y);
			
			if(y==1)
			{
				printf("\ntotal completed car created: %d\n", compcar);
				if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
		        	{
		        	        printf("fatal error in opening fifo1.\n");
		        	        exit(5);
		        	}
				write(fd,&mypid,4);
				close(fd);
				sleep(2);

				fl = fopen("log.txt","a+");
				fprintf(fl,"\n%s\n","fourth process is sending signal to main process to kill itself");
				fprintf(fl,"\ntotal car finished: %d\n", compcar);
				fclose(fl);

	                       	kill(mainpid, SIGINT);
				*(shm+11)=count;
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
					printf("\n%d",*(shm+12));
					if(tyre<4)
					{
						printf("\nenter the number of tyre:");
						scanf("%d",&tyre);
						*(shm+5)=tyre;

						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of tyre:%d\n",tyre);
						fclose(fl);
					}
		
					if(engine<1)
					{
						printf("\nenter the number of engine:");
						scanf("%d",&engine);
						*(shm+6)=engine;

						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of engine:%d\n",engine);
						fclose(fl);
					}
		
					if(ackit<1)
					{
						printf("\nenter the number of ackit:");
						scanf("%d",&ackit);
						*(shm+7)=ackit;
						
						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of ackit:%d\n",ackit);
						fclose(fl);
					}
		
		
					if(furnkit<1)
					{
						printf("\nenter the number of furniture kit:");
						scanf("%d",&furnkit);
						*(shm+8)=furnkit;

						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of furniture kit:%d\n",furnkit);
						fclose(fl);
					}
					*(shm+12)=0;
				}
			}		

		}

		else if(count<1)
		{
			sleep(2);
			printf("\nNo Body available for assembling\n");

			printf("want to exit enter the value 2\n");
			scanf("%d",&t);
			if(t==2)
			{
				printf("\ntotal completed car created: %d\n", compcar);
				exit(1);
			}
			//sleep(5);
		}
	
	}

	

	void *output()
	{
		sleep(2);
		//printf("\ncount: %d", count);
		if(count>0)
		{
			printf("\nCar ready to go\n");
			compcar++;
			*(shm+5)=*(shm+5)-4;
			*(shm+6)=*(shm+6)-1;
			*(shm+7)=*(shm+7)-1;
			*(shm+8)=*(shm+8)-1;

			printf("\nnumber of tyre left:%d",*(shm+5));
			printf("\nnumber of engine left:%d",*(shm+6));
			printf("\nnumber of ackit left:%d",*(shm+7));
			printf("\nnumber of furnkit left:%d\n",*(shm+8));

			fl = fopen("log.txt","a+");
			
			mytime = time(NULL);
			char* tmp=ctime(&mytime);
			tmp=tmp+24;
			*tmp='\0';
			tmp=tmp-24;
	
			fprintf(fl,"\n%d comlete car is created at %s\n",compcar,tmp);
			fclose(fl);
			count--;
			
		}

	}

	while(1)
	{
		pthread_t tid1,tid2;
		pthread_create(&tid1,NULL,input,NULL);	
		pthread_join(tid1,NULL);	
		pthread_create(&tid2,NULL,output,NULL);
		pthread_join(tid2,NULL);
		sleep(10);
		//printf("")
	}

}
