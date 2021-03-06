/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

/*수정사항
	1)bubble sort 이중 for문 수정
	2)search 함수, 해시테이블에 찾고자하는 수가 없을 경우 예외처리 main에서 메시지 출력
	3)프로그램 종료시 해시 테이블 해제 하도록 freeTable()추가*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);	/*배열 랜덤한 값(0~12)으로 초기화*/
int freeArray(int *a);		/*동적할당한 배열원소 해제*/
void printArray(int *a);	/*배열 출력*/

int selectionSort(int *a);	/*선택 정렬*/
int insertionSort(int *a);	/*삽입 정렬*/
int bubbleSort(int *a);		/*버블 정렬*/
int shellSort(int *a);		/*쉘 정렬*/
/* recursive function으로 구현 */
int quickSort(int *a, int n);	/*퀵 정렬*/


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);

/*해시테이블 해제*/
int freeTable(int* ht);

int main()
{
	printf("-----[2021076029] [장정환]-----\n");
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));	//랜덤한 시드

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);	/*배열 랜덤한 값으로 초기화*/
			break;
		case 'q': case 'Q':
			freeArray(array);	/*배열 해제*/
			freeTable(hashtable);
			break;
		case 's': case 'S':
			selectionSort(array);	/*선택 정렬*/
			break;
		case 'i': case 'I':
			insertionSort(array);	/*삽입 정렬*/
			break;
		case 'b': case 'B':
			bubbleSort(array);		/*버블 정렬*/
			break;
		case 'l': case 'L':
			shellSort(array);		/*쉘 정렬*/
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);	//정렬 전
			quickSort(array, MAX_ARRAY_SIZE);	/*퀵 정렬*/
			printf("----------------------------------------------------------------\n");
			printArray(array);	//정렬 후

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);	//해시테이블 생성 전
			hashing(array, &hashtable);
			printArray(hashtable);	//해시테이블 생성
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);	//찾고자하는 key 값
			printArray(hashtable);	//해시테이블 출력
			index = search(hashtable, key);	//찾고자하는 key값을 가지는 버킷탐색
			if(index==-1){	//해시테이블에 없을 경우
				printf("fail to search\n");
				break;
			}
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);	//배열 출력
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL){
		free(a);
		}	//배열 해제
	return 0;
}
int freeTable(int* ht){
	if(ht != NULL){
		free(ht);
		}//해시 테이블 해제
	return 0;
}
void printArray(int *a)
{
	if (a == NULL) {
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	//원소 번호출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	//원소 출력
		printf("%5d ", a[i]);
	printf("\n");
}

/*선택 정렬*/
int selectionSort(int *a)
{
	int min;		//매 정렬단계에서 최소값을 저장할 변수
	int minindex;	//매 정렬단계에서 최솟값의 인덱스값을 저장할 변수
	int i, j;		//for문의 반복 변수

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		/*정렬 전에 최소값은 매번 반복문의 i번째 원소로 가정  */
		minindex = i; 
		min = a[i];
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)	//i번째 원소 이후의 원소들과 비교
		{
			if (min > a[j])	//j번째 원소가 min 보다 작은 경우
			{
				/*a[j]를 min으로 가정 */
				min = a[j];
				minindex = j;
			}
		}
		/*정렬부분 매 단계에서의 최소값과 i번째 원소 교환*/
		/*매 단계가 끝나면 앞쪽 부터 한개의 원소씩 오름차순으로 정렬*/
		a[minindex] = a[i];
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}
/*삽입 정렬*/
int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬전 배열 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];	//i번째 원소값을 저장
		j = i;		//index값을 저장

		/*정렬되지 않은 집합에서 있는 삽입할 원소와 정렬된 집합의 원소들을 비교*/
		/*삽입 할 원소의 적절한 위치 j를 구한다*/
		while (a[j-1] > t && j > 0)	
		{	
			/*정렬된 집합의 원소들을 한칸씩 뒤로 미룬다.*/
			a[j] = a[j-1];
			j--;
		}
		/*정렬된 집합에 원소 삽입*/
		a[j] = t;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 출력

	return 0;
}

