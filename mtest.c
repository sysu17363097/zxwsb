#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <alloca.h>
extern void afunc(void);
int bss_var;
int data_var = 42;
int main(int argc, char **argv)
{
	char *p,*b,*nb;
	printf("代码段地址:\n");
	printf("\tmain:%p\n",main);
	printf("\tafunc:%p\n",afunc);
	printf("栈地址:\n");
	afunc();
	p = (char*)alloca(32);
	if(p != NULL){
		printf("\talloca()申请的起始地址:%p\n",p);
		printf("\talloca()申请的末尾地址:%p\n",p+31);
	}
	printf("数据段地址：\n");
	printf("\tdata_var地址:%p\n",&data_var);
	printf("BSS段地址：\n");
	printf("\tbss_var地址:%p\n",&bss_var);
	b = sbrk((ptrdiff_t) 32);
	nb = sbrk((ptrdiff_t) 0);
	printf("堆地址:\n");
	printf("\t初始堆尾:%p\n",b);
	printf("\t更新的堆尾:%p\n",nb);
	b = sbrk((ptrdiff_t) -16);
	nb = sbrk((ptrdiff_t) 0);
	printf("\t最终的堆尾:%p\n",nb);
	sleep(10000000);
}
void afunc(void)
{
	static int level = 0;
	int stack_var;
	if(++level == 4)
		return ;
	printf("\t栈%d: 栈地址: %p\n",level,&stack_var);
	afunc();
}
	
