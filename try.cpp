#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

sem_t empty;
sem_t full;
sem_t chefzZerofull;
sem_t chefzZeroempty;
sem_t chefzOneempty;
sem_t chefzOnefull;

queue<int> q1;
queue<int> q2;
queue<int> q3;
bool queue2Full=false;
bool queue3Full=false;
pthread_mutex_t lock;
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lockprint;
pthread_mutex_t lock3;
pthread_mutex_t lock4;

int firstQueueSize=5;
int secondZeroQueueSize=5;
int secondOneQueueSize=5;

void init_semaphore()
{
	sem_init(&empty,0,firstQueueSize);
	sem_init(&full,0,0);
	sem_init(&chefzZerofull,0,0);
	sem_init(&chefzZeroempty,0,secondZeroQueueSize);
	sem_init(&chefzOnefull,0,0);
	sem_init(&chefzOneempty,0,secondOneQueueSize);	
	//sem_init(&chefz,0,1);
	pthread_mutex_init(&lock,0);
	pthread_mutex_init(&lock1,0);
	pthread_mutex_init(&lock2,0);
	pthread_mutex_init(&lockprint,0);
	pthread_mutex_init(&lock3,0);
	pthread_mutex_init(&lock4,0);
}
/*
void * ConsumerFunc(void *)
{
	while(true){
		
		if(q1.size()!=0 && q2.size()!=0 && q1.size()!=5 && q2.size()!=5){
			pthread_mutex_lock(&lock);
			int t=q2.front();
			q2.pop();
			//q2.push(t);
			printf("Consumer ate %d\n",t);
			pthread_mutex_unlock(&lock);		
		}
		
			
			
		
		
	}
}
*/
//single queue for chefZ
/*
void * ChefzPart(void *)
{
	while(true){
		
		if(q1.size()!=0 && q2.size()!=5){
			pthread_mutex_lock(&lock);
			int t=q1.front();
			q1.pop();
			q2.push(t);
			printf("ChefZ added %d\n",t);
			pthread_mutex_unlock(&lock);		
		}	
		else if(q2.size()==5){
			pthread_mutex_lock(&lock);
			printf("Chefz is waiting\n");
			
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		
	}
} */
/*
void * ChefzPart(void *)
{
	
	while(true){
		pthread_mutex_lock(&lock);
		
		//pthread_mutex_unlock(&lock);
		if(q1.front()==0){
			//pthread_mutex_lock(&lock);
			if(q1.size()!=0 && q2.size()!=5){
				//pthread_mutex_lock(&lock);
				//sleep(1);
				int t=q1.front();
				q1.pop();
				q2.push(t);
				printf("ChefZ added in Queue 2 %d\n",t);
				//pthread_mutex_unlock(&lock);			
			}	
			if(q2.size()==5 && !queue2Full){
				//pthread_mutex_lock(&lock);
				printf("Queue 2 is full\n");
				queue2Full=true;
				//return NULL;
				//pthread_mutex_unlock(&lock);
				
			}
			//pthread_mutex_unlock(&lock);	
		}
		else if(q1.front()==1){
			//pthread_mutex_lock(&lock);
			if(q1.size()!=0 && q3.size()!=5){
				//pthread_mutex_lock(&lock);
				sleep(1);
				int t=q1.front();
				q1.pop();
				q3.push(t);
				printf("ChefZ added in Queue 3 %d\n",t);
				//pthread_mutex_unlock(&lock);			
			}	
			if(q3.size()==5 && !queue3Full){
				//pthread_mutex_lock(&lock);
				printf("Queue 3 is full\n");
				queue3Full=true;
				//return NULL;
				//pthread_mutex_unlock(&lock);
				
			}
			//pthread_mutex_unlock(&lock);		
		}
		pthread_mutex_unlock(&lock);
		//sem_post(&chefy);
	}
}

void * ProducerFunc(void * t){
	int temp;
	temp=*(int*)t;
	free(t);
	while(true){
				
		sleep(1);
		if(q1.size()<5 )
		{	
			
			pthread_mutex_lock(&lock);
			q1.push(temp);
			printf("producer produced item %d\n",temp);
			pthread_mutex_unlock(&lock);
		}
		else if(q1.size()==5 && q2.size()==5 && q3.size()==5){
			
			pthread_mutex_lock(&lock);
			printf("They are in waiting...\n");
			pthread_mutex_unlock(&lock);
			return NULL;		
		}
		else{
			//break;
		}
		
	}

}
*/

