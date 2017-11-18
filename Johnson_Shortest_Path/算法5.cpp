#include<cstdlib>
#include<iostream>
#include<time.h>
#include<limits.h>

using namespace std;

struct edge{
	int num;
	int weight;
	edge* neighbor;
};

struct vertex{
	int d;
	int num;
	int heapindex;
	vertex* parent;
	edge* neighbor;
};

struct graph{
	vertex* chain[1000];
	int V;
};

int D[1000][1000];
vertex* pi[1000][1000];

void INITIALIZE_SINGLE_SOURCE(graph* G, int s){
	int i;
	for (i = 1; i <= G->V; i++){
		G->chain[i]->d = 99999999;
		G->chain[i]->heapindex = i;
		G->chain[i]->parent = NULL;
	}
	G->chain[s]->d = 0;
}

void RELAX(graph* G,vertex* u,edge* v){
	if (G->chain[v->num]->d > u->d + v->weight){
		G->chain[v->num]->d = u->d + v->weight;
		G->chain[v->num]->parent = u;
	}
}

int BELLMAN_FORD(graph* G, int s){
	int i,j;
	edge* p;
	INITIALIZE_SINGLE_SOURCE(G, s);
	for (i = 1; i <= G->V - 1; i++){
		for (j = 1; j <= G->V; j++){
			p = G->chain[j]->neighbor;
			while (p != NULL){
				RELAX(G, G->chain[j], p);
				p = p->neighbor;
			}
		}
	}
	for (j = 1; j <= G->V; j++){
		p = G->chain[j]->neighbor;
		while (p != NULL){
			if (G->chain[p->num]->d > G->chain[j]->d + p->weight){
				return 0;
			}
			p = p->neighbor;
		}
	}
	return 1;
}

int PARENT(int i){  //计算最小堆中父结点的位置
	int j;
	j = floor(i / 2);
	return j;
}

int LEFT(int i){  //计算最小堆中左孩子的位置
	int j;
	j = 2 * i;
	return j;
}

int RIGHT(int i){  //计算最小堆中右孩子的位置
	int j;
	j = 2 * i + 1;
	return j;
}

void MIN_HEAPIFY(vertex* *Q, int i, int n){  //维护最小堆的性质
	int l, r, small;
	vertex* temp;
	l = LEFT(i);
	r = RIGHT(i);
	if (l <= n&&Q[l]->d < Q[i]->d){
		small = l;
	}
	else{
		small = i;
	}
	if (r <= n&&Q[r]->d < Q[small]->d){
		small = r;
	}
	if (small != i){
		temp = Q[i];
		Q[i] = Q[small];
		Q[small] = temp;
		Q[i]->heapindex = i;
		Q[small]->heapindex = small;
		MIN_HEAPIFY(Q, small, n);
	}
}

void BUILD_MIN_HEAP(vertex* *Q, int n){  //建最小堆
	int i;
	for (i = floor(n / 2); i >= 1; i--){
		MIN_HEAPIFY(Q, i, n);
	}
}

vertex* EXTRACT_MIN(vertex* *Q, int n){  
	vertex* min;
	min = Q[1];
	Q[1] = Q[n];
	Q[1]->heapindex = 1;
	MIN_HEAPIFY(Q, 1, n - 1);
	return min;
}

void DECREASE_KEY(vertex* *Q, int i, int key){
	vertex* temp;
	Q[i]->d = key;
	while (i > 1 && Q[PARENT(i)]->d > Q[i]->d){
		temp = Q[i];
		Q[i] = Q[PARENT(i)];
		Q[PARENT(i)] = temp;
		Q[i]->heapindex = i;
		Q[PARENT(i)]->heapindex = PARENT(i);
		i = PARENT(i);
	}
}

void DIJKSTRA(graph* G, int s){
	INITIALIZE_SINGLE_SOURCE(G, s);
	vertex* Q[1000];
	vertex* v;
	edge* p;
	int i,j;
	for (i = 1; i <= G->V; i++){
		Q[i] = G->chain[i];
	}
	BUILD_MIN_HEAP(Q, G->V);
	i--;
	while (i > 1){
		v = EXTRACT_MIN(Q, i);
		i--;
		p = v->neighbor;
		while (p != NULL){
			if (Q[G->chain[p->num]->heapindex] == G->chain[p->num]){
				RELAX(G, v, p);
				if (G->chain[p->num]->d == v->d + p->weight){
					DECREASE_KEY(Q, G->chain[p->num]->heapindex, G->chain[p->num]->d);
				}
			}
			p = p->neighbor;
		}
	}
}

void PRINT_PATH(int s, int v){
	if (v == s){
		cout << s << '\t';
	}
	else if (pi[s][v] == NULL){
		cout << "从s到v没有路径" << endl;
	}
	else{
		PRINT_PATH(s, pi[s][v]->num);
		cout << v << '\t';
	}
}

