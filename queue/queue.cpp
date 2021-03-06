// queue.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <malloc.h>
#include "queue.h"

void InitAQueue(AQueue *Q) {
	int key,i;
	void *data;
	if (!Q) {
		printf("使用了空指针作为参数\n");
		return;
	}
	if (!IsEmptyAQueue(Q) && Q != NULL) ClearAQueue(Q);
	Q->rear = -1;
	Q->front = -1;
	printf("请选择你要创建的队列类型 1,int 2,double 3,char \n");
	scanf_s("%d", &key);
	if (key == 1) {
		Q->data_size = sizeof(int);
		data = (int *)malloc(Q->data_size);
		for (i = 0; i < MAXQUEUE; i++)
		{
			Q->data[i] = (int *)malloc(Q->data_size);
		}
		printf("已为你创建一个长度为10的整形队列,请继续为该队列初始化\n");
		for (i = 0; i < MAXQUEUE; i++)
		{
			printf("是否输入数据入队 1,入队 2,停止\n");
			scanf_s("%d", &key);
			if (key == 1) {
				printf("请输入一个整形数:\n");
				scanf_s("%d", (int *)data);
				EnAQueue(Q, data);
			}
			else {
				printf("队列创建成功\n");
				return;
			}
		}
	}
	else if (key == 2) {
		Q->data_size = sizeof(double);
		data = (double *)malloc(Q->data_size);
		for (i = 0; i < MAXQUEUE; i++)
		{
			Q->data[i] = (double *)malloc(Q->data_size);
		}
		printf("已为你创建一个长度为10的双精度浮点数队列,请继续为该队列初始化\n");
		for (i = 0; i < MAXQUEUE; i++)
		{
			printf("是否输入数据入队 1,入队 2,停止\n");
			scanf_s("%d", &key);
			if (key == 1) {
				printf("请输入一个浮点数:\n");
				scanf_s("%lf", (double *)data);
				EnAQueue(Q, data);
			}
			else {
				printf("队列创建成功\n");
				return;
			}
		}
	}
	else if (key == 3) {
		Q->data_size = sizeof(char);
		data = (char *)malloc(Q->data_size);
		for (i = 0; i < MAXQUEUE; i++)
		{
			Q->data[i] = (char *)malloc(Q->data_size);
		}
		printf("已为你创建一个长度为10的字符型队列,请继续为该队列初始化\n");
		for (i = 0; i < MAXQUEUE; i++)
		{
			printf("是否输入数据入队 1,入队 2,停止\n");
			scanf_s("%d", &key);
			getchar();
			if (key == 1) {
				printf("请输入一个字符:\n");
				scanf_s("%c", (char *)data,1);
				getchar();
				EnAQueue(Q, data);
			}
			else {
				printf("队列创建成功\n");
				return;
			}
		}
	}
	else {
		printf("没有你想要创建的类型,请重新选择");
		InitAQueue(Q);
	}
	return;
}    //循环队列初始化函数


Status EnAQueue(AQueue *Q, void *data) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	Status Y_N;
	Y_N = IsFullAQueue(Q);
	if (Y_N == TRUE) {
		printf("队列已满,无法入队\n");
		return FLASE;
	}
	if (Q->front == -1) Q->front++;
	if (Q->data_size == sizeof(int)) {
		if (Q->rear<MAXQUEUE - 1) {
			Q->rear++;
			*(int *)Q->data[Q->rear] = *(int *)data;
		}
		else
		{
			Q->rear = 0;
			*(int *)Q->data[Q->rear] = *(int *)data;
		}
	}
	if (Q->data_size == sizeof(double)) {
		if (Q->rear<MAXQUEUE - 1) {
			Q->rear++;
			*(double *)Q->data[Q->rear] = *(double *)data;
		}
		else
		{
			Q->rear = 0;
			*(double *)Q->data[Q->rear] = *(double *)data;
		}
	}
	if (Q->data_size == sizeof(char)) {
		if (Q->rear<MAXQUEUE - 1) {
			Q->rear++;
			*(char *)Q->data[Q->rear] = *(char *)data;
		}
		else
		{
			Q->rear = 0;
			*(char *)Q->data[Q->rear] = *(char *)data;
		}
	}
	return TRUE;
	
}    //循环队列入队函数

Status IsEmptyAQueue(const AQueue *Q) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	if (Q->front == -1 && Q->rear == -1) return TRUE;
	return FLASE;
}

