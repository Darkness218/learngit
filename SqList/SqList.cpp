#include "SqList.h"

//template<class ElemType>
//SqList<ElemType>::SqList():elem(0),length(-1),size(0),increment(0) {
//
//}

//初始化链表
template<class ElemType>
Status InitList_Sq(SqList<ElemType> &L, int size, int inc) {
	L.elem = malloc(sizeof(ElemType)*size);
	if (!L.elem) return ERROR;
	L.increment = inc;
	L.size = size;
	L.length = 0;
	return OK;
}

template<class ElemType>
Status ClearList_Sq(SqList<ElemType> &L) {
	L.length = 0;
}

//判空
template<class ElemType>
Status ListEmpty_Sq(SqList<ElemType> L) {
	if (!L.length) return OK;
	else return ERROR;
}

//查询
template<class ElemType>
int Search_Sq(SqList<ElemType> L, ElemType e) {
	if (ListEmpty_Sq(L)) return -1;
	for (int i = 0; i < L.length; i++) {
		if (L.elem[i] == e) return i;
	}
	return -1;
}

//得到元素
template<class ElemType>
Status GetElem_Sq(SqList<ElemType> L, int i, ElemType &e) {
	if (ListEmpty_Sq(L)||i>L.length) return ERROR;
	e = L.elem[i - 1];
	return OK;
}

//得到链表的长度
template<class ElemType>
int ListLength_Sq(SqList<ElemType> L) {
	return L.length;
}

//插入
template<class ElemType>
Status PutElem_Sq(SqList<ElemType> &L, int i, ElemType e) {
	if (i < 0||i>L.length) return ERROR;
	if (L.length== L.size) {
		L.elem = realloc(L.elem, L.size + L.increment);
		if (!L.elem) return ERROR;
		L.size += L.increment;
	}
	L.elem[i - 1] = e;
	L.length++;
	return OK;
}

//表尾插入
template<class ElemType>
Status Append_Sq(SqList<ElemType> &L, ElemType e) {
	if (L.length == L.size) {
		L.elem = realloc(L.elem, L.size + L.increment);
		if (!L.elem) return ERROR;
		L.size += L.increment;
	}
	L.elem[L.length - 1] = e;
	L.length++;
	return OK;
}


//删除表尾元素
template<class ElemType>
Status DeleteLast_Sq(SqList<ElemType> &L, ElemType &e) {
	if (ListEmpty_Sq(L)) return ERROR;
	e = L.elem[--L.length];
	return OK;
}

//输出链表信息
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
	cout << "顺序表当前的长度为:" << ListLength_Sq(L)<<endl;
	cout << "容量为:" << L.size<<endl;
	cout << "增容量为:" << L.increment << endl;
	cout << "表的数据为:" << endl;
	for (int i; i < L.length; i++) {
		cout << L.elem[i]<<" ";
	}
}

template<class ElemType>
void Text_Sq(SqList<ElemType> &L) {
	int key;
	Status right = ERROR;
	bool condition = true;
	while (condition)
	{
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
			right=InitList_Sq(L, size, increment);
			if (right == ERROR) cout << "初始化失败请重新操作" << endl;
			break;
		case 2:
			int e;
			cout << "请输入要插入的数据:" << endl;
			cin >> e;
			right = Append_Sq(L, e);
			break;
		case 3:
			break;
		case 4:
			right = DeleteLast_Sq(L);
			break;
		default:
			break;
		}
		if (right == ERROR) cout << "操作失败请重新操作" << endl;
		else Display_Sq(L);

	}
}