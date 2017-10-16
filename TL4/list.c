#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This function will create a new list and return a pointer to that list.
 */
LIST* create_list()
{
	LIST* newList;
	if ((newList = malloc(sizeof(LIST))) == NULL) {
		return NULL;
	}
	newList->head = NULL;
	newList->tail = NULL;
	newList->size = 0;
	return newList;
}

/**
 * This function will create a node from the given data and return a pointer to that node.
 */
NODE* create_node(int data)
{
	NODE* newNode;
	if ((newNode = malloc(sizeof(NODE))) == NULL) {
      return NULL;
    }
	newNode->next = NULL;
	newNode->data = data;
	return newNode;
}

/**
 * This function will add the given data to the head of the linked list (llist), if the data is not already contained 
 * in the linked list.
 */
void push_front(LIST* llist, int data)
{
	NODE* newNode;
	newNode = create_node(data);
	if (llist->size == 0) {
		//see if necessary to point head to tail
		llist->head = newNode;
		llist->tail = newNode;
		
	} else {
		NODE* cur;
		cur = llist->head;
		while (cur) {
			if (cur->data == data) {
				return;
			}
			cur = cur->next;
		}
		newNode->next = llist->head;
		llist->head = newNode;
	}
	llist->size++;
}

/**
 * This function will return the data located at the given index in the list (llist)
 * i.e. for a list of 5,7,8,4,9,3 peek_index(llist, 2) would give me the value 8.
 * If the index is out of bounds, return -1.
 */
int peek_index(LIST* llist, int index)
{	
	if (index < 0 || index >= llist->size) {
		return -1;
	} else {
		NODE* newNode = llist->head;
		int i;
		for (i = 0; i < index; i++) {
			newNode = newNode->next;
		}
		return newNode->data;
	}
	
}

/**
 * Removes the last node from the tail of the list and returns the data from the removed node to the user.
 * If the list is empty, return -1.
 */
int pop_back(LIST* llist)
{	
	if (llist->size == 0) {
		return -1;
	} else {
		int ret;
		if (llist->size == 1) {
			ret = llist->head->data;
			free(llist->tail);
			llist->head = NULL;
			llist->tail = NULL;			
		} else {
			ret = llist->tail->data;

			NODE* cur = llist->head;
			

			NODE* temp = cur;
			
			while(cur) {
				temp = cur;
				cur = cur->next;
			}
			llist->tail = temp;
			temp->next = NULL;
			free(cur);

            // temp = llist->tail->prev;
            // free_func(llist->tail->data);
            // free(llist->tail);
            // llist->tail = temp;
            // llist->tail->next = NULL;
			// NODE* cur = llist->head;
			// NODE* prev;
			// ret = cur->data;
			// while (cur) {
			// 	prev = cur;
			// 	cur = cur->next;
			// }
			// // printf("break\n");
			// prev->next = NULL;
			// llist->tail = prev;
			// free(cur);
			

		}
		llist->size--;
		return ret;
	}
}

/** 
 * This function will print out the entire list in a nicely formatted way. Do whatever you want, 
 * just try to make it easy to read.
 */
void print_list(LIST* llist)
{
	NODE* cur;
	cur = llist->head;
	while(cur) {
		printf("%d \n", cur->data);
		cur = cur->next;
	}
}
