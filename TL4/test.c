#include "list.h"
#include <stdio.h>

// Your name here

// Test your list code here! Make sure you test all the cases!
int main()
{
	LIST* llist = create_list();

	push_front(llist, 1);

	// Your tests here

	//1. test push one element: 1
	printf("break 1\n");
	print_list(llist);
	printf("\n");

	//2. test push more element: 3, 2, 1
	printf("break 2\n");
	push_front(llist, 2);
	push_front(llist, 3);
	print_list(llist);
	printf("\n");

	//3. test don't push same number :3 2 1 
	printf("break 3\n");
	push_front(llist, 1);
	print_list(llist);
	printf("\n");

	//4. test remove one element: 3 2
	printf("break 4\n");
	printf("%d\n", llist->tail->data);
	int i = pop_back(llist);
	printf("%d\n", i);//should print 1
	printf("\n");


	print_list(llist);
	printf("\n");

	//5. test remove all elements: 
	printf("break 5\n");
	int k = pop_back(llist);
	k = pop_back(llist);
	printf("%d\n", k);//should print 3

	print_list(llist);
	printf("\n");//should print nothing

	//6. test get
	printf("break 6\n");
	push_front(llist, 1);
	push_front(llist, 2);
	push_front(llist, 3);
	push_front(llist, 4);

	int j = peek_index(llist, 1);//should print 3
	printf("%d\n", j);//should print 3

	// pop_back(llist);
	// pop_back(llist);
	// pop_back(llist);
	// pop_back(llist);


	// free(llist);
	return 0;
}
