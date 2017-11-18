#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

int partition(int *A, int p, int r){
	int x,i,j,temp;
	x = A[r];
	i = p - 1;
	for (j = p; j <= r - 1; j++){
		if (A[j] <= x){
			i = i + 1;
			temp = A[i];
			A[i] = A[j];
			A[j] = temp;
		}
	}
	temp = A[i + 1];
	A[i + 1] = A[r];
	A[r] = temp;
	return(i + 1);
}

int randomized_partition(int *A, int p, int r){
	int i,temp;
	srand((unsigned)time(NULL));
	i = (rand() % (r - p + 1)) + p;
	temp = A[i];
	A[i] = A[r];
	A[r] = temp;
	return partition(A, p, r);
}

int randomized_select(int *A, int p, int r, int i){
	int q,k;
	if (p == r){
		return(A[p]);
	}
	q = randomized_partition(A, p, r);
	k = q - p + 1;
	if (i == k){
		return(A[q]);
	}
	else if (i < k){
		return(randomized_select(A, p, q - 1, i));
	}
	else return(randomized_select(A, q + 1, r, i - k));
}

int main(){
	clock_t starttime, endtime;
	int i, n, k;
	int a[100000];
	cout << "input the length n of the array (n>1000) and the order number k" << endl;
	cin >> n;
	cin >> k;
	starttime = clock();
	srand((unsigned)time(NULL));
	for (i = 0; i < n; i++){
		a[i] = rand();
	}
	cout << randomized_select(a, 0, n - 1, k) << endl;
	endtime = clock();
	cout << "total time:" << (double)(endtime - starttime)/CLOCKS_PER_SEC << "s" << endl;
	system("pause");
}