// tree.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BinaryTree.h"
#include <iostream>
#include <queue>
using namespace::std;

template<class T>
class LStack {
public:
	LStack();
	~LStack();
	void Push(T val);
	void Pop();
	int Size() const;
	T Top() const;

private:
	typedef struct Node {
		T val;
		Node* next;
	} Node, *PNode, *List;

private:
	List data;
	int size;
};

template<class T>
LStack<T>::LStack() {
	data = NULL;
	size = 0;
}

template<class T>
LStack<T>::~LStack() {
	delete data;
}

template<class T>
void LStack<T>::Push(T val)
{
	List p = data;
	if (!data) {
		data = new Node();
		data->val = val;
		data->next = NULL;
		size++;
		return;
	}
	while (p->next)
	{
		p = p->next;
	}
	p = p->next = new Node();
	p->val = val;
	p->next = NULL;
	size++;
	return;
}

template<class T>
void LStack<T>::Pop()
{
	if (!size) {
		return;
	}
	List p = data;
	int i = 1;
	while (i<size - 1)
	{
		p = p->next;
		i++;
	}
	delete p->next;
	p->next = NULL;
	size--;
	if (!size) {
		delete p;
		data = NULL;
	}
	return;
}

template<class T>
int LStack<T>::Size() const
{
	return size;
}

template<class T>
T LStack<T>::Top() const
{
	if (!size) {
		return '\0';
	}
	List p = data;
	int i = 1;
	while (i<size)
	{
		p = p->next;
		i++;
	}
	return p->val;
}

Status InitBiTree(BiTree T) {
	if (!T) return ERROR;
	T->data = 0;
	T->key = -1;
	T->lchild = T->rchild = NULL;
	return SUCEESS;
}

Status visit(char data) {
	if (data == -1) return ERROR;
	cout << data;
	return SUCEESS;
}

int is_sign(char p) {
	return p == '*'||p == '+'||p == '-'||p == '/';
}

Status CreateBiTree(BiTree T, char* definition) {
	char *p=NULL,sign;
	LStack<char> stack1;
	LStack<BiTree> stack2;
	BiTree P=new BiTNode;
	int i,L=-1,size,temp;
	for (p = definition; *p; p++) L++;
	char *expression = new char[L + 1];
	for (i = 0; i <= L; i++) {
		*(expression + i) = *(definition + L - i);
	}
	*(expression + i) = '\0';
	for (p = expression,i=0;* p; p++) {
		size = stack1.Size();
		if (*p <= '9'&&*p >= '0') {
			*(definition + i) = *p;
			i++;
		}
		else if(*p=='/'||*p=='*'||*p=='+'||*p=='-'){
			if (!size) {
				stack1.Push(*p);
			}
			else {
				sign = stack1.Top();
				if (*p == '+' || *p == '-') {
					if (sign == '*' || sign == '/') {
						while (sign)
						{
							*(definition + i) = sign;
							i++;
							stack1.Pop();
							sign = stack1.Top();
						}
					}
					stack1.Push(*p);
				}
				else {
					stack1.Push(*p);
				}
			}
		}
	}
	while (sign=stack1.Top())
	{
		*(definition + i) = sign;
		i++;
		stack1.Pop();
	}
	*(definition + i) = '\0';
	for (i = 0; i <= L / 2; i++) {
		temp=*(definition + i) ;
		*(definition + i) = *(definition +L- i);
		*(definition + L - i) = temp;
	}
	for (p = definition; *p; p++) {
		if (!T->lchild && !T->rchild) {
			T->data = *p;
			    T->lchild = new BiTNode;
				T->rchild = new BiTNode;
			stack2.Push(T);
			sign = T->data;
			P = T->lchild;
			continue;
		}
		P->data = *p;
		if (is_sign(P->data)) {
			P->lchild = new BiTNode;
			P->rchild = new BiTNode;
			stack2.Push(P);
			P = P->lchild;
		}
		else {
			P->lchild = P->rchild = NULL;
			P = stack2.Top();
			stack2.Pop();
			if (!P) break;
			P = P->rchild;
		}
	}
	while (P=stack2.Top())
	{
		P->rchild = NULL;
		stack2.Pop();
	}
	return SUCEESS;
}

Status InOrderTraverse(BiTNode T, Status visit(char data)) {
	BiTree p=&T;
	if (p != NULL) {
		if(p->lchild) InOrderTraverse(*p->lchild, visit);
		visit(T.data);
		if (p->rchild) InOrderTraverse(*p->rchild, visit);
	}
	return SUCEESS;
}

Status PostOrderTraverse(BiTNode T, Status visit(char data)) {
	BiTree p = &T;
	if (p != NULL) {
		if (p->lchild) PostOrderTraverse(*p->lchild, visit);
		if (p->rchild) PostOrderTraverse(*p->rchild, visit);
		visit(T.data);
	}
	return SUCEESS;
}

Status PreOrderTraverse(BiTNode T, Status visit(TElemType data)) {
	BiTree p = &T;
	if (p != NULL) {
		visit(T.data);
		if (p->lchild) PreOrderTraverse(*p->lchild, visit);
		if (p->rchild) PreOrderTraverse(*p->rchild, visit);
	}
	return SUCEESS;
}

Status DestroyBiTree(BiTree T) {
	
	if (T != NULL) {
		if (T->lchild) DestroyBiTree(T->lchild);
		if (T->rchild) DestroyBiTree(T->rchild);;
		delete T;
	}
	return SUCEESS;
}

Status LevelOrderTraverse(BiTNode T, Status visit(char data)) {
	queue<BiTree> que;
	BiTree p = &T;
	que.push(p);
	while (!que.empty())
	{
		visit(p->data);
		if (p->lchild) que.push(p->lchild);
		if (p->rchild) que.push(p->rchild);
		que.pop();
		if(!que.empty()) p = que.front();
	}
	return SUCEESS;
}

int Value(BiTNode T) {
	queue<BiTree> que;
	BiTree p = &T;
	char sign,N_sign;
	int sum=0,left,right;
	LStack<char> stackc;
	LStack<int> stackn;
	que.push(p);
	while (!que.empty())
	{
		sign = p->data;
		if (!is_sign(sign)) stackn.Push(sign-'0');
		else stackc.Push(sign);
		if (p->lchild) que.push(p->lchild);
		if (p->rchild) que.push(p->rchild);
		que.pop();
		if (!que.empty()) p = que.front();
	}
	while (stackc.Size())
	{
		sign = stackc.Top();
		stackc.Pop();
		right = stackn.Top();
		stackn.Pop();
		left = stackn.Top();
		stackn.Pop();
		switch (sign)
		{
		case '+':   left += right;
			break;
		case '-':   left -= right;
			break;
		case '*':   left *= right;
			break;
		case '/': 
			if (right == 0) {
				cout << "被除数为0??柴浪吗" << endl;
				return -1;
			  }
			left /= right;
			break;
		default:
			break;
		}
		N_sign = stackc.Top();
		if (N_sign == '/' || N_sign == '*')  sum += left;
		else stackn.Push(left);
	}
	 sum += stackn.Top();
	cout << "sum=" << sum << endl;
	return sum;
	
}



