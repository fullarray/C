#include<stdio.h>

void main(){
	int arr[10], i, j, n, array_key;
	printf("\nEnter array valuesz:\n");
	scanf("%d", &n);
	printf("Input %d array value(s): \n", n);
	for(i=0; i<n; i++){
		scanf("%d", &arr[i]);
	}
	for(i = 1; i<n; i++){
		array_key = arr[i];
		j = i-1;
	
	
	while(j >= 0 && arr[j] > array_key){
		arr[j+1] = arr[j];
		j = j-1;
	}
	
	arr[j+1] = array_key;
	}
	
	printf("Array sorted: \n");
	for(i=0; i<n; i++){
		printf("%d \n", arr[i]);
	}
}