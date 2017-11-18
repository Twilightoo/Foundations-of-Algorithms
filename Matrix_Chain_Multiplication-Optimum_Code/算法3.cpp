#include<cstdlib>
#include<iostream>
#include<time.h>
#include<limits.h>

using namespace std;

int m[1000][1000];  //m[i][j]存放从Ai连乘到Aj所需的最少标量计算次数
int s[1000][1000];  //s[i][j]存放从Ai连乘到Aj的最优分割点
                    //均为全局变量

void MATRIX_CHAIN_ORDER(int* p, int n){  //自底向上计算m和s
	int i,l,j,k,q;
	for (i = 1; i <= n; i++){            //赋初值
		m[i][i] = 0;
	}
	for (l = 2; l <= n; l++){
		for (i = 1; i <= n - l + 1; i++){
			j = i + l - 1;
			m[i][j] = INT_MAX;
			for (k = i; k <= j - 1; k++){
				q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (q < m[i][j]){       //更新
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
}

void PRINT_OPTIMAL_PARENS(int i, int j){  //递归输出最优括号化方案
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
	cout << "若测试，输入0；若分析性能，输入1" << endl;
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
		cout << "需要标量乘法次数：" << m[1][6] << endl;
		cout << "最优方案：" << endl;
		PRINT_OPTIMAL_PARENS(1, 6);
		cout << endl;
	}
	else{
		cout << "输入连乘矩阵的个数，随机生成矩阵大小" << endl;
		cin >> n;
		starttime = clock();  //记录开始时间
		srand((unsigned)time(NULL));
		for (i = 0; i <= n; i++){  //在[1,100]间随机生成矩阵的行数和列数
			p[i] = (rand() % 100) + 1;
		}
		MATRIX_CHAIN_ORDER(p, n);
		cout << "需要标量乘法次数：" << m[1][n] << endl;
		cout << "最优方案：" << endl;
		PRINT_OPTIMAL_PARENS(1, n);
		cout << endl;
		endtime = clock();  //记录结束时间
		cout << "总时间：" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;
	}
	system("pause");
}
