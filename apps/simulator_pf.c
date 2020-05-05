#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
int main() {
	int size = 100000;
	//scanf("Enter size: %d", &size);
        int **arr = (int**)malloc(size * sizeof(int*));
	int i;
	for(i = 0; i < size; i++) {
		arr[i] = (int*)malloc(size * sizeof(int));
	}
	int j = 0;
	int k;
	while(k < 3) {
	    for(i = 0; i < 1000; i++) {
	        for(j = 0; j < 1000; j++) {
		    int m = arr[i][j];
		    m = arr[size-1-i][size-1-j];
		    //printf("%d\n",m);
		}
	    }
	    k++;
	}
    return 0;
}
