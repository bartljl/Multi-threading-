/*
* Program: MH.c
*
* Requires:    
*              semaphore.h
*
* Description:
*
*              Each child needs to be woken up, fed breakfast, sent to school, given dinner, given a bath.
               Mom applies the tasks in order and then let the father read the child a book and tuck the child in bed.
  		   
*
* To execute:
*
*        ./MH <N>
*
*         <N> is the number of the day.
*
* Written by Li Jialong.
*/

#include <semaphore.h>


pthread_mutex_t mutex;
sem_t mom,dad;
int cycles;
int day;

/*
* Function: Mother
*
* Description:
*
*            Woken up, fed breakfast, sent to school, given dinner, given a bath
*            to children.          
*
* Parameters:
*
*             args_1.
*
*/
void *Mother(void *args_1){

   for(day=1;day<=cycles;day++)
 {
	sem_wait(&mom);
	printf("Mother Hubbard sighs and begins Day %d.\n",day);
	for(int i=1;i<=12;i++)
	{
		printf("Mother is waking child #%d.\n",i);
	}
	for(int i=1;i<=12;i++)
	{
		printf("Mother is feeding child #%d.\n",i);
	}
	for(int i=1;i<=12;i++)
	{
		printf("Mother sent child #%d to school.\n",i);
	}
	for(int i=1;i<=12;i++)
	{
		printf("Mother is giving child #%d dinner.\n",i);
	}
	for(int j=1;j<=12;j++)
	{
		printf("Mother has bathed child #%d.\n",j);
		usleep(100);
		sem_post(&dad);
	}
	
	printf("Mother is taking a nap break.\n");

 }
}

/*
* Function: Father
*
* Description:
*
*            Read the child a book and tuck the child in bed.
*
* Parameters:
*
*             args_2.
*
*/
void *Father(void *args_2){
 
 while(day<=cycles)
  {
	for(int j=1;j<=12;j++)
	{
	  sem_wait(&dad);
   	  printf("Father is reading a book to child #%d.\n",j);
	}
	for(int j=1;j<=12;j++)
	{
   	  printf("Father is tucking in child #%d.\n",j);
        }
	printf("Father wakes Mother and then goes to sleep.\n");
	sem_post(&mom);
  }
}

/*
*
* Parameters:
*
* argc = 2.
* argv[1] is <N>.
*
*/
int main(int argc,char *argv[])
{
	pthread_t p1,p2;
	int rc1,rc2;
 
    cycles = atoi(argv[1]);
	sem_init(&mom,0,1);
	sem_init(&dad,0,0);
	
	rc1=pthread_create(&p1,NULL,Mother,NULL);
	if(rc1)
	{
	printf("ERROR; return code from pthread_create(p1) is %d\n", rc1);
	exit(-1);
	}
 
	rc2=pthread_create(&p2,NULL,Father,NULL);
	if(rc1)
	{
	printf("ERROR; return code from pthread_create(p2) is %d\n", rc2);
	exit(-1);
	}
	
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	
	exit(0);
 
}
