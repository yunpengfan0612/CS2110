#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Here we are going to write some functions to support a linked list that stores
 * Person data (name, age)
 */
typedef struct person_t
{
    char* name;
    int age;
} Person;

/* Example functions given to you. If you want to truly grasp these concepts, try
 * writing your own structs and functions as well!
 */

// Create a new Person
Person* create_person(const char* name, int age)
{
    Person* p = (Person*) malloc(sizeof(Person));
    p->name = strdup(name);
    p->age = age;
    return p;
}

// Make a deep copy of a Person
void* copy_person(const void* data)
{
    Person *p = (Person*) data;
    return create_person(p->name, p->age);
}

// Print a Person
void print_person(void* data)
{
    Person *p = (Person*) data;
    printf("%s, %d\n", p->name, p->age);
}

// Free a Person
void free_person(void* data)
{
    // This is safe because we should only be passing in Person struct pointers
    Person *p = (Person*) data;
    // free any malloc'd pointers contained in the Person struct (just name)
    free(p->name);
    // Now free the struct itself; this takes care of non-malloc'd data, like age.
    free(p);
}

// Return 1 if the person's name is 8+ characters long
int long_name(const void *data)
{
	Person *p = (Person*) data;
	return strlen(p->name) > 7;
}

// Return 1 if the person's name is 6- characters long
int short_name(const void *data)
{
	Person *p = (Person*) data;
	return strlen(p->name) < 7;
}