void ClearAQueue(AQueue *Q) {
	if (!Q) {
		printf("队列还未初始化\n");
		return;
	}
	Q->front = -1;
	Q->rear = -1;
	return;
}

Status IsFullAQueue(const AQueue *Q) {
	if (!Q) {
		printf("队列还未初始化\n");
		return FLASE;
	}
	if (Q->front== (1+Q->rear)%MAXQUEUE ) return TRUE;
	return FLASE;
}

void DestoryAQueue(AQueue *Q) {
	if (!Q) {
		printf("队列还未初始化\n");
		return;
	}
	free(Q);
}

Status GetHeadAQueue(AQueue *Q, void *e) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	if (IsEmptyAQueue(Q)) {
		printf("目前未空队列，没有队头\n");
		return FLASE;
	}
	if(Q->data_size==sizeof(int)) *(int *)e = *(int *)Q->data[Q->front];
	if (Q->data_size == sizeof(double)) *(double *)e = *(double *)Q->data[Q->front];
	if (Q->data_size == sizeof(char)) *(char *)e = *(char *)Q->data[Q->front];
	return TRUE;
}

int LengthAQueue(AQueue *Q) {
	if (!Q) {
		printf("队列还未初始化\n");
		return -1;
	}
	if (Q->rear == -1 && Q->front == -1) return 0;
	else if (Q->rear >= Q->front) return Q->rear - Q->front+1;
	else return MAXQUEUE+1 - (Q->front - Q->rear);
}

Status DeAQueue(AQueue *Q) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	Status Y_N;
	Y_N = IsEmptyAQueue(Q);
	if (Y_N == TRUE) {
		printf("队列为空队列,无法出队\n");
		return FLASE;
	}
	if (Q->front == Q->rear) {
		Q->front = -1;
		Q->rear = -1;
	}
	else if (Q->front < MAXQUEUE - 1) Q->front++;
	else Q->front = 0;
	return TRUE;
}

Status TraverseAQueue(const AQueue *Q, void(*foo)(void *q)) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	Status Y_N;
	Y_N = IsEmptyAQueue(Q);
	if (Y_N == TRUE) {
		printf("队列为空队列\n");
		return FLASE;
	}
	if (Q->data_size == sizeof(char)) {
		printf("我暂时还不会输出字符队列呢");
		return FLASE;
	}
	int i;
	for (i = Q->front; i != Q->rear; i++)
	{
		if (i == MAXQUEUE) i = 0;
		if (Q->rear == 0 && i == 0) break;
		printf("Q->data[%d]=", i);
		(*foo)(Q->data[i]);
	}
	printf("Q->data[%d]=", i);
	(*foo)(Q->data[i]);
	if (Q->data_size == sizeof(char)) {
		printf("我暂时还不会输出字符队列呢");
	}
	return TRUE;
}

void APrint(void *q) {
	printf("%d\n", *(int *)q);
}


void InitLQueue(LQueue *Q) {
	int key;
	if (!Q) {
		printf("使用了空指针作为参数\n");
		return;
	}
	if (!IsEmptyLQueue(Q) && Q != NULL) ClearLQueue(Q);
	printf("请选择你要创建的队列类型 1,int 2,double 3,char \n");
	scanf_s("%d", &key);
	if (key == 1) {
		Q->data_size = sizeof(int);
		printf("已为你创建一个整形队列\n");
		return;
			}
	else if (key == 2) {
		Q->data_size = sizeof(double);
		printf("已为你创建一个双精度浮点数队列\n");
				return;
			}
	else if (key == 3) {
		Q->data_size = sizeof(char);
		printf("已为你创建一个字符型队列\n");
				return;
			}
	else {
		printf("没有你想要创建的类型,请重新选择");
		InitLQueue(Q);
	}
	return;
}

void DestoryLQueue(LQueue *Q) {
	if (!Q) {
		printf("使用了空指针");
		return ;
	}
	ClearLQueue(Q);
	free(Q);
}

Status IsEmptyLQueue(const LQueue *Q){
	if (!Q) {
		printf("使用了空指针");
		return FLASE;
	}
	if (!Q->front && !Q->rear) return TRUE;
	return FLASE;
}

