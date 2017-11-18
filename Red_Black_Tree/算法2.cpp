#include<cstdlib>
#include<iostream>
#include<time.h>

using namespace std;

struct Node{  //�ýṹ���ʾ������Ľ�㣬�����ؼ��֡���ɫ�����ӡ��Һ��ӡ�������������
	int key;
	int size;
	char color;
	Node* left;
	Node* right;
	Node* p;
};

struct Tree{
	Node* root;
	Node* NIL;
};

void inorder_rbtree_walk(Tree* T,Node* x){  //����������������˳�����
	if (x != T->NIL){
		inorder_rbtree_walk(T,x->left);
		cout << x->key << " ";
		inorder_rbtree_walk(T,x->right);
	}
}

Node* rbtree_minimun(Tree* T){  //����������Сֵ
	Node *x = T->root;
	while (x->left != T->NIL){
		x = x->left;
	}
	return x;
}

Node* rbtree_maximun(Tree* T){  //�����������ֵ
	Node *x = T->root;
	while (x->right != T->NIL){
		x = x->right;
	}
	return x;
}

Node* rbtree_search(Tree* T, int x){  //�ҵ��ؼ���x���ڵĽ��
	Node* p = T->root;
	if (p == T->NIL || x == p->key){
		return p;
	}
	Tree* t = new Tree;
	t->NIL = T->NIL;
	if (x < p->key){
		t->root = p->left;
		return rbtree_search(t, x);
	}
	else{
		t->root = p->right;
		return rbtree_search(t, x);
	}
}

Node* rbtree_succesor(Tree* T,Node* x){  //�������н��ĺ��
	if (x->right != T->NIL){
		Tree* t = new Tree;
		t->NIL = T->NIL;
		t->root = x->right;
		return rbtree_minimun(t);
	}
	Node* y;
	y = x->p;
	while (y != T->NIL&&x == y->right){
		x = y;
		y = y->p;
	}
	return y;
}

Node* rbtree_predecessor(Tree* T, Node* x){  //�������н���ǰ��
	if (x->left != T->NIL){
		Tree* t = new Tree;
		t->NIL = T->NIL;
		t->root = x->left;
		return rbtree_maximun(t);
	}
	Node* y;
	y = x->p;
	while (y != T->NIL&&x == y->left){
		x = y;
		y = y->p;
	}
	return y;
}