/*버블 정렬*/
int bubbleSort(int *a)
{	
	int check=0;
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 출력

	/*이중 for문을 이용 첫번째 원소부터 자신보다 큰 수가 나오기 전까지 뒤로 이동*/
	/*고정된 뒤에 반복문 빠져나오는 것이 아닌 고정된 뒤의 원소들에 대해서도 버블정렬*/
	/*한 단계를 수행한뒤 다시 처음 원소부터 반복 이때 배열에서 제일 큰 수는 배열 맨뒤에 위치하므로
		범위를 줄여준다.*/
	for(i = MAX_ARRAY_SIZE; i > 0; i--)
	{
		for (j = 1; j < i; j++)
		{
			/*앞의 수가 더 클 경우 바로 뒤 원소와 교환*/
			if (a[j-1] > a[j])
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}
	
	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열 출력

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 배열 출력

	/*부분 집합의 기준이 되는 간격을 h로 지정*/
	/*간격을 반으로 줄이면서 반복*/
	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{	
		for (i = 0; i < h; i++)
		{
			/*h 간격 만큼 떨어진 원소 집합에 대해 삽입정렬*/
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				/*h간격 만큼 떨어진 두개의 원소들에 대해 삽입정렬*/
				while (k > h-1 && a[k-h] > v)
				{	/*h간격 만큼 떨어진 원소 중 뒤의 원소에 
					h간격 만큼 떨어진 원소 중 앞의 원소값 삽입*/
					a[k] = a[k-h];
					k -= h;
				}
				/*h간격 만큼 떨어진 원소 중 앞의 원소에 
					v에 저장해둔 뒤의 원소값 삽입*/
				a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1) //recursive 종료 조건
	{
		v = a[n-1];	//pivot(배열의 마지막 원소)
		/*힙정렬 시 사용할 배열의 왼쪽, 오른쪽 인덱스*/
		i = -1;		//배열의 왼쪽 인덱스 
		j = n - 1;	//배열의 오른쪽 인덱스

		while(1)
		{
			while(a[++i] < v);	//pivot보다 큰 값을 찾는다
			while(a[--j] > v);	//pivot보다 작은 값을 찾는다.
			
			/*이미 교환한 원소제외 pivot보다 큰 값이나 작은 값을 못찾을 경우*/
			if (i >= j) break;	
			/*큰 값과 작은 값을 교환*/
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		/*pivot위치 조정 왼쪽에는 pivot보다 작은값, 오른족에는 pivot보다 큰값들*/
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		/*정렬된 pivot원소값 기준 왼쪽의 원소에 대해 quicksort */
		quickSort(a, i);	
		/*정렬된 pivot원소값 기준 오른쪽 원소에 대해 quicksort*/
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

int hashCode(int key) {
	/*제산 함수를 이용한 해시 함수*/
   return key % MAX_HASH_TABLE_SIZE;
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];	//앞서 랜덤한 값을 가지는 배열로 key 값을 지정
		hashcode = hashCode(key);	//제산함수를 이용해 해시 주소를 구한다
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)	// 해당 해시주소의 버킷이 비어있을경우
		{
			hashtable[hashcode] = key;	//해당 해시주소에 key 값 삽입
		}
		/*선형 개방 주소법*/ 
		else 	{

			index = hashcode;

			while(hashtable[index] != -1)	/*다음버킷에 빈 슬롯이 있는지 조사*/
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;	// 빈 버킷에 key 삽입
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);	//찾고자하는 key의 제산함수로 해시 주소를 구한다
	int check_end=index;	//해시 테이블에 찾고자하는 key값이 없을 경우를 check

	if(ht[index] == key)		//제산함수로 구한 해시주소에 찾고자하는 key값이 있을 경우 반혼
		return index;
	//제산함수로 구한 해시주소에 찾고자하는 key 값이 없을 경우 다음 버킷을 조사하면서 탐색
	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;	
		if(check_end==index){	//처음의 해시주소로 돌아왔을 경우 (찾고자하는 수가 없는경우)
			return -1;
		}
	}
	return index;	//찾은 버킷의 주소 반환
}



