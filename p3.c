#include<stdio.h>
#include "header.h"

int main()
{
	int fd3,count=0,assembly=0,t=0;
	int sideglass,rearglass,frontglass,y=0;

	pid_t pid,mypid,mainpid,id4;
	mypid=getpid();
	int fdr,fd,fd4;

	FILE *fl;	
	
		time_t mytime;
		fl = fopen("log.txt","a+");
                mytime = time(NULL);
		char* tmp=ctime(&mytime);
		tmp=tmp+24;
		*tmp='\0';
		tmp=tmp-24;
		fprintf(fl,"\nat %s time third process has started\n",tmp);
		fclose(fl);	


	int signo;
	static struct sigaction act;
	void catchint(int signo)
	{
		//printf("\nhello\n");
		if(signo==SIGUSR1)
		{
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
	//printf("\n%d\n",*(shm+10));
		count = *(shm+10);


	if((fd3=open("/home/ayush/Desktop/fifo3",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo3.\n");
                exit(5);
        }
	write(fd3,&mypid,4);
	printf("mypid is %d \n", mypid);
	sleep(30);
	read(fd3,&mainpid,4);
	printf("pid of main process is %d \n",mainpid);

	write(fd3,&mypid,4);
	close(fd3);

	if((fd4=open("/home/ayush/Desktop/fifo4",O_RDWR|O_NONBLOCK)) < 0)
        {
                printf("fatal error in opening fifo4.\n");
                exit(5);
        }
	sleep(15);
	read(fd4,&id4,4);
//	printf("\nid4 = %d\n", id4);
	close(fd4);


	void *input()
	{
		rearglass=*(shm+1);
		frontglass=*(shm+2);
		sideglass=*(shm+3);

		if(rearglass<1 || frontglass<1 || sideglass<4)
		{
			if(rearglass<1)
			{
				printf("\nnot enough rearglass\n");
			}

			if(frontglass<1)
			{
				printf("\nnot enough frontglass\n");
			}

			if(sideglass<4)
			{
				printf("\nnot enough sideglass\n");
			}


			printf("\n1 --> want to stop the process\n2 --> want to add raw material\n");
			printf("choose:");
			scanf("%d",&y);
			
			if(y==1)
			{
				printf("\ntotal car for assembly created: %d\n", assembly);
				if((fd=open("/home/ayush/Desktop/fifo1",O_RDWR|O_NONBLOCK)) < 0)
		        	{
		        	        printf("fatal error in opening fifo1.\n");
		        	        exit(5);
		        	}
				write(fd,&mypid,4);
				close(fd);
				sleep(2);
	                       	kill(mainpid, SIGINT);
				fl = fopen("log.txt","a+");
				fprintf(fl,"\n%s\n","process three is sending signal to main process to kill itself");
				fprintf(fl,"\ntotal car for assembly created: %d\n", assembly);
				fclose(fl);
				*(shm+10)=count;
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
					if(rearglass<1)
					{
						printf("\nenter the number of rearglass:");
						scanf("%d",&rearglass);
						*(shm+1)=rearglass;
					
						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of rearglass:%d\n",rearglass);
						fclose(fl);
						
					}
		
					if(frontglass<1)
					{
						printf("\nenter the number of cans of frontglass:");
						scanf("%d",&frontglass);
						*(shm+2)=frontglass;
				
						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of frontglass:%d\n",frontglass);
						fclose(fl);
					}
		
					if(sideglass<4)
					{
						printf("\nenter the number of cans of sideglass:");
						scanf("%d",&sideglass);
						*(shm+3)=sideglass;

						fl = fopen("log.txt","a+");
						fprintf(fl,"\nupdating the value of sideglass:%d\n",sideglass);
						fclose(fl);
					}
					*(shm+12)=0;
				}	
			}		

		
		}

		else if(count<1)
		{
			sleep(2);
			printf("\nNo Painted Body available\n");

			printf("want to exit enter the value 2\n");
			scanf("%d",&t);
			if(t==2)
			{
				printf("\ntotal car for assembly created: %d\n", assembly);
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
			printf("\nsending car for assembling\n");
			assembly++;
			*(shm+1)=*(shm+1)-1;
			*(shm+2)=*(shm+2)-1;
			*(shm+3)=*(shm+3)-4;

			printf("\nnumber of rearglass left:%d",*(shm+1));
			printf("\nnumber of frontglass left:%d",*(shm+2));
			printf("\nnumber of sideglass left:%d\n",*(shm+3));
			
			fl = fopen("log.txt","a+");
			fprintf(fl,"\n%d body glasses are fit\n",assembly);
			fprintf(fl,"\n%s","process 3 sending signal to process 4 that they are sending  body for assembling\n");
			fclose(fl);
			
			count--;
			kill(id4,SIGUSR1);
			
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
