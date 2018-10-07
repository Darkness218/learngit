#include "SqList.h"

//template<class ElemType>
//SqList<ElemType>::SqList():elem(0),length(-1),size(0),increment(0) {
//
//}

//��ʼ������
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

//�п�
template<class ElemType>
Status ListEmpty_Sq(SqList<ElemType> L) {
	if (!L.length) return OK;
	else return ERROR;
}

//��ѯ
template<class ElemType>
int Search_Sq(SqList<ElemType> L, ElemType e) {
	if (ListEmpty_Sq(L)) return -1;
	for (int i = 0; i < L.length; i++) {
		if (L.elem[i] == e) return i;
	}
	return -1;
}

//�õ�Ԫ��
template<class ElemType>
Status GetElem_Sq(SqList<ElemType> L, int i, ElemType &e) {
	if (ListEmpty_Sq(L)||i>L.length) return ERROR;
	e = L.elem[i - 1];
	return OK;
}

//�õ�����ĳ���
template<class ElemType>
int ListLength_Sq(SqList<ElemType> L) {
	return L.length;
}

//����
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

//��β����
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


//ɾ����βԪ��
template<class ElemType>
Status DeleteLast_Sq(SqList<ElemType> &L, ElemType &e) {
	if (ListEmpty_Sq(L)) return ERROR;
	e = L.elem[--L.length];
	return OK;
}

//���������Ϣ
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
	cout << "˳���ǰ�ĳ���Ϊ:" << ListLength_Sq(L)<<endl;
	cout << "����Ϊ:" << L.size<<endl;
	cout << "������Ϊ:" << L.increment << endl;
	cout << "�������Ϊ:" << endl;
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
			right=InitList_Sq(L, size, increment);
			if (right == ERROR) cout << "��ʼ��ʧ�������²���" << endl;
			break;
		case 2:
			int e;
			cout << "������Ҫ���������:" << endl;
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
		if (right == ERROR) cout << "����ʧ�������²���" << endl;
		else Display_Sq(L);

	}
}