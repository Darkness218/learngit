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
}   //初始化

template<class ElemType>
Status DestroyList_Sq(SqList<ElemType> &L);     //销毁

template<class ElemType>
Status ClearList_Sq(SqList<ElemType> &L) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return ERROR;
	}
	L.length = 0;
}   //清空

template<class ElemType>
Status ListEmpty_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return ERROR;
	}
	if (!L.length) return OK;
	else return ERROR;
}  //判空

template<class ElemType>
int Search_Sq(SqList<ElemType> L, ElemType e) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return -1;
	}
	if (ListEmpty_Sq(L)) return -1;
	for (int i = 0; i < L.length; i++) {
		if (L.elem[i] == e) return i;
	}
	return -1;
}   //查找

template<class ElemType>
Status GetElem_Sq(SqList<ElemType> L, int i, ElemType &e) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return ERROR;
	}
	if (ListEmpty_Sq(L) || i > L.length) return ERROR;
	e = L.elem[i - 1];
	return OK;
}  //得到链表中的元素

template<class ElemType>
int ListLength_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return -1;
	}
	return L.length;
} //得到链表当前长度

template<class ElemType>
Status ListTracerse_Sq(SqList<ElemType> L, Status(*vist)(ElemType e));  //遍历

template<class ElemType>
Status PutElem_Sq(SqList<ElemType> &L, int i, ElemType e) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
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
}    //插入

template<class ElemType>
Status Append_Sq(SqList<ElemType> &L, ElemType e) {
	if (L.length == L.size) {
		L.elem = (ElemType *)realloc(L.elem, (L.size + L.increment)*sizeof(ElemType));
		if (!L.elem) return ERROR;
		L.size += L.increment;
	}
	L.elem[L.length++] = e;
	return OK;
}    //表尾插入

template<class ElemType>
Status DeleteLast_Sq(SqList<ElemType> &L, ElemType &e) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return ERROR;
	}
	if (ListEmpty_Sq(L)) return ERROR;
	e = L.elem[--L.length];
	return OK;
}    //删除表尾元素

template<class ElemType>
void Display_Sq(SqList<ElemType> L) {
	if (L.elem == 0) {
		cout << "顺序表还未初始化,请先初始化顺序表" << endl;
		return;
	}
	if (ListEmpty_Sq(L)) {
		cout << "当前顺序表为空" << endl;
		return;
	}
	cout << "顺序表当前的长度为:" << ListLength_Sq(L) << endl;
	cout << "容量为:" << L.size << endl;
	cout << "增容量为:" << L.increment << endl;
	cout << "表的数据为:" << endl;
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
		cout << "请选择要测试的功能: 1,初始化顺序表 2,插入 3,查找 4,删除 0,结束" << endl;
		cin >> key;
		switch (key)
		{
		case 0:
			condition = false;
			break;
		case 1:
			int size, increment;
			cout << "请输入初始化的容量跟增容量:" << endl;
			cin >> size >> increment;
			right = InitList_Sq(L, size, increment);
			if (right == ERROR) cout << "初始化失败请重新操作" << endl;
			break;
		case 2:
			cout << "请输入要插入的数据:" << endl;
			cin >> e;
			right = Append_Sq(L, e);
			break;
		case 3:
			cout << "请输入要查找的数据:" << endl;
			cin >> e;
			id = Search_Sq(L, e);
			if (id == -1) {
				cout << "顺序表中无此数据" << endl;
			}
			else {
				cout << "数据" << e << "在表中的索引为:" << id<<endl;
			}
			break;
		case 4:
			right = DeleteLast_Sq(L,e);
			if(right) cout << "被删除的数据为:" << e << endl;
			break;
		default:
			break;
		}
		if (key!=3&&right == ERROR) cout << "操作失败请重新操作" << endl;
		else Display_Sq(L);
		system("PAUSE");
	}
}





