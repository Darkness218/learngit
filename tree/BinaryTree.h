typedef char TElemType;     // �������������Ԫ������Ϊ�ַ�

typedef struct {
   TElemType *elem;     // 0�ŵ�Ԫ����
   int lastIndex;              // ���������һ�����ı��
} SqBiTree;    // ˳��洢�Ķ�����

typedef struct  BiTNode {
    TElemType      data;     // ������
	int key;
    struct BiTNode  *lchild,*rchild;  // ���Һ���ָ��
} BiTNode,*BiTree;   // ��������

enum Status{
	SUCEESS,
	ERROR
};


Status InitBiTree(BiTree T);
//�������������ն�����T
Status DestroyBiTree(BiTree T);
//��ʼ������������T����
//����������ݻٶ�����T
Status CreateBiTree(BiTree T, char* definition);
//��ʼ������ definition�����������Ķ���
//�����������definition���������T


//���²��ֺ�������δָ����������
Status visit(char data);
int Value(BiTNode T);			//������Ķ�������ֵ
Status PreOrderTraverse(BiTNode T, Status visit(TElemType data));
//��ʼ������������T���ڣ�visitΪ�Խ��Ĳ�����Ӧ�ú���
//����������������T����ÿ��������visit����һ���ҽ�һ�Σ�һ��visitʧ�ܣ������ʧ��
Status InOrderTraverse(BiTNode T, Status visit(char data));	//�������
Status PostOrderTraverse(BiTNode T, Status visit(char data));	//�������
Status LevelOrderTraverse(BiTNode T, Status visit(char data));	//�������
/*
��ʾ�����ڽ��ṹ�������ø�Tagֵ��־������������������������
�ɸ�����Ҫ�������Ӳ���.
*/