#include <stdio.h>

void main(){
	int arr[10];
	int i, j, N, temp;
	
	/*Declare function below for better readability*/
	int find_max(int b[10], int k);
	void exchange(int b[10], int k);
	
	printf("\nEnter values in array.");
	scanf("%d", &N);
	printf("\nEnter values one by one.");
	for(i=0; i<N; i++){
		scanf("%d", &arr[i]);
	}
	
	/*Perform sorting*/
	exchange(arr, N);
	printf("Array sorted:\n");
	for(i=0; i<N; i++){
		printf("%d\n", arr[i]);
	}
}

int find_max(int b[10], int k){
	int max=0, j;
	
	for(j=1; j<=k; j++){
		if(b[j] > b[max]){
			max = j;
		}
	}
	return(max):
}

void exchange(int b[10], int k){
	int temp, big, j;
	
	for(j=k-1; j>=1; j--){
		big = find_max(b, j);
		temp = b[big];
		b[big] = b[j];
		b[j] = temp;
	}
	return;
}