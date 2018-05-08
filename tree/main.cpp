#include "BinaryTree.h"
#include "stdafx.h"
#include <iostream>
using namespace::std;

int main() {
	BiTree p=new BiTNode();
	char *expression = new char[100]();
	int key=1;
	while (key)
	{
		cout << "请先输入一个不含括号十位数内的中缀表达式(中缀哦):" << endl;
		cin >> expression;
		CreateBiTree(p, expression);
		while (key)
		{
			cout << "请选择 1,前序遍历 2,中序遍历 3,后序遍历 4,层次遍历 5,计算 6,重新输入表达式 0,结束操作" << endl;
			cin >> key;
			switch (key)
			{
			case 0: key = 0;
				break;
			case 1: PreOrderTraverse(*p, visit);
				break;
			case 2: InOrderTraverse(*p, visit);
				break;
			case 3:  PostOrderTraverse(*p, visit);
				break;
			case 4:  LevelOrderTraverse(*p, visit);
				break;
			case 5:Value(*p);
				break;
			case 6: DestroyBiTree(p);
				cout << "请先输入一个不含括号十位数内的中缀表达式(中缀哦):" << endl;
				cin >> expression;
				CreateBiTree(p, expression);
				break;
			default:
				cout << "你好像不小心按错了呢" << endl;
				break;
			}
		}
	}
	DestroyBiTree(p);
	return 0;
}