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
		cout << "��������һ����������ʮλ���ڵ���׺���ʽ(��׺Ŷ):" << endl;
		cin >> expression;
		CreateBiTree(p, expression);
		while (key)
		{
			cout << "��ѡ�� 1,ǰ����� 2,������� 3,������� 4,��α��� 5,���� 6,����������ʽ 0,��������" << endl;
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
				cout << "��������һ����������ʮλ���ڵ���׺���ʽ(��׺Ŷ):" << endl;
				cin >> expression;
				CreateBiTree(p, expression);
				break;
			default:
				cout << "�����С�İ�������" << endl;
				break;
			}
		}
	}
	DestroyBiTree(p);
	return 0;
}