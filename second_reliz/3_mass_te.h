//this file not used in this project. this file do for study

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "common.h"

struct mass1 {
	struct list* str;
};
/*
struct list {
	struct element* head;
	struct element* tail;
};
*/
/*
struct element {
	struct element* next;
	struct element* prev;
	size_t row;
	size_t column;
	int value; // maybe size_t
};
*/

struct list1 {
	struct node* head;
	struct node* tail;
};

struct node1 {
	struct node* next;
	struct node* prev;
	struct data* data;
};

struct data1 {
	size_t row;
	size_t column;
	int value;
};

#ifndef ALLOC_ARR
#define ALLOC_ARR(arr, count) arr = malloc((count) * sizeof((arr)[0]))
#endif
#ifndef ALLOC_PTR
#define ALLOC_PTR(ptr) ALLOC_ARR(ptr,1)
#endif

//enum { EMPTY, ONE, MORE, ERROR = -1};

void mass_ini1(struct mass1* vmass)
{
	struct list1* t = 0;
	ALLOC_PTR(t);
	assert(t > 0);

	t->head = 0;
	t->tail = 0;

	vmass->str = t;
}

int invariant1(struct mass1* vmass)
{
	if (vmass->str->head == vmass->str->tail && vmass->str->tail == 0)
		return EMPTY;
	else if (vmass->str->head == vmass->str->tail && vmass->str->tail != 0)
		return ONE;
	else if (vmass->str->head != vmass->str->tail && vmass->str->tail != 0)
		return MORE;
	else {
		perror("some error_invariant");
		return ERROR;
	}
}

int mass_add1(struct mass1* vmass, struct data1* vdata)
{
//  	assert(elem->next == 0);
//	assert(elem->prev == 0);

	int rc = invariant(vmass);

	if (rc == MORE) {
		struct node1* temp = 0;
		ALLOC_PTR(temp);
		assert(temp > 0);

		temp->next = 0;
		temp->prev = vmass->str->tail;
		temp->data = vdata;

		vmass->str->tail->next = temp;
		vmass->str->tail = temp;

		assert(vmass->str->tail->next == 0);
		return 0;
	}
	else if (rc == ONE) {
		struct node1* temp = 0;
		ALLOC_PTR(temp);
		assert(temp > 0);

		temp->next = 0;
		temp->prev = vmass->str->tail;
		temp->data = vdata;

		vmass->str->head->next = temp;
		vmass->str->tail = temp;

		assert(vmass->str->head->prev == 0);
		assert(vmass->str->tail->next == 0);
		assert(vmass->str->tail != vmass->str->head);

		return 0;
	}
	else if (rc == EMPTY) {
		struct node1* temp = 0;
		ALLOC_PTR(temp);
		assert(temp > 0);

		temp->next = 0;
		temp->prev = 0;
		temp->data = vdata;

		vmass->str->head = temp;
		vmass->str->tail = vmass->str->head;

		return 0;
	}
	else if (rc == ERROR) {
		return ERROR;
	}
	else {
		perror("error in add");
		return ERROR;
	}
	perror("error add");
	return ERROR;
}

//find
//@return -1 - error, -2 - no element
struct node* mass_find1(struct mass1* vmass, struct data1* vdata)
{
	int rc = invariant(vmass);

	if (rc == EMPTY) {
		return (struct node1*)ERROR;
	}
	else if (rc == MORE) {
		struct node1* temp = vmass->str->head;
		while (temp) {
			if(temp->data == vdata) { // maybe error

				return temp;
			}
			temp = temp->next; // need to test
			                   //finding last element
		}

		temp = 0;
		return (struct node1*)ERROR;
	}
	else if (rc == ONE) {
		if(vmass->str->head->data == vdata) { // maybe error
			return vmass->str->head;
		}
		else 
			return (struct node1*)ERROR;
	}
	else if (rc == ERROR) {
		return (struct node1*)ERROR;
	}
	else {
		perror("error in add");
		return (struct node1*)ERROR;
	}
	perror("error add");
	return (struct node1*)ERROR;
}

//in this struct we don`t need insert
int mass_insert1(struct mass1* vmass, struct data1* given,
		struct data1* after)
{
	struct node1* temp = 0;
	temp = mass_find1(vmass, after);
	assert(temp != (struct node1*)ERROR);

	//need to check
	//maybe this expression change
	if (temp == (struct node1*)ERROR)
		return ERROR;
	else if (temp == vmass->str->tail) {
		int rc = mass_add(vmass, given);
		assert(rc == 0);

		return 0; //success
	}
	else { //more than one, not in tail
		assert(temp > 0);

		struct node1* node = 0;
		ALLOC_PTR(node);
		assert(node > 0);

		node->prev = temp;
		node->next = temp->next;
		node->data = given;

		temp->next->prev = node;
		temp->next = node;

		return 0;
	}
	perror("error add");
	return ERROR;
}

