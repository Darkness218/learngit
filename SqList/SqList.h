#pragma once

#include <malloc.h>
#include <iostream>
using namespace::std;

template<class ElemType>
struct SqList{
	ElemType *elem;
	int length;
	int size;
	int increment;
	SqList():elem(0), length(-1), size(0), increment(0) {

	}
};

enum Status {
	ERROR=0,
	OK=1
};

template<class ElemType>
Status InitList_Sq(SqList<ElemType> &L, int size, int inc) {
	L.elem = (ElemType *)malloc(sizeof(ElemType)*size);
	if (!L.elem) return ERROR;
	L.increment = inc;
	L.size = size;
	L.length = 0;
	return OK;
}   //��ʼ��

template<class ElemType>
Status DestroyList_Sq(SqList<ElemType> &L);     //����

template<class ElemType>
Status ClearList_Sq(SqList<ElemType> &L) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return ERROR;
	}
	L.length = 0;
}   //���

template<class ElemType>
Status ListEmpty_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return ERROR;
	}
	if (!L.length) return OK;
	else return ERROR;
}  //�п�

template<class ElemType>
int Search_Sq(SqList<ElemType> L, ElemType e) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return -1;
	}
	if (ListEmpty_Sq(L)) return -1;
	for (int i = 0; i < L.length; i++) {
		if (L.elem[i] == e) return i;
	}
	return -1;
}   //����

template<class ElemType>
Status GetElem_Sq(SqList<ElemType> L, int i, ElemType &e) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return ERROR;
	}
	if (ListEmpty_Sq(L) || i > L.length) return ERROR;
	e = L.elem[i - 1];
	return OK;
}  //�õ������е�Ԫ��

template<class ElemType>
int ListLength_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return -1;
	}
	return L.length;
} //�õ�����ǰ����

template<class ElemType>
Status ListTracerse_Sq(SqList<ElemType> L, Status(*vist)(ElemType e));  //����

template<class ElemType>
Status PutElem_Sq(SqList<ElemType> &L, int i, ElemType e) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return ERROR;
	}
	if (i < 0 || i>L.length) return ERROR;
	if (L.length == L.size) {
		L.elem = (ElemType *)realloc(L.elem, L.size + L.increment);
		if (!L.elem) return ERROR;
		L.size += L.increment;
	}
	L.elem[i - 1] = e;
	L.length++;
	return OK;
}    //����

template<class ElemType>
Status Append_Sq(SqList<ElemType> &L, ElemType e) {
	if (L.length == L.size) {
		L.elem = (ElemType *)realloc(L.elem, (L.size + L.increment)*sizeof(ElemType));
		if (!L.elem) return ERROR;
		L.size += L.increment;
	}
	L.elem[L.length++] = e;
	return OK;
}    //��β����

template<class ElemType>
Status DeleteLast_Sq(SqList<ElemType> &L, ElemType &e) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return ERROR;
	}
	if (ListEmpty_Sq(L)) return ERROR;
	e = L.elem[--L.length];
	return OK;
}    //ɾ����βԪ��

template<class ElemType>
void Display_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "˳���δ��ʼ��,���ȳ�ʼ��˳���" << endl;
		return;
	}
	if (ListEmpty_Sq(L)) {
		cout << "��ǰ˳���Ϊ��" << endl;
		return;
	}
	cout << "˳���ǰ�ĳ���Ϊ:" << ListLength_Sq(L) << endl;
	cout << "����Ϊ:" << L.size << endl;
	cout << "������Ϊ:" << L.increment << endl;
	cout << "�������Ϊ:" << endl;
	for (int i=0; i < L.length; i++) {
		cout << L.elem[i] << " ";
	}
	cout << endl;
}

template<class ElemType>
void Text_Sq(SqList<ElemType> &L) {
	int key;
	Status right = ERROR;
	bool condition = true;
	while (condition)
	{
		ElemType e;
		int id=-1;
		system("CLS");
		cout << "��ѡ��Ҫ���ԵĹ���: 1,��ʼ��˳��� 2,���� 3,���� 4,ɾ�� 0,����" << endl;
		cin >> key;
		switch (key)
		{
		case 0:
			condition = false;
			break;
		case 1:
			int size, increment;
			cout << "�������ʼ����������������:" << endl;
			cin >> size >> increment;
			right = InitList_Sq(L, size, increment);
			if (right == ERROR) cout << "��ʼ��ʧ�������²���" << endl;
			break;
		case 2:
			cout << "������Ҫ���������:" << endl;
			cin >> e;
			right = Append_Sq(L, e);
			break;
		case 3:
			cout << "������Ҫ���ҵ�����:" << endl;
			cin >> e;
			id = Search_Sq(L, e);
			if (id == -1) {
				cout << "˳������޴�����" << endl;
			}
			else {
				cout << "����" << e << "�ڱ��е�����Ϊ:" << id<<endl;
			}
			break;
		case 4:
			right = DeleteLast_Sq(L,e);
			if(right) cout << "��ɾ��������Ϊ:" << e << endl;
			break;
		default:
			break;
		}
		if (key!=3&&right == ERROR) cout << "����ʧ�������²���" << endl;
		else Display_Sq(L);
		system("PAUSE");
	}
}





