#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 10 
#define PNUM 3
#define CNUM 2
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int buffer[N] = { 0 };
int producer_index = 0;
int consumer_index = 0;
int sum = 0;
int producer_id = 0;
int consumer_id = 0;
void *produce() {
	int pid = ++producer_id;
	while(1) {
		sleep(1);
		if(sum == 50){
			printf("producer%d terminate.\n",pid);
			return 0;
		}
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		producer_index = producer_index % N;
		buffer[producer_index] = 1;
		printf("producer%d produced an item at buffer[%d].\n",pid,producer_index);
		producer_index++;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}
void *consume() {
	int cid = ++consumer_id;
	while(1) {
		sleep(1);
		if(sum == 50){
			printf("consumer%d terminate.\n",cid);
			return 0;
		}
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		consumer_index = consumer_index % N;
		buffer[consumer_index] = 0;
		printf("consumer%d consumed an item at buffer[%d].\n",cid,consumer_index);
		consumer_index++;
		sum++;
		printf("sum: %d\n",sum);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}
int main() {
	int i;
	pthread_t producer[PNUM];
	pthread_t consumer[CNUM];
	sem_init(&empty,0,N);
	sem_init(&full,0,0);
	pthread_mutex_init(&mutex,NULL);
	for(i=0;i<PNUM;i++)
		pthread_create(&producer[i],NULL,produce,&i);
	for(i=0;i<CNUM;i++)
		pthread_create(&consumer[i],NULL,consume,&i);	
	for(i=0;i<PNUM;i++)
		pthread_join(producer[i],NULL);
	for(i=0;i<CNUM;i++)
		pthread_join(consumer[i],NULL);	
	printf("terminate\n");
	return 0;
}
