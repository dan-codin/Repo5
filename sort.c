#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("\nExtra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{

		//create temporary arrays
		int *arrLeft;
		int *arrRight;
	//array with one element is already sorted
	if(l < r){
	
		//find the middle of the array and assign it to variable m
		int m = l +(r-1)/2;
		//recursive call to mergeSort
		mergeSort(pData, l, m);
		//mergeSort(pData, m + 1, r);

		int i, j,k;
		int sizeL = m -l + 1;
		int sizeR = r - m;

		//copy data to temp arrays arrLeft and arrRight
		arrLeft = Alloc(sizeL);	
		for( i = 0; i < sizeL; i++){
			arrLeft[i] = pData[l + i];
		}
		//
		for(int p = 0; p<sizeL; p++){
			printf("%d ",arrLeft[p]);
		}
		//
		arrRight = Alloc(sizeR);
		for(j = 0; j < sizeR; j++){
			arrRight[j] = pData[m+1+j];
		}
		
		i = 0;
		j = 0;
		k = l;
		//merge the arrays
		while(i < sizeL && j < sizeR){
			if(arrLeft[i] <= arrRight[j]){
				pData[k] = arrLeft[i];
				i++;
				
			}
			else{
				pData[k] = arrRight[j];
				j++;
			}
				k++;
		}
		//check for remaining elements if the arrays were uneven
		while( i < sizeL){
			pData[k] = arrLeft[i];
			i++;
			k++;
		}
		//deallocate memory
		DeAlloc(arrLeft);
		while( j < sizeR){
			pData[k] = arrRight[j];
			j++;
			k++;
	
		}
		//deallocate memory
		DeAlloc(arrRight);
		
	}

	
	
	
	
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}