/* This main function does a little testing
   Like all good CS Majors you should test
   your code here. There is no substitute for testing
   and you should be sure to test for all edge cases
   e.g., calling remove_front on an empty list.
*/
int main(void)
{
	/* Now to make use of all of this stuff */
	list* llist = create_list();

  	/* What does an empty list contain?  Lets use our handy traversal function */
  	printf("TEST CASE 1\nAn Empty list should print nothing here:\n");
  	traverse(llist, print_person);
  	printf("\n");

 	/* Lets add a person and then print */
 	push_front(llist, create_person("Andrew", 24));
  	printf("break\n");
 	printf("TEST CASE 2\nA List with one person should print that person:\n");
 	traverse(llist, print_person);
 	printf("\n");

 	/* Lets remove that person and then print */
 	remove_front(llist, free_person);
 	printf("TEST CASE 3\nAnother Empty list should print nothing here:\n");
 	traverse(llist, print_person);
 	printf("\n");

 	/* Lets add two people and then print */
 	push_front(llist, create_person("Nick", 22));
 	push_front(llist, create_person("Randal", 21));
 	printf("TEST CASE 4\nA List with two people should print those two people:\n");
 	traverse(llist, print_person);
 	printf("\n");

	/* Lets copy this list */
	list* llist2 = copy_list(llist, copy_person);
	printf("TEST CASE 5\nA copied list should print out the same two people:\n");
 	traverse(llist2, print_person);
 	printf("\n");

  	/* Lets kill the list */
  	empty_list(llist, free_person);
 	printf("TEST CASE 6\nAfter freeing all nodes the list should be empty:\n");
 	traverse(llist, print_person);
	printf("\n");

	/* Let's make a list of people, and remove certain ones! */
	/* Should remove anyone whose name is 8+ characters long */

	push_front(llist, create_person("Josephine", 27));
	push_front(llist, create_person("Dave", 34));
	push_front(llist, create_person("Benjamin", 23));
	push_front(llist, create_person("Lisa", 41));
	push_front(llist, create_person("Maximilian", 24));
	remove_if(llist, long_name, free_person);
	printf("TEST CASE 7\nShould only print 2 people with short names:\n");
	traverse(llist, print_person);
	printf("\n");

 	/* YOU ARE REQUIRED TO MAKE MORE TEST CASES THAN THE ONES PROVIDED HERE */
 	/* You will get points off if you do not you should at least test each function here */

 	/* remove one person from back*/
	remove_back(llist, free_person);
	printf("TEST CASE 8\nShould only print 1 people with short names:\n");
	traverse(llist, print_person);
	printf("\n");

	/* remove the last one person from back*/
	remove_back(llist, free_person);
	printf("TEST CASE 9\nAn Empty list should print nothing here:\n");
	traverse(llist, print_person);
	printf("\n");

	/* test is_empty function*/
	printf("TEST CASE 10\nShould print 1 here: %d\n", is_empty(llist));
	printf("\n");

	/* test remove_front*/
	push_front(llist, create_person("Marissa", 18));
	push_front(llist, create_person("Katherine", 18));
	remove_front(llist, free_person);
	printf("TEST CASE 11-a\nShould print out Marissa:\n");
	traverse(llist, print_person);
	printf("\n");

	remove_front(llist, free_person);
	printf("TEST CASE 11-b\nAn Empty list should print nothing here:\n");
	traverse(llist, print_person);
	printf("\n");

	printf("TEST CASE 11-c\nShould print -1 here: %d\n", remove_front(llist, free_person));
	printf("\n");

	/* test push_front list*/
	push_front(llist, create_person("Marissa", 18));
	printf("TEST CASE 12-a\nShould print out Marissa:\n");
	traverse(llist, print_person);
	printf("\n");
	push_front(llist, create_person("Katherine", 18));
	printf("TEST CASE 12-b\nShould print out Katherine and Marissa :\n");
	traverse(llist, print_person);
	printf("\n");

	/* test empty_list*/
	empty_list(llist, free_person);
 	printf("TEST CASE 13\nAfter freeing all nodes the list should be empty:\n");
 	traverse(llist, print_person);
	printf("\n");
	
	/* test push_back list*/
	push_back(llist, create_person("Marissa", 18));
	printf("TEST CASE 14-a\nShould print out Marissa:\n");
	traverse(llist, print_person);
	printf("\n");
	push_back(llist, create_person("Katherine", 18));
	printf("TEST CASE 14-b\nShould print out Marissa and Katherine :\n");
	traverse(llist, print_person);
	printf("\n");

	/* test remove_back*/
	remove_back(llist, free_person);
	printf("TEST CASE 15-a\nShould print out Marissa:\n");
	traverse(llist, print_person);
	printf("\n");

	remove_back(llist, free_person);
	printf("TEST CASE 15-b\nAn Empty list should print nothing here:\n");
	traverse(llist, print_person);
	printf("\n");

	/* test copy_list*/
	push_front(llist, create_person("Marissa", 18));
	push_front(llist, create_person("Katherine", 18));
	printf("TEST CASE 16-a\nThe size of llist should be 2 : %d\n", llist->size);
 	printf("\n");
	list* llist3 = copy_list(llist, copy_person);
	printf("TEST CASE 16-b\nA copied list should print out the same two people: Katherine and Marissa\n");
 	traverse(llist3, print_person);
 	printf("\n");

 	/* test to get back and front data from list*/
 	printf("TEST CASE 17-a\nThe back data of llist should be Marissa : \n");
 	print_person(back(llist));
 	printf("\n");
 	printf("TEST CASE 17-b\nThe front data of llist should be Katherine : \n");
 	print_person(front(llist));
 	printf("\n");
 	printf("TEST CASE 17-c\nTest for front and back with a list of size 0\nShould print two \"NULL\":\n");
 	empty_list(llist, free_person);
  	if(!front(llist))
  		printf("NULL\n");
  	if(!back(llist))
  		printf("NULL\n");
  	printf("\n");

 	/* test remove_if*/
 	empty_list(llist, free_person);
 	push_back(llist, create_person("Jose", 27));
	push_back(llist, create_person("Dave", 34));
	push_back(llist, create_person("Benjamin", 23));
	push_back(llist, create_person("Lisa", 41));
	push_back(llist, create_person("Max", 24));
	remove_if(llist, long_name, free_person);
	printf("TEST CASE 18-a\nShould only print 4 people with short names:\n");
	traverse(llist, print_person);
	printf("\n");
	remove_if(llist, short_name, free_person);
	printf("TEST CASE 18-b\nShould print an empty list :\n");
	traverse(llist, print_person);
	printf("\n");

 	/* Testing over clean up*/
	empty_list(llist, free_person);
 	free(llist);
	empty_list(llist2, free_person);
	free(llist2);
	empty_list(llist3, free_person);
 	free(llist3);

  	return 0;
}

