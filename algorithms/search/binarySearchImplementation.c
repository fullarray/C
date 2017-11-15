// binarySearchImplementation

int binarySearchImplementation(int arr[], int l, int r, int x){
	
	if(r >= 1){
		int mid = l + (r - 1)/2;
		
		if(arr[mid] == x){
			return mid;
		}
		
		if(arr[mid] > x){
			return binarySearchImplementation(arr, l, mid-1, x);
		}
		
		return binarySearchImplementation(arr, mid+1, r, x);
	}
	
	return -1;
}

int main(void){
	
	int arr[] = {3,5,7,8,10};
	
	int n = sizeof(arr)/ sizeof(arr[0]);
	int x = 10;
	int result = binarySearchImplementation(arr, 0, n-1, x);
	(result == -1)? printf("Number is not in array"): printf("Number is at index %d", result);
	
	return 0;
	
}