void mass_rm(struct mass1* vmass, struct data1* elem)
{
	struct node1* temp;
	temp = mass_find1(vmass, elem);
	assert(temp != (struct node1*)ERROR);

	//maybe this expression change
	if (temp == (struct node1*)ERROR) {
		perror("error rm");
		return;
	}
	else if (temp == vmass->str->tail) {
		if(vmass->str->head != vmass->str->tail) {
			vmass->str->tail = vmass->str->tail->prev;
			vmass->str->tail->next = 0;

			temp->next = 0;
			temp->prev = 0;
			if(temp->data != 0) {
				free(temp->data);
				temp->data = 0;
			}			
			if(temp != 0) {
				free(temp);
				temp = 0;
			}
			return;
		}
		else { //head == tail
			vmass->str->head->prev = 0;
			vmass->str->head->next = 0;

			if(vmass->str->head->data != 0) {
				free(vmass->str->head->data);
				vmass->str->head->data = 0;
			}			
			if(vmass->str->head != 0) {
				free(vmass->str->head);
				vmass->str->head = 0;
			}
			vmass->str->tail->next = 0; //need because some magic
			vmass->str->tail = 0;

			assert(vmass->str->head == vmass->str->tail);
			temp = 0;
			return;
		}
/*
		vmass->str->tail = vmass->str->tail->prev;
		vmass->str->tail->next = 0;
*/

	}
	else if (temp == vmass->str->head) {
		vmass->str->head = vmass->str->head->next;
		vmass->str->head->prev = 0;

		temp->next = 0;
		temp->prev = 0;

		if(temp->data != 0) {
			free(temp->data);
			temp->data = 0;
		}
		if (temp) {
			free(temp);
			temp = 0;
		}
		return;
	}
	else { //more than one
		temp->next->prev = temp->prev;
		temp->prev->next = temp->next;

		temp->prev = 0;
		temp->next = 0;

		if(temp->data != 0) {
			free(temp->data);
			temp->data = 0;
		}
		if (temp) {
			free(temp);
			temp = 0;
		}
		return;
	}
	perror("error rm");
	return;
}

void mass_fini1(struct mass1* vmass)
{
	while(vmass->str->head) {
	  //		printf("test fini\n");
		mass_rm(vmass, vmass->str->tail->data);
	}
}

void mass_print1(struct mass1* vmass)
{
	if(vmass->str->head == 0)
		return;
	else {
		struct node1* temp = 0;
		temp = vmass->str->head;
		while (temp) {
			printf("row = %zu, column = %zu, value = %i", 
			       temp->data->row, temp->data->column,
			       temp->data->value);
			temp = temp->next;
		}
	}
	printf("\n");
}

/*
int main(int argc, char** argv)
{
  	struct mass* m;
	ALLOC_PTR(m);
	assert(m != 0);

	mass_ini(m);

*/
/*
//@ 0
int mass_add(struct mass* vmass, struct data* vdata)

//@ERROR, 0
int mass_insert(struct mass* vmass, struct data* given,
		struct data* after)

void mass_rm(struct mass* vmass, struct data* elem)
void mass_fini(struct mass* vmass)
void mass_print(struct mass* vmass)
*/
/*
	struct data* t1 = 0;
	ALLOC_PTR(t1);
	assert(t1 > 0);
	t1->row = 1;
	t1->column = 1;
	t1->value = 1;

	int rc = mass_add(m, t1);
	assert(rc == 0);

	mass_print(m);

	struct data* t2 = 0;
	ALLOC_PTR(t2);
	assert(t2 > 0);
	t2->row = 2;
	t2->column = 2;
	t2->value = 2;

	struct data* t3 = 0;
	ALLOC_PTR(t3);
	assert(t3 > 0);
	t3->row = 3;
	t3->column = 3;
	t3->value = 3;

	struct data* t4 = 0;
	ALLOC_PTR(t4);
	assert(t4 > 0);
	t4->row = 4;
	t4->column = 4;
	t4->value = 4;

	rc = mass_add(m, t2);
	assert(rc == 0);

	rc = mass_add(m, t3);
	assert(rc == 0);

	mass_print(m);

	rc = mass_insert(m, t4,	t3);
	assert(rc == 0);
	//check in end and after head

	mass_print(m);
	//____________________worked^^
	/*
	  void mass_rm(struct mass* vmass, struct data* elem)
	  void mass_fini(struct mass* vmass)
	 */

	//	mass_rm(m, t3);
	/*
	mass_print(m);

	//___________________deleting
	mass_fini(m);             //g
	if(m->str != 0) {         //g
		free(m->str);     //g
		m->str = 0;       //g
	}                         //g
	if (m != 0) {             //g
		free(m);          //g
		m = 0;            //g
	}                         //g
	//___________________deleting

	return 0;
}
*/