void * WaiterOne(void *){
	int a,b,c;	
	while(true){
		
		sem_wait(&chefzZerofull);
		pthread_mutex_lock(&lock3);
		a=q1.size();
		c=q3.size();
		q2.pop();
		b=q2.size();
		pthread_mutex_lock(&lockprint);
		printf("Waiter1 take away a chocolate cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
		pthread_mutex_unlock(&lockprint);
		pthread_mutex_unlock(&lock3);
		sem_post(&chefzZeroempty);
		sleep(1);
		
	}	
}

void * WaiterTwo(void *){
	int a,b,c;
	while(true){
		
		sem_wait(&chefzOnefull);
		pthread_mutex_lock(&lock4);
		a=q1.size();
		b=q2.size();
		q3.pop();
		c=q3.size();
		pthread_mutex_lock(&lockprint);
		printf("Waiter2 take away a vanilla cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
		pthread_mutex_unlock(&lockprint);
		pthread_mutex_unlock(&lock4);
		sem_post(&chefzOneempty);
		sleep(1);
		
	}
}

void * ChefzPart(void *){
	int a,b,c;
	while(true){

		//sem_wait(&full);
		//sleep(1);
		//pthread_mutex_lock(&lock);

		if(q1.front()==0){
			sem_wait(&chefzZeroempty);
			sem_wait(&full);
			//sleep(1);
			pthread_mutex_lock(&lock1);
			c=q3.size();
			int t=q1.front();
			q1.pop();
			a=q1.size();
			q2.push(t);
			b=q2.size();
			pthread_mutex_lock(&lockprint);
			printf("chefZ pushed a chocolate cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
			pthread_mutex_unlock(&lockprint);
			pthread_mutex_unlock(&lock1);
			sem_post(&empty);
			sem_post(&chefzZerofull);
			
		}
		//pthread_mutex_unlock(&lock);
		//sem_post(&chefz);
		else if(q1.front()==1){
			sem_wait(&chefzOneempty);
			sem_wait(&full);
			//sleep(1);
			pthread_mutex_lock(&lock1);
			b=q2.size();
			int t=q1.front();
			q1.pop();
			a=q1.size();
			q3.push(t);
			c=q3.size();
			pthread_mutex_lock(&lockprint);
			printf("chefZ pushed a vanilla cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
			pthread_mutex_unlock(&lockprint);
			pthread_mutex_unlock(&lock1);
			sem_post(&empty);
			sem_post(&chefzOnefull);		
		}
		else{
			printf("first queue is empty");
		}
		sleep(1);
		//pthread_mutex_unlock(&lock);
			
	}
}

void * ProducerFunc2(void * t){
	
	int temp;
	temp=*(int*)t;
	free(t);
	int a,b,c;
	while(true){

		sem_wait(&empty);
		//sleep(1);
		pthread_mutex_lock(&lock);
		b=q2.size();
		c=q3.size();
		q1.push(temp);
		a=q1.size();
		//printf("\n");
		//pthread_mutex_lock(&lockprint);
		printf("chefY has made a vanilla cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
		//pthread_mutex_unlock(&lockprint);
		//printf("Queue size : %d\n",q1.size());
		//printf("\n");
		pthread_mutex_unlock(&lock);
		
		sem_post(&full);
		sleep(1);	
	}

}

void * ProducerFunc1(void * t){
	
	int temp;
	temp=*(int*)t;
	free(t);
	int a,b,c;
	while(true){

		sem_wait(&empty);
		//sleep(1);
		pthread_mutex_lock(&lock);
		b=q2.size();
		c=q3.size();
		q1.push(temp);
		a=q1.size();
		//printf("\n");
		//pthread_mutex_lock(&lockprint);
		printf("chefX has made a chocolate cake\nQueue1 size : %d\nQueue2 size : %d\nQueue3 size : %d\n\n",a,b,c);
		//pthread_mutex_unlock(&lockprint);
		//printf("Queue size : %d\n",q1.size());
		//printf("\n");
		pthread_mutex_unlock(&lock);
		
		sem_post(&full);
		sleep(1);	
	}

}


int main()
{
	//printf("ok");
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;
	
	init_semaphore();
	int *val1 = malloc(sizeof(*val1));
	*val1 = 0;
	int *val2 = malloc(sizeof(*val2));
	*val2 = 1;
	//int t;
	//sem_getvalue(&empty, &t);
	//printf("%d",t);
	pthread_create(&thread1,NULL,ProducerFunc1,val1);

	pthread_create(&thread2,NULL,ProducerFunc2,val2);
	
	pthread_create(&thread3,NULL,ChefzPart,NULL);
	sleep(5);
	pthread_create(&thread4,NULL,WaiterOne,NULL);
	pthread_create(&thread5,NULL,WaiterTwo,NULL);
	
	

	
	
	pthread_exit(NULL); 
	return 0;
}