void JOHNSON(graph* G){
	int i = 1;
	int j;
	edge* p;
	edge* rear;
	int h[1000];
	G->V++;
	G->chain[G->V] = new vertex;
	p = new edge;
	p->num = 1;
	p->weight = 0;
	G->chain[G->V]->neighbor = p;
	while (i < G->V-1){
		rear = p;
		p = new edge;
		p->num = i + 1;
		p->weight = 0;
		rear->neighbor = p;
		i++;
	}
	p->neighbor = NULL;
	if (BELLMAN_FORD(G, G->V) == 0){
		cout << "图包含有负权重环" << endl;
	}
	else{
		for (i = 1; i <= G->V; i++){
			h[i] = G->chain[i]->d;
		}
		for (i = 1; i <= G->V; i++){
			p = G->chain[i]->neighbor;
			while (p != NULL){
				p->weight = p->weight + h[i] - h[p->num];
				p = p->neighbor;
			}
		}
		G->V--;
		for (i = 1; i <= G->V; i++){
			DIJKSTRA(G, i);
			for (j = 1; j <= G->V; j++){
				D[i][j] = G->chain[j]->d+h[j]-h[i];
				pi[i][j] = G->chain[j]->parent;
			}
		}
	}
}

int main(){
	clock_t starttime, endtime;
	int i,j,c;
	int s[10];
	graph* G;
	vertex* v;
	edge* p;
	edge* rear;
	G = new graph;
	cout << "选择执行的操作：1.测试(书上示例) 2.分析性能(随机生成图)" << endl;
	cin >> c;
	if (c == 1){
		G->V = 5;
		for (i = 1; i <= 5; i++){
			v = new vertex;
			v->num = i;
			G->chain[i] = v;
		}
		v = G->chain[1];
		p = new edge;
		p->num = 2;
		p->weight = 3;
		p->neighbor = new edge;
		p->neighbor->num = 3;
		p->neighbor->weight = 8;
		p->neighbor->neighbor = new edge;
		p->neighbor->neighbor->num = 5;
		p->neighbor->neighbor->weight = -4;
		p->neighbor->neighbor->neighbor = NULL;
		v->neighbor = p;
		v = G->chain[2];
		p = new edge;
		p->num = 4;
		p->weight = 1;
		p->neighbor = new edge;
		p->neighbor->num = 5;
		p->neighbor->weight = 7;
		p->neighbor->neighbor = NULL;
		v->neighbor = p;
		v = G->chain[3];
		p = new edge;
		p->num = 2;
		p->weight = 4;
		p->neighbor = NULL;
		v->neighbor = p;
		v = G->chain[4];
		p = new edge;
		p->num = 1;
		p->weight = 2;
		p->neighbor = new edge;
		p->neighbor->num = 3;
		p->neighbor->weight = -5;
		p->neighbor->neighbor = NULL;
		v->neighbor = p;
		v = G->chain[5];
		p = new edge;
		p->num = 4;
		p->weight = 6;
		p->neighbor = NULL;
		v->neighbor = p;
		JOHNSON(G);
		cout << "最短路径长度矩阵：" << endl;
		for (i = 1; i <= 5; i++){
			for (j = 1; j <= 5; j++){
				cout << D[i][j] << '\t';
			}
			cout << endl;
		}
		for (i = 1; i <= 5; i++){
			for (j = 1; j <= 5; j++){
				cout << "从结点" << i << "到结点" << j << "的最短路径" << endl;
				PRINT_PATH(i, j);
				cout << endl;
			}
		}
	}
	else{
		cout << "输入矩阵规模" << endl;
		cin >> G->V;
		starttime = clock();
		srand((unsigned)time(NULL));
		for (i = 1; i <= G->V; i++){
			v = new vertex;
			v->num = i;
			G->chain[i] = v;
		}
		for (i = 1; i <= G->V; i++){
			for (j = 1; j <= 5; j++){
				s[j] = i;
			}
			p = new edge;
			p->num = (rand() % G->V) + 1;
			while (p->num == i){
				p->num = (rand() % G->V) + 1;
			}
			s[1] = p->num;
			p->weight = (rand() % 102) - 1;
			G->chain[i]->neighbor = p;
			rear = p;
			for (j = 1; j < 5; j++){
				p = new edge;
				p->num = (rand() % G->V) + 1;
				while (p->num == s[1] || p->num == s[2] || p->num == s[3] || p->num == s[4] || p->num == s[5]){
					p->num = (rand() % G->V) + 1;
				}
				s[j + 1] = p->num;
				p->weight = (rand() % 102) - 1;
				rear->neighbor = p;
				rear = p;
			}
			p->neighbor = NULL;
		}
		JOHNSON(G);
		endtime = clock();
		cout << "总时间：" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;
	}
	system("pause");
}