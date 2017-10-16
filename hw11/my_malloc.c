#include "my_malloc.h"
/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you will receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* make sure this always points to the beginning of your current
 * heap space! if it does not, then the grader will not be able
 * to run correctly and you will receive 0 credit. remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it
 * however, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;

/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/**** SIZES FOR THE FREE LIST ****
 * freelist[0] -> 16
 * freelist[1] -> 32
 * freelist[2] -> 64
 * freelist[3] -> 128
 * freelist[4] -> 256
 * freelist[5] -> 512
 * freelist[6] -> 1024
 * freelist[7] -> 2048
 */


void* my_malloc(size_t size)
{
  /* FIX ME */
	//set up total size needed
	int totalSize = size + sizeof(metadata_t);

	//get index of total size need to be allocate in freelist
	int index = indexOfFreelist(totalSize);

	//if request size is 0, do nothing
	if (size == 0) {
		return NULL;
	}
	
	//if the needed size is larger than 2048, set error No, return null
	if (index == 8) {
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	
	//if heap is null, initially call my_sbrk
	if (heap == NULL) {
		heap = my_sbrk(SBRK_SIZE);
		freelist[7] = (metadata_t*) heap;
		freelist[7]->in_use = 0;
		freelist[7]->size = SBRK_SIZE;
		freelist[7]->next = NULL;
		freelist[7]->prev = NULL;
	} 
	//get pointer to free memory
	metadata_t* result = getFreeIndex(index);
	
	if (result != NULL) {
		result->in_use = 1;
	} else {
		return NULL;
	}
	ERRNO = NO_ERROR;
	return (void*) (result + 1);

}

//get index of freelist according to the size
int indexOfFreelist(int size) {
	if (size > 0 && size <= 16) {
		return 0;
	} else if (size <= 32) {
		return 1;
	} else if (size <= 64) {
		return 2;
	} else if (size <= 128) {
		return 3;
	} else if (size <= 256) {
		return 4;
	} else if (size <= 512) {
		return 5;
	} else if (size <= 1024) {
		return 6;
	} else if (size <= 2048) {
		return 7;
	} else {
		return 8;
	}
}
//return thepointer to the free memory
metadata_t* getFreeIndex(int index) {
	metadata_t* result;
	//if freelist at index has free space, allocate it
	if (freelist[index] != NULL) {
		result = freelist[index];
		metadata_t* newHead = result->next;
		freelist[index] = newHead;
		if (newHead != NULL) {
			newHead->prev = NULL;
		}
		return result;
	}
	//if we cannot find free space at index 7, we need to call my_sbrk to allocate more space
	if (index == 7) {
		result = (metadata_t*) my_sbrk(SBRK_SIZE);
		if (result == NULL) {
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		result->size = SBRK_SIZE;
		return result;
	}
	//get pointer to free block
	result = getFreeIndex(index + 1);
	//if pointer does not exist return null
	if (result == NULL) {
		return NULL;
	}
	metadata_t* ptr = split(result, index);
	return ptr;
}

//split free space to get a right fit one and set the other onto the freelist
metadata_t* split(metadata_t* ptr, int index) {
	
	metadata_t* split1;
	metadata_t* split2;
	short half = ptr->size / 2;

	split1 = ptr;
	split1->size = half;
	split1->prev = NULL;
	split1->next = NULL;
	split1->in_use = 1;

	split2 = (metadata_t*) ((char*) split1 + ptr->size);
	split2->size = split1->size;
	split2->prev = NULL;
	split2->in_use = 0;
	
	split2->next = freelist[index];
	freelist[index] = split2;

	return split1;
}
//automatically allocate entire array at a time
void* my_calloc(size_t num, size_t size)
{
  /* FIX ME */
	void* allocMem = my_malloc(num * size);
	if (allocMem != NULL) {
		unsigned char* pointer = allocMem;
		while (size--) {
			*pointer++ = (unsigned char) 0;
		}
	}
  	return allocMem;
}

//deallocate buddy system
void my_free(void* ptr)
{
  /* FIX ME */
	//if heap is null, do nothing
	if (heap == NULL) {
		return;
	}

	metadata_t* temp;
	//set pinter pint to metadatas
	temp = (metadata_t*)((char*)ptr - sizeof(*temp));
	temp = removeMerge(temp);
	//put pointer back to freelist
	int index = indexOfFreelist(temp->size);
	metadata_t* oldPtr = freelist[index];
	if(oldPtr) {
		oldPtr->prev = temp;
	}
	temp->prev = NULL;
	temp->next = oldPtr;
	temp->in_use = 0;
	freelist[index] = temp;
	ERRNO = NO_ERROR;
	return;
}

//merge free buddy from freelist
metadata_t* removeMerge(metadata_t* ptr)
{
	int index = indexOfFreelist(ptr->size);
	if (index == 7) {
		return ptr;
	}
	metadata_t* temp1 = ptr;
	temp1->in_use = 0;
	//get buddy of temp1
	metadata_t* temp2 = getBuddy(ptr);
	if (temp2->size == temp1->size && (!temp1->in_use && !temp2->in_use)) {
		//remove temp2 from heap
		if (temp2->prev == NULL)
		{
			freelist[index] = temp2->next;
			if(temp2->next) {
			 temp2->next->prev = NULL;
			 }
		} else {
			temp2->prev->next = temp2->next;
			if (temp2->next) {
				temp2->next->prev = temp2->prev;
			}
			temp2->prev = NULL;
			temp2->next = NULL;
		}
		// make temp1 has the smaller memory address
		// swap if temp1 is not the smaller one
		if ((long)temp1 - (long)temp2 > 0)
		{
			metadata_t* temp = temp2;
			temp2 = temp1;
			temp1 = temp;
		}
		temp1->prev = NULL;
		temp1->next = NULL;
		temp1->size = (temp1->size)*2;
		temp1 = removeMerge(temp1);
	}
	return temp1;
}

//get the pointer to a buddy
metadata_t* getBuddy(metadata_t* ptr) {

	int size = ptr->size;
	// int n = (int)log2(size);
	int n = indexOfFreelist(size);
	long mask = 1 << n;
	//subtract heap from pointer
	metadata_t* temp = (metadata_t*)((long)ptr - (long)heap);
	metadata_t* buddy = temp;
	//
	buddy = (metadata_t*)((long)buddy ^ mask);
	//put buddy on heap
	buddy = (metadata_t*)((long)buddy + (long)heap);

	return buddy;
}


void* my_memmove(void* dest, const void* src, size_t num_bytes) {

	char* s = (char*) src;
	char* d = (char*) dest;
	char temp[num_bytes];

	int i = 0;
	while (i < num_bytes) {
		temp[i] = s[i];
		i++;
	}
	int j = 0;
	while (j < num_bytes) {
		d[j] = temp[j];
		j++;
	}
	ERRNO = NO_ERROR;
	return d;
}




