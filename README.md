#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#define producerNum  3
#define consumerNum  3
///生产者：3 消费者：3 库存：20 
#define sleeptime 1000
sem_t sem_produce;
sem_t sem_consume;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int buffer[21] = {0};//buffer[0]不用 
int sign = 1; 
void * consumer(void *consnumber){
    while(1){
    	sem_wait(&sem_consume);
    	pthread_mutex_lock(&mutex);	
		while(sign == 1){
    		printf("缓冲区为空！\n");
    		pthread_cond_wait(&cond, &mutex);
		} 	
		int temp = buffer[sign-1];
		buffer[sign-1] = 0;
		sign--;
		printf("消费者%d号消费一个产品,名为%d,缓冲区位置为%d\n",consnumber, temp, sign);
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_produce);
		usleep(sleeptime);
	}
}
void * producer(void *prodnumber){	
	int a = (int)prodnumber;
	while(1){
    	usleep(sleeptime);
		sem_wait(&sem_produce); 	
    	pthread_mutex_lock(&mutex);	
    	srand(a);
		int temp = rand()%1234;
    	while (sign == 21){
    		printf("缓冲区已满！\n");
    		pthread_cond_wait(&cond, &mutex);			
    	}
    	printf("生产者%d号生成一个产品,名为%d,缓冲区位置为%d\n",prodnumber,temp, sign);
    	buffer[sign] = temp;
		sign ++;
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_consume);
		usleep(sleeptime);
	}
}



int main()
{
    pthread_t threadPool[producerNum+consumerNum];
    int i;
    for(i = 0; i < producerNum; i++){
        pthread_t temp;
        pthread_create(&temp, NULL, producer, (void*)(i));
        threadPool[i] = temp;
    }//创建生产者进程放入线程池


    for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        pthread_create(&temp, NULL, consumer, (void*)(i));
        threadPool[i+producerNum] = temp;
    }//创建消费者进程放入线程池


    for(i = 0; i < producerNum+consumerNum; ++i){
        pthread_join(threadPool[i], NULL);
    }//阻塞 
}

