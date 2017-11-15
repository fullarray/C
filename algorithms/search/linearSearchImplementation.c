// linearSearchImplementation
// Time complexity: O(n)

int linearSearchImplementation(int arr[], int n, int x){
	int i;
	for(i=0; i<n; i++){
		if(arr[i] == x){
			return i;
		}
	}
	return -1;
}