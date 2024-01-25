#include "../include/sort.h"


int Comparator(const void* p1, const void* p2)
{
	char* s1 = *(char**)p1;
	char* s2 = *(char**)p2;

	return strcmp(s1, s2);
}

void Swap(char** a, char** b) {
	char* tmp = *a;
	*a = *b;
	*b = tmp;
}

void QSort(char* data[], int left, int right) {
	//printf("\nQSORT FROM: %d TO: %d\n", left, right);
	if (left >= right) return;

	int mid = Partition(data, left, right);
	QSort(data, left, mid-1);
	QSort(data, mid+1, right);
	
}

int Partition(char* data[], int left, int right) {
	int mid = left + (rand() % (right - left + 1));
	char* pivot = data[mid];
	int size = right - left + 1;

	while (left < right) {

		//DebugPrint(data, left, right, size, mid);
		if (Comparator(&data[left], &data[right]) == 0) left++;

		while (Comparator(&data[left], &pivot) == -1) {
			left++;
			//DebugPrint(data, left, right, size, mid);
		}

		while (Comparator(&data[right], &pivot) == 1) {
			right--;
			//DebugPrint(data, left, right, size, mid);
		}

		if (left < right) Swap(data + left, data + right);
		//DebugPrint(data, left, right, size, mid);
	}
	return right;
}