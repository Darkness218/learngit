// que.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "queue.h"
#include <malloc.h>


int main()
{
	AQueue *Q = (AQueue *)malloc(sizeof(AQueue));
	int key;
	void *data;
	Status Y_N;
	printf("循环队列测试:\n");
	for (key = 1; key != -1;)
	{
		printf("请选择要进行的操作: 1,创建队列 2,入队 3,出队 4,判断队列是否为空 5,判断队列是否为满 6,查询队头元素 7,清空队列 8确定队列长度 9,输出非字符队列 0,结束");
		scanf_s("%d", &key);
		switch (key)
		{
		case 0: key = -1;
			break;
		case 1: InitAQueue(Q);
			break;
		case 2: printf("请输入一个数据:\n");
			if (Q->data_size == sizeof(int)) {
				data = (int *)malloc(sizeof(int));
				scanf_s("%d", (int *)data);
				EnAQueue(Q, data);
				free(data);
			}
			else if (Q->data_size == sizeof(double)) {
				data = (double *)malloc(sizeof(double));
				scanf_s("%lf", (double *)data);
				EnAQueue(Q, data);
				free(data);
			}
			else if (Q->data_size == sizeof(char)) {
				data = (char *)malloc(sizeof(char));
				getchar();
				scanf_s("%c", (char *)data, 1);
				EnAQueue(Q, data);
				free(data);
			}
			key = 2;
			break;
		case 3:
			Y_N = DeAQueue(Q);
			if (Y_N) printf("出队成功\n");
			else printf("出队失败\n");
			break;
		case 4:
			Y_N = IsEmptyAQueue(Q);
			if (Y_N == FLASE) printf("队列不为空\n");
			else printf("队列为空\n");
			break;
		case 5:
			Y_N = IsFullAQueue(Q);
			if (!Y_N) printf("队列还没满\n");
			break;
		case 6:
			if (Q->data_size == sizeof(int)) {
				data = (int *)malloc(sizeof(int));
				Y_N = GetHeadAQueue(Q, data);
				if (Y_N) printf("%d\n", *(int *)data);
				free(data);
			}
			if (Q->data_size == sizeof(double)) {
				data = (double *)malloc(sizeof(double));
				Y_N = GetHeadAQueue(Q, data);
				if (Y_N) printf("%f\n", *(double *)data);
				free(data);
			}
			if (Q->data_size == sizeof(char)) {
				data = (char *)malloc(sizeof(char));
				Y_N = GetHeadAQueue(Q, data);
				if (Y_N) printf("%c\n", *(char *)data);
				free(data);
			}
			break;
		case 7: ClearAQueue(Q);
			break;
		case 8: key = LengthAQueue(Q);
			if (key != -1) {
				printf("队列长度为:%d\n", key);
			}
			else {
				printf("当前为空队列");
				key = 8;
			}

			break;
		case 9: if(Q->data_size==sizeof(int)) TraverseAQueue(Q, APrint);
			else if (Q->data_size == sizeof(double)) TraverseAQueue(Q, LPrint);
			else {
				printf("该队列未字符队列，暂时无法输出");
			}
			break;
		default:
			printf("错误的操作指令");
			break;
		}
	}

	LQueue *A = (LQueue *)malloc(sizeof(LQueue));
	A->front = NULL;
	A->rear = NULL;
	A->data_size = 0;
	printf("链式队列测试:\n");
	for (key = 1; key != -1;)
	{
		printf("请选择要进行的操作: 1,创建队列 2,入队 3,出队 4,判断队列是否为空  5,查询队头元素 6,清空队列 7,确定队列长度 8,输出非字符队列 0,结束");
		scanf_s("%d", &key);
		switch (key)
		{
		case 0: key = -1;
			break;
		case 1: InitLQueue(A);
			break;
		case 2: printf("请输入一个数据:\n");
			if (A->data_size == sizeof(int)) {
				data = (int *)malloc(sizeof(int));
				scanf_s("%d", (int *)data);
				EnLQueue(A, data);
				free(data);
			}
			else if (A->data_size == sizeof(double)) {
				data = (double *)malloc(sizeof(double));
				scanf_s("%lf", (double *)data);
				EnLQueue(A, data);
				free(data);
			}
			else if (A->data_size == sizeof(char)) {
				data = (char *)malloc(sizeof(char));
				getchar();
				scanf_s("%c", (char *)data, 1);
				EnLQueue(A, data);
				free(data);
			}
			break;
		case 3:
			Y_N = DeLQueue(A);
			if (Y_N) printf("出队成功\n");
			else printf("出队失败\n");
			break;
		case 4:
			Y_N = IsEmptyLQueue(A);
			if (Y_N == FLASE) printf("队列不为空\n");
			else printf("队列为空\n");
			break;
		case 5:
			if (A->data_size == sizeof(int)) {
				data = (int *)malloc(sizeof(int));
				Y_N = GetHeadLQueue(A, data);
				if (Y_N) printf("%d\n", *(int *)data);
				free(data);
			}
			if (A->data_size == sizeof(double)) {
				data = (double *)malloc(sizeof(double));
				Y_N = GetHeadLQueue(A, data);
				if (Y_N) printf("%f\n", *(double *)data);
				free(data);
			}
			if (A->data_size == sizeof(char)) {
				data = (char *)malloc(sizeof(char));
				Y_N = GetHeadLQueue(A, data);
				if (Y_N) printf("%c\n", *(char *)data);
				free(data);
			}
			break;
		case 6: ClearLQueue(A);
			break;
		case 7: key = LengthLQueue(A);
			if (key != -1) {
				printf("队列长度为:%d\n", key);
			}
			else {
				printf("当前为空队列");
				key = 7;
			}

			break;
		case 8: if (A->data_size == sizeof(int)) TraverseLQueue(A, APrint);
				else if (A->data_size == sizeof(double)) TraverseLQueue(A, LPrint);
				else {
					printf("该队列未字符队列，暂时无法输出");
				}
			break;
		default:
			printf("错误的操作指令");
			break;
		}
	}
    return 0;
}

