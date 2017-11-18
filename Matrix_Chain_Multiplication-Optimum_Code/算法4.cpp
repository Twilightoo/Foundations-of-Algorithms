#include<cstdlib>
#include<iostream>
#include<time.h>

using namespace std;

struct Node{  //�ṹ���ʾ���������Ľ��
	int freq;
	Node* left;
	Node* right;
};

int PARENT(int i){  //������С���и�����λ��
	int j;
	j = floor(i / 2);
	return j;
}

int LEFT(int i){  //������С�������ӵ�λ��
	int j;
	j = 2 * i;
	return j;
}

int RIGHT(int i){  //������С�����Һ��ӵ�λ��
	int j;
	j = 2 * i + 1;
	return j;
}

void MIN_HEAPIFY(Node* *A, int i,int n){  //ά����С�ѵ�����
	int l, r, small;
	Node* temp;
	l = LEFT(i);
	r = RIGHT(i);
	if (l <= n&&A[l]->freq < A[i]->freq){
		small = l;
	}
	else{
		small = i;
	}
	if (r <= n&&A[r]->freq < A[small]->freq){
		small = r;
	}
	if (small != i){
		temp = A[i];
		A[i] = A[small];
		A[small] = temp;
		MIN_HEAPIFY(A, small, n);
	}
}

void BUILD_MIN_HEAP(Node* *A, int n){  //����С��
	int i;
	for (i = floor(n / 2); i >= 1; i--){
		MIN_HEAPIFY(A, i, n);
	}
}

Node* EXTRACT_MIN(Node* *A, int n){  //ȥ�������ؾ�����Сfreq�Ľ��
	Node* min;
	min = A[1];
	A[1] = A[n];
	MIN_HEAPIFY(A, 1, n - 1);
	return min;
}

void INSERT(Node* *A, Node* z, int n){  //����С���в����½��
	Node* temp;
	int i = n + 1;
	A[i] = z;
	while (i > 1 && A[PARENT(i)]->freq > A[i]->freq){
		temp = A[i];
		A[i] = A[PARENT(i)];
		A[PARENT(i)] = temp;
		i = PARENT(i);
	}
}

Node* HUFFMAN(Node* *Q, int n){  //���������룬���ع��������ĸ����
	int i;
	int size = n;
	Node* z;
	Node* x;
	Node* y;
	for (i = 1; i <= n - 1; i++){
		z = new Node;
		x = EXTRACT_MIN(Q,size);
		size--;
		y = EXTRACT_MIN(Q, size);
		size--;
		z->left = x;
		z->right = y;
		z->freq = x->freq + y->freq;
		INSERT(Q, z, size);
		size++;
	}
	return Q[1];
}

void INORDER_TREE_WALK(Node* x){  //���������������
	if (x != NULL){
		INORDER_TREE_WALK(x->left);
		cout << x->freq << " ";
		INORDER_TREE_WALK(x->right);
	}
}

void PREORDER_TREE_WALK(Node* x){  //���������������
	if (x != NULL){
		cout << x->freq << " ";
		PREORDER_TREE_WALK(x->left);
		PREORDER_TREE_WALK(x->right);
	}
}

int main(){
	clock_t starttime, endtime;
	int n, i,test;
	int C[100000];
	Node* Q[100000];
	Node* p;
	Node* root;
	cout << "�����ԣ�����0�����������ܣ�����1" << endl;
	cin >> test;
	if (test == 0){
		C[1] = 45;
		C[2] = 13;
		C[3] = 12;
		C[4] = 16;
		C[5] = 9;
		C[6] = 5;
		for (i = 1; i <= 6; i++){ 
			p = new Node;
			p->freq = C[i];
			p->left = NULL;
			p->right = NULL;
			Q[i] = p;
		}
		BUILD_MIN_HEAP(Q, 6);
		root = HUFFMAN(Q, 6);
		cout << "���ɶ���������������Ϊ��";
		INORDER_TREE_WALK(root);
		cout << endl;
		cout << "���ɶ���������������Ϊ��";
		PREORDER_TREE_WALK(root);
		cout << endl;
	}
	else{
		cout << "�����ַ���������������ַ�Ƶ��" << endl;
		cin >> n;
		starttime = clock();
		srand((unsigned)time(NULL));
		for (i = 1; i <= n; i++){ //��������ַ�Ƶ�ʲ��õ���ʼ��
			C[i] = rand();
			p = new Node;
			p->freq = C[i];
			p->left = NULL;
			p->right = NULL;
			Q[i] = p;
		}
		BUILD_MIN_HEAP(Q, n);
		root = HUFFMAN(Q, n);
		endtime = clock();
		cout << "���ɶ���������������Ϊ��";
		INORDER_TREE_WALK(root);
		cout << endl;
		cout << "���ɶ���������������Ϊ��";
		PREORDER_TREE_WALK(root);
		cout << endl;
		cout << "��ʱ�䣺" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;
	}
	system("pause");
}