Status GetHeadLQueue(LQueue *Q, void *e) {
	if (!Q) {
		printf("还未创建队列\n");
		return FLASE;
	}
	if (IsEmptyLQueue(Q)) {
		printf("目前未空队列，没有队头\n");
		return FLASE;
	}
	if (Q->data_size == sizeof(int)) *(int *)e = *(int *)Q->front->data;
	if (Q->data_size == sizeof(double)) *(double *)e = *(double *)Q->front->data;
	if (Q->data_size == sizeof(char)) *(char *)e = *(char *)Q->front->data;
	return TRUE;
}

int LengthLQueue(LQueue *Q) {
	if (!Q) {
		printf("使用了空指针");
		return -1;
	}
	if (!Q->front && !Q->rear) return 0;
	Node *p=NULL;
	int i = 1;
	for (p = Q->front; p != Q->rear; p = p->next) {
		i++;
	}
	return i;
}

Status EnLQueue(LQueue *Q, void *data) {
	if (!Q) {
		printf("使用了空指针");
		return FLASE;
	}
	if (Q->data_size == 0) {
		printf("还未创建队列\n");
		return FLASE;
	}
	if (Q->data_size == sizeof(int)) {
		if (Q->front == NULL) {
			Q->front = (Node *)malloc(sizeof(Node));
			Q->rear = Q->front;
			Q->rear->data = (int *)malloc(sizeof(int));
			*(int *)Q->rear->data = *(int *)data;
			Q->rear->next = NULL;
		}
		else {
			Q->rear->next = (Node *)malloc(sizeof(Node));
			Q->rear = Q->rear->next;
			Q->rear->data = (int *)malloc(sizeof(int));
			*(int *)Q->rear->data = *(int *)data;
			Q->rear->next = NULL;
		}
	}
	if (Q->data_size == sizeof(double)) {
		if (Q->front == NULL) {
			Q->front = (Node *)malloc(sizeof(Node));
			Q->rear = Q->front;
			Q->rear->data = (double *)malloc(sizeof(double));
			*(double *)Q->rear->data = *(double *)data;
			Q->rear->next = NULL;
		}
		else {
			Q->rear->next = (Node *)malloc(sizeof(Node));
			Q->rear = Q->rear->next;
			Q->rear->data = (double *)malloc(sizeof(double));
			*(double *)Q->rear->data = *(double *)data;
			Q->rear->next = NULL;
		}
	}
	if (Q->data_size == sizeof(char)) {
		if (Q->front == NULL) {
			Q->front = (Node *)malloc(sizeof(Node));
			Q->rear = Q->front;
			Q->rear->data = (char *)malloc(sizeof(char));
			*(char *)Q->rear->data = *(char *)data;
			Q->rear->next = NULL;
		}
		else {
			Q->rear->next = (Node *)malloc(sizeof(Node));
			Q->rear = Q->rear->next;
			Q->rear->data = (char *)malloc(sizeof(char));
			*(char *)Q->rear->data = *(char *)data;
			Q->rear->next = NULL;
		}
	}
	return TRUE;
	
}

Status DeLQueue(LQueue *Q) {
	if (!Q) {
		printf("使用了空指针");
		return FLASE;
	}
	if (IsEmptyLQueue(Q)) {
		printf("队列为空，无法出列\n");
		return TRUE;
	}
	if (Q->front == Q->rear) {
		free(Q->front);
		Q->front = NULL;
		Q->rear = NULL;
		return TRUE;
	}
	Node *p = Q->front;
	Q->front = p->next;
	free(p);
	return TRUE;
}

void ClearLQueue(LQueue *Q) {
	if (!Q) {
		printf("使用了空指针");
		return ;
	}
	if (!Q->front && !Q->rear) return ;
	Node *p1 = NULL,*p2=NULL;
	for (p1 = Q->front; p1 != Q->rear; p1 = p2) {
		p2 = p1->next;
		free(p1);
	}
	free(p1);
	Q->front = NULL;
	Q->rear = NULL;
}

Status TraverseLQueue(const LQueue *Q, void(*foo)(void *q)) {
	if (!Q) {
		printf("使用了空指针\n");
		return FLASE;
	}
	Status Y_N;
	Y_N = IsEmptyLQueue(Q);
	if (Y_N == TRUE) {
		printf("队列为空队列\n");
		return FLASE;
	}
	Node *p = NULL;
	int i = 1;
	for (p = Q->front; p != Q->rear; p = p->next) {
		printf("第%d个数为:", i);
		(*foo)(p->data);
		i++;
	}
	printf("第%d个数为:", i);
	(*foo)(p->data);
	return TRUE;
}

void LPrint(void *q) {
	printf("%lf\n",*(double *) q);
}



