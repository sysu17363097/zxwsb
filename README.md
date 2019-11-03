# zxwsb
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//~ int sum=0;
void *runner(void *param);
enum condition{thinking,hungry,eating};//定义哲学家的状态 
enum chopstickscondition{leisure,occupied};//定义筷子的状态 
enum condition pcon[5];//5位哲学家状态记录 
enum chopstickscondition ccon[5];//5根筷子 

int main(int argc, char **argv)
{
	int i;
	pthread_t tid[5];
	pthread_attr_t attr[5];
	char *ls;

	//~ pthread_attr_init(&attr[0]);
	//~ pthread_create(&tid[0],&attr[0],runner,argv[1]);
	//~ pthread_join(tid[0],NULL);
	printf("%d",pcon[2]);
	//~ pthread_attr_init(&attr[1]);
	//~ pthread_create(&tid[1],&attr[1],runner,argv[1]);
	//~ pthread_join(tid[1],NULL);	
	//5位哲学家的线程开始思考/饥饿/吃饭 
	for (i=0;i<5;i++)
	{
		switch(i)//ls代表每位哲学家id不同 
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
			case 3:
				ls="3";
				break;	
			case 4:
				ls="4";
				break;
		}
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runner,ls);//哲学家开始运行 
		
	}
	
	while(1);
	//~ while(1)
	//~ {
		//~ sleep(2);
		//~ for (i=0;i<5;i++)
		//~ {
			//~ switch (pcon[i])
			//~ {
				//~ case thinking:
					//~ printf("The %d philosopher is thinking\n",i+1);
					//~ break;
				//~ case hungry:
					//~ printf("The %d philosopher is hungry\n",i+1);
					//~ break;
				//~ case eating:
					//~ printf("The %d philosopher is eating\n",i+1);
					//~ break;
			//~ }
			
		//~ }
	//~ }
	
	
	return 0;
}
void *runner(void *param)
{
	
	int t;
	int number = atoi(param);//每位哲学家的id，同时也代表了5个不同的随机数种子 
	srand(number);
	int left=number;//左手筷子编号即为自己所在编号 
	int right;
	if (number!=4)
		right=number+1;// 右手筷子编号则是自己号码加一 
	else
		right=0;//第5位（4）哲学家右手编号为0 
	while(1)
	{
		printf("The %d is thinking\n",number+1);//
		pcon[number]=thinking;
		t=rand() % 5 + 1;
		sleep(t);
		pcon[number]=hungry;
		printf("The %d is hungry\n",number+1);//

		while(ccon[left]||ccon[right]);//这个哲学家判断左右手的筷子都空闲时才能吃饭 
		ccon[left]=occupied;//筷子被占用 
		ccon[right]=occupied;

		
		
		pcon[number]=eating;//哲学家开始吃饭 
		printf("The %d is eating\n",number+1);//
		t=rand() % 5 + 1;
		sleep(t);//吃饭
		ccon[left]=leisure;//释放筷子
		ccon[right]=leisure;

	}
}
