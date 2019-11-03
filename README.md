#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
void *runner(void *param);
pthread_t tid[3];
pthread_attr_t attr[3];
void *ptr;//内存地址
void *ptr0;//内存数据地址
int zhic;//传入指数
int shm_fd;//共享内存
pthread_mutex_t mutex2;
const char *name2 = "empty";
const char *name1 = "full";
static sem_t *empty = NULL;
static sem_t *full = NULL;
pid_t gettid() {
	return syscall(SYS_gettid);
}
double randomExponential(double lambda,int seed)//负指数数产生
{
	srand(seed);
	double pv=0.0;
	pv=(double)(rand()%100)/100;
	while(pv==0)
	{
		pv=(double)(rand()%100)/100;
	}
	pv=(-1/lambda)*log(1-pv);
	return pv;
}
//------

//-------
int main(int argc, char **argv)
{
	const int Size=20;
	const char* name="OS";
	int i;
	
	char* ls;

	zhic=atoi(argv[1]);//获取参数
	//共享数据内存
	shm_fd=shm_open(name,O_CREAT|O_RDWR,0666);
	ptr0=mmap(0,Size,PROT_WRITE,MAP_SHARED,shm_fd,0);
	ptr=ptr0+1;	//第一位内存保存当前队列长度，之后20位保存数据，所以ptr为原地址加一
	//互斥锁
	pthread_mutex_init(&mutex2, NULL);
    empty = sem_open(name2, O_RDWR | O_CREAT, 00777, 20);
    full = sem_open(name1, O_RDWR | O_CREAT, 00777, 0);

	for (i=0;i<3;i++)
	{
		switch(i)
		{
			case 0:
				ls="5";
				break;
			case 1:
				ls="6";
				break;
			case 2:
				ls="7";
				break;	
		}
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runner,ls);
	}
	while(1);

	shm_unlink(name);
	return 0;
}

void *runner(void *param)
{
	int number = atoi(param);
	char len;

	float t;
	pid_t pid = gettid();//获取ID
	
	srand(number);
	printf("run\n");
	while(1)
	{
		len=((char*)ptr0)[0];
		if(len>1)
		{
			
			sem_wait(full);//等待锁满
			pthread_mutex_lock(&mutex2);//上锁
			//-----
			
			printf("\n");
			ptr=ptr0+len;//得到当前长度
			printf("the %d cons's %d pick %d",number-4,pid,((char*)ptr)[0]);
			sprintf(ptr0,"%c",len-1);//重新写入长度
			//-----
			
			sem_post(empty);//更新信号量
			pthread_mutex_unlock(&mutex2);//上锁
			t=randomExponential(zhic,number);//睡眠
			sleep(t);
		}
	}

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
void *runner(void *param);
pthread_t tid[3];
pthread_attr_t attr[3];
int len=0;//队列长度
int shm_fd;//共享内存
void *ptr0;//内存地址
void *ptr;//内存数据地址
int zhip;//传入指数
pthread_mutex_t mutex1;//定义锁
const char *name2 = "empty";
const char *name1 = "full";
static sem_t *empty = NULL;
static sem_t *full = NULL;

pid_t gettid() {
	return syscall(SYS_gettid);
}
double randomExponential(double lambda,int seed)//负指数数产生
{
	srand(seed);
	double pv=0.0;
	pv=(double)(rand()%100)/100;
	while(pv==0)
	{
		pv=(double)(rand()%100)/100;
	}
	pv=(-1/lambda)*log(1-pv);
	return pv;
}
int main(int argc, char **argv)
{
	
	int i=0;
	const int Size=21;
	const char* name="OS";
	
	char* ls;
	//sem_init();
	zhip=atoi(argv[1]);//获取参数
	//数据内存
	shm_fd=shm_open(name,O_CREAT|O_RDWR,0666);
	ftruncate(shm_fd,Size);
	ptr0=mmap(0,Size,PROT_WRITE,MAP_SHARED,shm_fd,0);
	ptr=ptr0+1;//第一位内存保存当前队列长度，之后20位保存数据，所以ptr为原地址加一
	sprintf(ptr0,"%c",0);//写入当前队列长度0
	//互斥锁
	pthread_mutex_init(&mutex1, NULL);
    empty = sem_open(name2, O_RDWR | O_CREAT, 00777, 20);
    full = sem_open(name1, O_RDWR | O_CREAT, 00777, 0);

	
	for (i=0;i<3;i++)
	{
		switch(i)
		{
			case 0:
				ls="0";
				break;
			case 1:
				ls="1";
				break;
			case 2:
				ls="2";
				break;	
		}
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runner,ls);
	}
	while(1);
	return 0;
}
void *runner(void *param)
{

	
	int number = atoi(param);
	int temp;
	char  c;//随机产生的的数据
	float t;//用于睡眠时间
	void*ptrm = ptr;//缓冲队列头部
	char len;//缓冲队列长度
	pid_t pid = gettid();
	srand(number+7);
	while(1)
	{
		
		ptrm = ptr;
		if(((char*)ptr0)[0]<20)
		{
			
		
		sem_wait(empty);//等待锁空
        pthread_mutex_lock(&mutex1);//上锁
        srand(rand() % 127 + 1);
		len=((char*)ptr0)[0];//得到当前长度
		sprintf(ptr0,"%c",len+1);//重新写入长度
		if (len==0)
		{
			len=len+1;
		}
		c=rand() % 127 + 1;//产生随机数
		ptrm+=(int)len;//得到队列尾部
		sprintf(ptrm,"%c",c);//写入队列尾部
		printf("ls %d the %d prod's %d put %d\n",len,number+1,pid,c);
		sem_post(full);//更新信号量
        pthread_mutex_unlock(&mutex1);//解锁
		t=randomExponential(zhip,number);//睡眠
		sleep(t);
		}

	}
	
}
