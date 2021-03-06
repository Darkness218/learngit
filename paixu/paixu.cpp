// paixu.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <string>
#include<stack> 
#include <iomanip>
#include <iostream>
using namespace::std;



void InsertSort(int *a,int n); //插入排序
void MergeSort(int *a, int n);  //归并排序
void QSort_Recursion(int *a, int n);   //快排递归版
void QSort(int *a, int n); //快排非递归版
void CountSort(int *a, int n); //计数排序
void RadixCountSort(int *a, int n);  //基计数排序
void colorSort(int *a, int n);   //颜色排序
void findK(int *a, int n);      //查询第K大
void text();    //测试函数

void InsertSort(int *a, int n) {
	if (n == 1) {
		cout << "数组只有一个数据，无须排序" << endl;
		return;
	}
	int tem;  //取值变量
	int i, j; //数组索引
	for (i = 1; i < n; i++) {
		tem = a[i];
		for (j = i - 1; j > -1; j--) {
			if (a[j] > tem) {
				a[j + 1] = a[j];
				if (j != 0 && a[j - 1] <= tem) {
					a[j] = tem;
					break;
				}
			}
			else break;
		}
		if (j == -1) a[0] = tem;
		else if (j != i - 1) a[j] = tem;
	}
}     

void MergeSort(int *a,int n) {
	int i,k,j=0,t=0;
	i = ((double)n / 2 + 0.5);
	if (n > 2) {
		MergeSort(a, i);
		MergeSort(a+i, n - i);
	}
	else if (n == 1) return;
	else if (n < 1) {
		cout << "我看你是故意没事找事，连数据都没有就要排序" << endl;
		return;
	}
	int *p[3];
	p[0] = a;
	p[1] = new int[i]();
	for (k = 0; k < i; k++) *(p[1]+k) = *(p[0]+k);
	p[2] = new int[n - i]();
	for (k = 0; k < n-i; k++) *(p[2] + k) = *(p[0] + k+i);
	for (k = 0; k < n; k++) {
		if (j < i&&t < n - i) {
			if (*(p[1] + j) > *(p[2] + t)) {
				*p[0] = *(p[2] + t);
				t++;
			}
			else {
				*p[0] = *(p[1] + j);
				j++;
			}
		}
		else if (j == i) {
			*p[0] = *(p[2] + t);
			t++;
		}
		else if (t == n - i) {
			*p[0] = *(p[1] + j);
			j++;
		}
		p[0]++;
	}
}

void QSort_Recursion(int *a, int n) {
	if (n < 2) return;
	int i=0,j=n-i-1,k=0,base=*a;
	while (i < j)
	{
		while (i<j)
		{
			if (a[j] >= base) j--;
			else {
				a[k] = a[j];
				a[j] = base;
				k = j;
				break;
			}
		}
		while (i<j)
		{
			if (a[i] <= base) i++;
			else {
				a[k] = a[i];
				a[i] = base;
				k = i;
				break;
			}
		}
	
	}
	
	QSort_Recursion(a, i+1);
	QSort_Recursion(a+i+1,n-i-1);
}



int partition(int *a, int n) {
	int low = 0, high = n;
	int *pivot = a;  //任选元素作为轴，这里选首元素  
	while (low<high) {
		while (low<high && a[high] >= *pivot)
			high--;
		a[low] = a[high];
		while (low<high && a[low] <=* pivot)
			low++;
		a[high] = a[low];
	}
	//此时low==high  
	a[low] = *pivot;
	return low;
}
void QSort(int *a, int n) {
	stack<int> st;
	int low = 0;
	int high = n;
	
	if (low<high) {
		int mid = partition(a, n);
		if (low<mid - 1) {
			st.push(low);
			st.push(mid - 1);
		}
		if (mid + 1<high) {
			st.push(mid + 1);
			st.push(high);
		}
		//其实就是用栈保存每一个待排序子串的首尾元素下标，下一次while循环时取出这个范围，对这段子序列进行partition操作  
		while (!st.empty()) {
			int q = st.top();
			st.pop();
			int p = st.top();
			st.pop();
			mid = partition(a,mid);
			if (p<mid - 1) {
				st.push(p);
				st.push(mid - 1);
			}
			if (mid + 1<q) {
				st.push(mid + 1);
				st.push(q);
			}
		}
	}
}
void CountSort(int *a, int n) {
	int i, *count;
	int *temp = new int[n]();
	count = new int[10]();
	for (i = 0; i < n; i++) {
		count[a[i]]++;
		*(temp + i) = *(a + i);
	}
	for (i = 1; i < 10; i++) {
		count[i]+=count[i-1];
	}
	for (i = n-1; i > -1; i--) {
		a[--count[temp[i]]] = temp[i];
	}
	
}

void RadixCountSort(int *a, int n) {
	int i, *bucket, maxVal = 0, digitPosition = 1;
	bucket = new int[n]();
	    for (i = 0; i < n; i++) {                    //找到最大的数，确定排序次数
		         if (a[i] > maxVal) maxVal = a[i];
		
	}
		   int pass = 1;  
		   while (maxVal / digitPosition > 0) {
			        int *digitCount=new int[10]();
			        for (i = 0; i < n; i++)
			            digitCount[a[i] / digitPosition % 10]++;
			        for (i = 1; i < 10; i++)
			             digitCount[i] += digitCount[i - 1];
			        for (i = n - 1; i >= 0; i--)
			           bucket[--digitCount[a[i] / digitPosition % 10]] = a[i];
			        for (i = 0; i < n; i++)
			         a[i] = bucket[i];
		         digitPosition *= 10;
	}

}

