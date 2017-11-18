#include<cstdlib>
#include<iostream>
#include<time.h>
#include<limits.h>

using namespace std;

int m[1000][1000];  //m[i][j]��Ŵ�Ai���˵�Aj��������ٱ����������
int s[1000][1000];  //s[i][j]��Ŵ�Ai���˵�Aj�����ŷָ��
                    //��Ϊȫ�ֱ���

void MATRIX_CHAIN_ORDER(int* p, int n){  //�Ե����ϼ���m��s
	int i,l,j,k,q;
	for (i = 1; i <= n; i++){            //����ֵ
		m[i][i] = 0;
	}
	for (l = 2; l <= n; l++){
		for (i = 1; i <= n - l + 1; i++){
			j = i + l - 1;
			m[i][j] = INT_MAX;
			for (k = i; k <= j - 1; k++){
				q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (q < m[i][j]){       //����
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
}

void PRINT_OPTIMAL_PARENS(int i, int j){  //�ݹ�����������Ż�����
	if (i == j){
		cout << "A" << i;
	}
	else{
		cout << "(";
		PRINT_OPTIMAL_PARENS(i, s[i][j]);
		PRINT_OPTIMAL_PARENS(s[i][j] + 1, j);
		cout << ")";
	}
}

int main(){
	clock_t starttime, endtime;
	int n,i,test;
	int p[1000];
	cout << "�����ԣ�����0�����������ܣ�����1" << endl;
	cin >> test;
	if (test == 0){
		p[0] = 30;
		p[1] = 35;
		p[2] = 15;
		p[3] = 5;
		p[4] = 10;
		p[5] = 20;
		p[6] = 25;
		MATRIX_CHAIN_ORDER(p, 6);
		cout << "��Ҫ�����˷�������" << m[1][6] << endl;
		cout << "���ŷ�����" << endl;
		PRINT_OPTIMAL_PARENS(1, 6);
		cout << endl;
	}
	else{
		cout << "�������˾���ĸ�����������ɾ����С" << endl;
		cin >> n;
		starttime = clock();  //��¼��ʼʱ��
		srand((unsigned)time(NULL));
		for (i = 0; i <= n; i++){  //��[1,100]��������ɾ��������������
			p[i] = (rand() % 100) + 1;
		}
		MATRIX_CHAIN_ORDER(p, n);
		cout << "��Ҫ�����˷�������" << m[1][n] << endl;
		cout << "���ŷ�����" << endl;
		PRINT_OPTIMAL_PARENS(1, n);
		cout << endl;
		endtime = clock();  //��¼����ʱ��
		cout << "��ʱ�䣺" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;
	}
	system("pause");
}
