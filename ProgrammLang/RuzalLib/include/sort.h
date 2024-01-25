#ifndef SORT_DED
#define SORT_DED
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "super_io.h"
#include <assert.h>

int Comparator(const void* p1, const void* p2);
void Swap(char** a, char** b);

void QSort(char* data[], int left, int right);
int Partition(char* data[], int left, int right);

#endif // !SORT_DED