void colorSort(int *a, int n) {
	int *p[3],tem;
	p[0] =p[1]= a;
	p[2] = a + n-1;
	while (p[0]<p[2])
	{
		if (*p[2] == 2) { 
			p[2]--;
		   continue;
		}
		else if (*p[1] == 0) { 
			p[1]++; 
			if (p[0] < p[1])  p[0] = p[1];
		   continue;
		}
		else if (*p[0] < 1) {
			tem = *p[0];
			*p[0] = *p[1];
			*p[1] = tem;
			p[1]++;
		}
		else if (*p[0] > 1) {
			tem = *p[0];
			*p[0] = *p[2];
			*p[2] = tem;
			p[2]--;
			if (*p[0] < 1) {
				tem = *p[0];
				*p[0] = *p[1];
				*p[1] = tem;
				p[1]++;
			}
		}
		p[0]++;
	}
}

void findK(int *a, int n) {
	static int t=-1;
	if (t == -1) {
		cout << "请输入要找要找的数的序号:" << endl;
		cin >> t;
	}
	if (n < 2) { 
		cout << "第k大的数为:" << a[0] << endl;
		t = -1;
		return; 
	}
	int i = 0, j = n - i - 1, k = 0, base = *a;
	while (i < j)
	{
		while (i<j)
		{
			if (a[j] >= base) j--;
			else {
				a[k] = a[j];
				a[j] = base;
				k = j;
				break;
			}
		}
		while (i<j)
		{
			if (a[i] <= base) i++;
			else {
				a[k] = a[i];
				a[i] = base;
				k = i;
				break;
			}
		}

	}

	if (i == t-1) {
		cout << "第k大的数为:" << a[i] << endl;
		t = -1;
	}
	else if(i>t-1) findK(a, i + 1);
	else if (i < t-1) {
		t = t - i-1;
		findK(a + i + 1, n - i - 1);
	}
}

void text() {
	int *a;
	int n = 0, i,num,j;
	int key = 1;
	void (*fun)(int *a, int n)=NULL;
	clock_t start, diff;
	FILE *fp = fopen("text.txt", "w");
	while (key)
	{
		cout << "请选择要使用的排序方法: 1,插入 2,快速(递归) 3,快速 4,归并 5,计数 6,基数计数  7,颜色排序 8,找k大 0,结束" << endl;
		cin >> key;
		if (!key) break;
		switch (key)
		{
		case 1: fun = InsertSort;
			break;
		case 2:
			fun = QSort_Recursion;
			break;
		case 3: 
			cout << "我还不会....." << endl;
			continue;
			fun = QSort;
			break;
		case 4:fun = MergeSort;
			break;
		case 5: fun = CountSort;
			break;
		case 6:fun = RadixCountSort;
			break;
		case 7:fun = colorSort;
			break;
		case 8:fun = findK;
			break;
		default:
			break;
		}
		if (key == 5) num = 10;
		else if (key == 7) num = 3;
		else num = 10000;
		cout << "请选择要测试的数据数量: 1,100*10k次 2,10000 3,50000 4,200000" << endl;
		cin >> key;
		if (key != 1) cout << "本次的测试数据为:" << endl;
		switch (key)
		{
		case 1:   n = 100;
			break;
		case 2:  n = 10000;
			break;
		case 3:   n = 50000;
			break;
		case 4:   n = 200000;
			break;
		default:
			break;
		}
		start = clock();
		if (n == 100) {
			for (j = 1; j < 10000; j++) {
				cout << "待排序的数据为:" << endl;
				a = new int[n]();
				for (i = 1; i <= n; i++) {
					a[i - 1] = rand() % num;
					cout << setw(5) << a[i - 1];
					if (i % 10 == 0 && i != 0) cout << endl;
				}
				cout << endl;
				(*fun)(a, n);
				if (fun != findK) {
					cout << "排序后的数据为:" << endl;
					for (i = 1; i <= n; i++) {
						cout << setw(5) << a[i - 1];
						if (i % 10 == 0 && i != 0) cout << endl;
					}
					cout << endl;
				}
				delete[] a;
			}
		}
		cout << "待排序的数据为:" << endl;
		fprintf(fp, "%s\n", "待排序的数据为:");
		a = new int[n]();
		for (i = 1; i <= n; i++) {
			a[i - 1] = rand() % num;
			cout << setw(5) << a[i - 1]<<"|";
			fprintf(fp, "%5d%c", a[i - 1], '|');
			if (i % 10 == 0 && i != 0) {
				cout << "\n|";
				fprintf(fp, "\n%c", '|');
			}

		}
		cout << endl;
		(* fun)(a, n);
		if (fun != findK) {
			cout << "排序后的数据为:" << endl;
			fprintf(fp, "%s\n", "排序后的数据为:");
			for (i = 1; i <= n; i++) {
				cout << setw(5) << a[i - 1]<<"|";
				fprintf(fp, "%5d%c", a[i - 1], '|');
				if (i % 10 == 0 && i != 0) {
					cout << "\n|";
					fprintf(fp, "\n%c", '|');
				}
			}
			cout << endl;
		}
		delete[] a;
		diff = clock() - start;
		cout <<"排序用时:"<< diff << endl;
		}
	fclose(fp);
	}

int main()
{
	srand((unsigned)time(NULL));
	text();
	getchar();
	return 0;
}

