/*
* Program: BB.c
*
* Requires:    
*              semaphore.h
*
* Description:
*
*                The producer generates printable characters and places them into the buffer and 
*            the consumer pulls the characters out of the buffer one at a time and prints them out.
*
* To execute:
*
*     ./BB <P> <C> <BufferSize>.
*
*     <P> is the # of threads to run the producer function on.
*     <C> is the # of threads to run the consumer function on.
*     <BufferSize> is the size of the bounded buffer.
*
*
* Written by Li Jialong 
*/
#include <semaphore.h>
#include "cse421header.h"

pthread_mutex_t mutex_1,mutex_2;//mutex
sem_t bufsize,avail,M;// semophore

int i=0,j=0;
int out_char_num=0,count=0;;// the total printed char number and the current number of characters.
int P,C,BufferSize;
int producer_id,consumer_id;//# of producer and customer.
char Buffer[1000000];


/*
* Function: producer_function
*
* Description:
*
*             Produce a charater and put into buffer.
*
* Parameters:
*
*             none
*
*/
void *producer_function(void *p){
 
  int id=++producer_id;
	
    while(1)
   {
      
      if(count==BufferSize) 
	  {
	  printf("Buffer is full.Producers wait. \n");
	  //block the producer
	  sem_init(&bufsize,0,0);
	  }  
	  
      sem_wait(&bufsize);
	  //lock the buffer
	  pthread_mutex_lock(&mutex_1);
	  
	  //random produce characters:'A'-'Z'
	  char c=(random()%26)+65;
	  Buffer[i]=c;
	  i=(i+1)%BufferSize;
	  out_char_num++;
	  count++;
	  printf("Producer #%d writes %c\n",id,c);
	  
	  //unlock the buffer
	  pthread_mutex_unlock(&mutex_1);
	  usleep(100);
	  sem_post(&avail);
	  
      //if characters are up to 1000,exit.	
	  if(out_char_num==1000) 
	  {
	   sem_post(&M);
	   break;
	  }
	  
   } 
    

}

/*
* Function: customer_function
*
* Description:
*
*             Read one charater and print it out.
*
* Parameters:
*
*             none
*
*/  
void *consumer_function(void *p){
  
	int id=++consumer_id;
	
	while(1)
 {  
    if(count==0)
	{
	  printf("Buffer is empty.Consumers wait. \n");
	  //block the consumer
	  sem_init(&avail,0,0);
	}
	
	sem_wait(&avail);
	//lock the buffer
	pthread_mutex_lock(&mutex_2);
	
	printf("Customer #%d reads %c\n",id,Buffer[j]);
	j=(j+1)%BufferSize;
	count--;
	//unlock the buffer
	
	pthread_mutex_unlock(&mutex_2);
	usleep(100);
	sem_post(&bufsize);
	
  }
   
}


/*
*
* Parameters:
*
* argc = 4.
* argv[1] is <P>.
* argv[2] is <C>.
* argv[3] is <BufferSize>.
*/
int main(int argc, char *argv[])
{
    P=atoi(argv[1]);
    C=atoi(argv[2]);
    BufferSize=atoi(argv[3]);
    pthread_t thread_producer[P],thread_consumer[C];
	int rc1,rc2;
	
	//output display
	printf("Bounded Buffer = %d\n",BufferSize);
	printf("%d Producer / %d Consumer \n",P,C);
	printf("Begin run:\n");
	printf("----------------\n");
	
    //initialize semaphore
    sem_init(&bufsize,0,BufferSize);
    sem_init(&avail,0,0);	
	sem_init(&M,0,0);
	
	//initialize mutex
	pthread_mutex_init(&mutex_1,NULL);
	pthread_mutex_init(&mutex_2,NULL);
	
    //create producer thread
	 for(int m = 1; m <= P; m++)
     {
	  rc1=pthread_create(&thread_producer[m],NULL,producer_function,NULL);
	  if(rc1)
	  {
	   printf("ERROR; return code from pthread_create() is %d\n", rc1);
	   exit(-1);
	  }
	 }
	 //create customer thread
	 for(int n = 1; n <= C; n++)
     {
	  rc2=pthread_create(&thread_consumer[n],NULL,consumer_function,NULL);
	  if(rc2)
	  {
	   printf("ERROR; return code from pthread_create() is %d\n", rc2);
	   exit(-1);
	  }
	 }
	
	 sem_wait(&M);
	 
	 //destroy semaphore
	 sem_destroy(&bufsize);
	 sem_destroy(&avail);
	 pthread_mutex_destroy(&mutex_1);
	 pthread_mutex_destroy(&mutex_2);
	
	 return 1;
	 exit(0);
}
