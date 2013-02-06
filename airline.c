/*
* Program: airline.c
*
* Requires:    
*              semaphore.h
*
* Description:
*
*              There are P passengers, B baggage handlers, S security screeners, and F flight attendants. 
*            Each passenger must give their baggage to a baggage handler, then go have their “security” screened by a security screener, and finally be seated by a flight attendant. 
*            When all P passengers have this done, the plane takes off.
*  		   
*
* To execute:
*
*        ./airline <p> <H> <S> <A>
*
*     <P> is the # of passengers.
*		  <H> is the # of baggage handlers.
*		  <S> is the # of security screeners.
*		  <A> is the # of flight attendants.
*
* Written by Li Jialong.
*/

#include <semaphore.h>
#include "cse421header.h"
int P,H,S,A;
int passenger_no,p_handler,p_screen,p_seat;
int count;
sem_t handler,screener,go_handler,go_screener,go_attendant,seated,M;
pthread_mutex_t mutex;//mutex

/*
* Function: Handler
*
* Description:
*              Show passenger is checking in his baggage.
* Parameters:
*              None.
*/
void *Handler(void *args){

    while(1)
	{
	  sem_wait(&go_handler);
	  printf("Passenger #%d is checking in his baggage.\n",++p_handler);
	  sem_post(&handler);
	}
}

/*
* Function: Screener
*
* Description:
*              Show passenger is being screened.
* Parameters:
*             None.
*/
void *Screener(void *args){

    while(1)
	{
	  sem_wait(&go_screener);
	  printf("Passenger #%d is being screened.\n",++p_screen);
	  sem_post(&screener);
	}
}

/*
* Function: Attendant
*
* Description:
*              Show passenger is being seated.
* Parameters:
*             None.
*/
void *Attendant(void *args){

    while(1)
	{
	  sem_wait(&go_attendant);
	  printf("Passenger #%d is being seated and relaxes.\n",++p_seat);
	  count++;
	  sem_post(&seated);
	  //when all passengers is seated,flight take off.
	  if(count==P)
	  {
	    sem_post(&M);
		break;
	  }
	  
	}
}


/*
* Function: Passenger
*
* Description:
*               Show passengers arrive at the terminal.
* Parameters:
*              None.
*/
void *Passenger(void *args){

    pthread_mutex_lock(&mutex);
   
	  passenger_no++;
	  printf("Passenger #%d arrives at the terminal.\n",passenger_no);
	
    pthread_mutex_unlock(&mutex);
	  sem_post(&go_handler);
	
    sem_wait(&handler);
	  sem_post(&go_screener);
	
	  sem_wait(&screener);
    sem_post(&go_attendant);
	
	  sem_wait(&seated);
	  pthread_exit(NULL);
}



/*
*
* Parameters:
*
* argc = 5.
* argv[1] is <P>.
* argv[2] is <H>.
* argv[3] is <S>.
* argv[4] is <A>.
* 
*/
int main(int argc,char *argv[])
{
   P = atoi(argv[1]);
   H = atoi(argv[2]);
   S = atoi(argv[3]);
   A = atoi(argv[4]);
   
   pthread_t passengers[P],handlers[H],screeners[S],attendants[A];
   int rc1,rc2,rc3,rc4;
   
   //initialize the semophore:handler,screener,go_handler,go_screener,go_attendant,seated
   sem_init(&handler,0,0);
   sem_init(&screener,0,0);
   sem_init(&go_handler,0,0);
   sem_init(&go_screener,0,0);
   sem_init(&go_attendant,0,0);
   sem_init(&seated,0,0);
   sem_init(&M,0,0);
   //initialize mutex.
   pthread_mutex_init(&mutex,NULL);
   
   for(int i=1;i<=H;i++)
   {
     rc1=pthread_create(&handlers[i],NULL,Handler,NULL); 
	 if(rc1)
	 {
	  printf("Error: return code from pthread_create() is %d\n",rc1);
	  exit(-1);
	 }
   }
   
   for(int i=1;i<=S;i++)
   {
     rc2=pthread_create(&screeners[i],NULL,Screener,NULL); 
	 if(rc2)
	 {
	  printf("Error: return code from pthread_create() is %d\n",rc2);
	  exit(-1);
	 }
   }
   
   for(int i=1;i<=A;i++)
   {
     rc3=pthread_create(&attendants[i],NULL,Attendant,NULL); 
	 if(rc3)
	 {
	  printf("Error: return code from pthread_create() is %d\n",rc3);
	  exit(-1);
	 }
   }
   
   //create passenger threads
   for(int i=1;i<=P;i++)
   {
     rc4=pthread_create(&passengers[i],NULL,Passenger,NULL); 
	 if(rc4)
	 {
	  printf("Error: return code from pthread_create() is %d\n",rc4);
	  exit(-1);
	 }
   }
    
   sem_wait(&M);
   printf("All passengers on board! Plane finally departs!!!\n");
   
   for(int i=1;i<=P;i++)
   {
    pthread_join(passengers[i],NULL);
   }
   
   //destroy semophore handler,screener,go_handler,go_screener,go_attendant,seated,M
   sem_destroy(&handler);
   sem_destroy(&screener);
   sem_destroy(&go_handler);
   sem_destroy(&go_screener);
   sem_destroy(&go_attendant);
   sem_destroy(&seated);
   sem_destroy(&M);
   //destroy mutex
   pthread_mutex_destroy(&mutex);
   
   return 1;
   exit(0);
}
