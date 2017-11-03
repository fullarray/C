#include<stdio.h>

int interpolationSearchImplementation(int arr[], int n, int x){
	
	int lo = o, hi = (n - 1);
	
	
	while(lo <= hi && x >= arr[lo] && x <= arr[hi]){
		
		int pos = lo + (((double)(hi - lo) / (arr[hi]-arr[lo]))*(x - arr[lo]));
		
		if(arr[pos] == x){
			return pos;
		}
		
		if(arr[pos] < x){
			lo = pos + 1;
		}else{
			hi = pos - 1;
		}
	}
	
	return -1;
}

int main(){
	
	int arr[] = {4,5,2,9,2,45,29,22,33};
	
	int n = sizeof(arr)/sizeof(arr[0]);
	
	int x = 45;
	int index = interpolationSearchImplementation(arr, n, x);
	
	if(index != -1){
		printf("Index found %d", index);
	}else{
		printf("Index not found.");
	}
	
	return 0;
}