void left_rotate(Tree*T, Node*x){  //��x����������ת
	Node* y = x->right;
	x->right = y->left;
	if (y->left != T->NIL){
		y->left->p = x;
	}
	y->p = x->p;
	if (x->p == T->NIL){
		T->root = y;
	}
	else if(x == x->p->left){
		x->p->left = y;
	}
	else{
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
	x->size = x->left->size + x->right->size + 1;
	y->size = y->left->size + y->right->size + 1;
}

void right_rotate(Tree*T, Node*y){  //��y����������ת
	Node* x = y->left;
	y->left = x->right;
	if (x->right != T->NIL){
		x->right->p = y;
	}
	x->p = y->p;
	if (y->p == T->NIL){
		T->root = x;
	}
	else if (y == y->p->left){
		y->p->left = x;
	}
	else{
		y->p->right = x;
	}
	x->right = y;
	y->p = x;
	y->size = y->left->size + y->right->size + 1;
	x->size = x->left->size + x->right->size + 1;
}

int rb_insert_fixup(Tree* T, Node* z){ //������z��Ժ��������ɫ���е���
	Node* y;
	int num=0;
	while (z->p->color == 'r'){
		if (z->p == z->p->p->left){
			y = z->p->p->right;
			if (y->color == 'r'){        //case1
				z->p->color = 'b';
				y->color = 'b';
				z->p->p->color = 'r';
				z = z->p->p;
			}
			else{
				if (z == z->p->right){     //case2
					z = z->p;
					left_rotate(T, z);
				}
				z->p->color = 'b';        //case3
				z->p->p->color = 'r';
				right_rotate(T, z->p->p);
			}
		}
		else{
			y = z->p->p->left;
			if (y->color == 'r'){
				z->p->color = 'b';
				y->color = 'b';
				z->p->p->color = 'r';
				z = z->p->p;
			}
			else{
				if (z == z->p->left){
					z = z->p;
					right_rotate(T, z);
				}
				z->p->color = 'b';
				z->p->p->color = 'r';
				left_rotate(T, z->p->p);
			}
		}
		num++;
	}
	T->root->color = 'b';
	return num;
}

int rb_insert(Tree* T, Node* z){  //������z
	Node* y = T->NIL;
	Node* x = T->root;
	int num = 0;
	while (x != T->NIL){
		y = x;
		y->size++;
		if (z->key < x->key){
			x = x->left;
		}
		else{
			x = x->right;
		}
		num++;
	}
	z->p = y;
	if (y == T->NIL){
		T->root = z;
	}
	else if(z->key<y->key){
		y->left = z;
	}
	else{
		y->right = z;
	}
	z->left = T->NIL;
	z->right = T->NIL;
	z->color = 'r';
	num+=rb_insert_fixup(T, z);
	return num;
}

void rb_transplant(Tree* T, Node* u, Node* v){  //�ý��v������u
	if (u->p == T->NIL){
		T->root = v;
	}
	else if (u == u->p->left){
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	v->p = u->p;
	Node* path = v->p;
	while (path != T->NIL){
		path->size = path->left->size + path->right->size + 1;
		path = path->p;
	}
}

int rb_delete_fixup(Tree* T, Node* x){  //ɾ������Ժ��������ɫ���е���
	Node* w;
	int num = 0;
	while (x != T->root&&x->color == 'b'){
		if (x == x->p->left){
			w = x->p->right;
			if (w->color == 'r'){        //case1
				w->color = 'b';
				x->p->color = 'r';
				left_rotate(T, x->p);
				w = x->p->right;
			}
			if (w->left->color == 'b'&&w->right->color == 'b'){  //case2
				w->color = 'r';
				x = x->p;
			}
			else{
				if (w->right->color == 'b'){  //case3
					w->left->color = 'b';
					w->color = 'r';
					right_rotate(T, w);
					w = x->p->right;
				}
				w->color = x->p->color;   //case4
				x->p->color = 'b';
				w->right->color = 'b';
				left_rotate(T, x->p);
				x = T->root;
			}
		}
		else{
			w = x->p->left;
			if (w->color == 'r'){
				w->color = 'b';
				x->p->color = 'r';
				right_rotate(T, x->p);
				w = x->p->left;
			}
			if (w->right->color == 'b'&&w->left->color == 'b'){
				w->color = 'r';
				x = x->p;
			}
			else{
				if (w->left->color == 'b'){
					w->right->color = 'b';
					w->color = 'r';
					left_rotate(T, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = 'b';
				w->left->color = 'b';
				right_rotate(T, x->p);
				x = T->root;
			}
		}
		num++;
	}
	x->color = 'b';
	return num;
}

int rb_delete(Tree* T, Node* z){  //ɾ�����z
	Node* y = z;
	Node* x=T->root;
	int num = 0;
	char y_original_color = y->color;
	while (x != z){
		if (z->key < x->key){
			x = x->left;
			num++;
		}
		else{
			x = x->right;
			num++;
		}
	}
	if (z->left == T->NIL){        //z��������Ϊ��
		x = z->right;
		rb_transplant(T, z, z->right);
	}
	else if (z->right == T->NIL){  //z��������Ϊ��
		x = z->left;
		rb_transplant(T, z, z->left);
	}
	else{                         //z����������������
		Tree* t = new Tree;
		t->root = z->right;
		t->NIL = T->NIL;
		y = rbtree_minimun(t);
		y_original_color = y->color;
		x = y->right;
		if (y->p == z){
			x->p = y;
		}
		else{
			rb_transplant(T, y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		rb_transplant(T, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (y_original_color == 'b'){
		num+=rb_delete_fixup(T, x);
	}
	return num;
}

Node* rb_select(Node* x, int i){
	int r = x->left->size + 1;
	if (i == r){
		return x;
	}
	else if (i < r){
		return rb_select(x->left, i);
	}
	else{
		return rb_select(x->right, i - r);
	}
}

int main(){
	int k=0;
	int value,i,n,arr[100000];
	int num = 0;
	Node* p;
	Tree* t = new Tree;
	Tree* T=new Tree;
	Node* root = new Node;
	Node* NIL = new Node;
	NIL->color = 'b';
	NIL->key = -1;
	NIL->left = NULL;
	NIL->right = NULL;
	NIL->p = NULL;
	NIL->size = 0;
	root = NIL;
	T->NIL = NIL;
	T->root = root;
	while (k != 10){
		cout << "ѡ��Ժ�������еĲ�����" << endl;
		cout << "1.����" << endl;
		cout << "2.����Сֵ" << endl;
		cout << "3.�����ֵ" << endl;
		cout << "4.����" << endl;
		cout << "5.��ǰ��" << endl;
		cout << "6.������" << endl;
		cout << "7.ɾ�����" << endl;
		cout << "8.���Ҿ��и����ȵ�Ԫ��" << endl;
		cout << "9.����������飬��������" << endl;
		cout << "10.�˳�" << endl;
		cin >> k;
		switch (k){
		  case(1):
			  if (T->root == T->NIL){
				  cout << "�˺����Ϊ��" << endl;
			  } 
			  else{
				  inorder_rbtree_walk(T,T->root);
				  cout << endl;
			  }
			  break;
		  case(2):
			  if (T->root == T->NIL){
				  cout << "�˺����Ϊ��" << endl;
			  }
			  else{
				  cout << "��СֵΪ:" << rbtree_minimun(T)->key << endl;
			  }
			  break;
		  case(3):
			  if (T->root == T->NIL){
				  cout << "�˺����Ϊ��" << endl;
			  }
			  else{
				  cout << "���ֵΪ:" << rbtree_maximun(T)->key << endl;
			  }
			  break;
		  case(4):
			  cout << "����value����value�ĺ��" << endl;
			  cin >> value;
			  p = rbtree_search(T, value);
			  if (p == T->NIL){
				  cout << "��ֵΪvalue�Ľ��" << endl;
			  }
			  else{
				  cout << rbtree_succesor(T, p)->key << endl;
			  }
			  break;
		  case(5):
			  cout << "����value����value��ǰ��" << endl;
			  cin >> value;
			  p = rbtree_search(T, value);
			  if (p == T->NIL){
				  cout << "��ֵΪvalue�Ľ��" << endl;
			  }
			  else{
				  cout << rbtree_predecessor(T, p)->key << endl;
			  }
			  break;
		  case(6):
			  cout << "����Ҫ����Ĺؼ���value" << endl;
			  cin >> value;
			  p = new Node;
			  p->color = 'r';
			  p->key = value;
			  p->left = T->NIL;
			  p->p = T->NIL;
			  p->right = T->NIL;
			  p->size = 1;
			  num=rb_insert(T, p);
			  inorder_rbtree_walk(T, T->root);
			  cout << endl;
			  break;
		  case(7):
			  cout << "����Ҫɾ���Ĺؼ���value" << endl;
			  cin >> value;
			  p = rbtree_search(T, value);
			  if (p == T->NIL){
				  cout << "��ֵΪvalue�Ľ��" << endl;
			  }
			  else{
				  num=rb_delete(T, p);
				  inorder_rbtree_walk(T, T->root);
				  cout << endl;
			  }
			  break;
		  case(8):
			  cout << "����iֵ�����ҵ�iС��Ԫ��" << endl;
			  cin >> i;
			  cout << rb_select(T->root, i)->key << endl;
			  break;
		  case(9) :
			  num = 0;
			  t->NIL = T->NIL;
			  t->root = T->NIL;
			  cout << "�������鳤��" << endl;
			  cin >> n;
			  srand((unsigned)time(NULL));
			  for (i = 0; i < n; i++){
				  arr[i] = rand();
			  }
			  for (i = 0; i < n; i++){
				  p = new Node;
				  p->color = 'r';
				  p->key = arr[i];
				  p->left = t->NIL;
				  p->p = t->NIL;
				  p->right = t->NIL;
				  p->size = 1;
				  num += rb_insert(t, p);
			  }
			  cout << "������ڶ�����ѭ������:" << num << endl;
			  value = rand();
			  p = new Node;
			  p->color = 'r';
			  p->key = value;
			  p->left = t->NIL;
			  p->p = t->NIL;
			  p->right = t->NIL;
			  p->size = 1;
			  cout << "����һ�����ѭ������" << rb_insert(t, p) << endl;
			  cout << "ɾ��һ�����ѭ������" << rb_delete(t, p) << endl;
			  break;
		  default:
			  break;
		}
	}
	system("pause");
}