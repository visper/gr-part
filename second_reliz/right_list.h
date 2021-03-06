//the best realisation of list

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

//#include "common.h"

#ifndef ALLOC_ARR
#define ALLOC_ARR(arr, count) ((arr) = malloc((count) * sizeof ((arr)[0])))
#endif
#ifndef ALLOC_PTR
#define ALLOC_PTR(ptr) ALLOC_ARR(ptr, 1)
#endif

//enum { EMPTY, ONE, MORE, ERROR = -1};

struct vertex {
	int weight;
};

struct vertex_list_node {
	struct vertex_list_node* next;
	struct vertex_list_node* prev;
	struct vertex* data;
};

struct vertex_list {
	struct vertex_list_node* head;
	struct vertex_list_node* tail;
};

/*
void vertex_list_ini(struct vertex_list* vlist);
void vertex_list_fini(struct vertex_list* vlist);
int vertex_list_add(struct vertex_list* vlist, struct vertex* vertex);
int vertex_list_ins(struct vertex_list* vlist, struct vertex* given,
		    struct vertex* after);
void vertex_list_rm(struct vertex_list* vlist, struct vertex* vertex);
//struct vertex_list_node* vertex_list_find(struct vertex_list* vlist, 
//					  struct vertex* find);
int invariant(struct vertex_list* vlist);
void vertex_list_print(struct vertex_list* vlist);
*/

void vertex_list_node_del(struct vertex_list_node* node)
{
	assert(node->prev == NULL);
	assert(node->next == NULL);

	if (node->data != NULL) {
		free (node->data);
		node->data = NULL;
	}
	if (node != NULL) {
		free (node);
		node = NULL;
	}
	//	printf("del success\n");
}

//identify state of list
//@0 if empty, @1 if only one element, @2 if more than 1 elem
int vertex_list_state(const struct vertex_list* vlist)
{
	if (vlist->head == NULL && vlist->tail == NULL)
		return EMPTY;
	else if (vlist->head == vlist->tail && vlist->head != NULL)
		return ONE;
	else if (vlist->head != vlist->tail && vlist->head != NULL
		 && vlist->tail != NULL)
		return MORE;
	else
		return ERROR;
}

bool invariant(const struct vertex_list* vlist)
{
	return vertex_list_state(vlist) != ERROR;//maybe =
}

void vertex_list_ini(struct vertex_list* vlist)
{
	vlist->head = NULL;
	vlist->tail = NULL;
}

 //maybe vertex is const, but warning
int vertex_list_add(struct vertex_list* vlist, struct vertex* vertex)
{
	int rc = vertex_list_state(vlist);

	struct vertex_list_node* node;
	ALLOC_PTR(node);
	if (node == 0)
		return -ENOMEM;
	
	node->next = NULL;
	node->data = vertex;

	if (rc == MORE || rc == ONE) {
		node->prev = vlist->tail;

		vlist->tail->next = node;
		vlist->tail = node;

		assert(invariant(vlist));
		//if assert must be del, maybe this check or delete invariant
		if (invariant(vlist))
			return 0;
		else 
			return ERROR;
	}
	if (rc == EMPTY) {
		node->prev = NULL;

		vlist->head = node;
		vlist->tail = node;

		assert(invariant(vlist));
		return 0;
	}
	node->data = NULL;
	node->prev = NULL;
	vertex_list_node_del(node);

	perror("no one case is not apply");
	return -EINVAL; // errno
} // -9 lines

//maybe I have reason to rewrite this fuu, to find from tail,
//because it is beter for fini
struct vertex_list_node* vertex_list_find(const struct vertex_list* vlist, 
					  const struct vertex* find)
{
	struct vertex_list_node* scan = NULL;

	for (scan = vlist->head; scan != NULL; scan = scan->next)
		if (scan->data == find)
			return scan;

	return NULL;
}

//one of "after" or "node" must be equal NULL
//in this function situation like in rm
//maybe given, node are const, but warning
int vertex_list_ins(struct vertex_list* vlist, struct vertex* given,
		    const struct vertex* after)
{
	//case in what we have only after, and we must find it 
	struct vertex_list_node* temp = NULL;
	temp = vertex_list_find(vlist, after);
	if (temp == NULL) {
		perror("can`t find element");
		return ERROR;
	}
	
	if (temp == vlist->tail) {
		//fuu of add to tail may to write singly
		int rc = vertex_list_add(vlist, given);
		return rc != 0 ? rc : 0;
	}
	struct vertex_list_node* elem_ins;
	ALLOC_PTR(elem_ins);
	if (elem_ins == 0)
		return -ENOMEM;	

	elem_ins->data = given;
	elem_ins->prev = temp;
	elem_ins->next = temp->next;

	temp->next->prev = elem_ins;
	temp->next = elem_ins;
		
	return 0;
}

void vertex_list_print(const struct vertex_list* vlist)
{
	printf("[ ");

	struct vertex_list_node* print = NULL;
	for(print = vlist->head; print != NULL; print = print->next)
		printf("%i ", print->data->weight);

	printf("]\n");
}

void vertex_list_rm(struct vertex_list* vlist, struct vertex* vertex)
{
	int rc = vertex_list_state(vlist); 
	if (rc == EMPTY || rc == ERROR)
		return;

	struct vertex_list_node* temp = NULL;
	temp = vertex_list_find(vlist, vertex);
	assert(temp != NULL);
	if (temp == NULL)
		return;

	if (temp == vlist->head) {
		if (vlist->head->next != NULL) {
			vlist->head = vlist->head->next;
			vlist->head->prev = NULL;
		}
		else { // only 1 element
			vlist->tail = NULL;
			vlist->head = NULL;
		}
		
		temp->next = NULL;
		vertex_list_node_del(temp);
	}
	//here some error
	else if (temp == vlist->tail) {
		if (vlist->tail->prev != NULL) {
			vlist->tail = vlist->tail->prev;
			vlist->tail->next = NULL;
		}
			
		temp->prev = NULL;
		vertex_list_node_del(temp);
		if (vertex_list_state(vlist) == ONE) {
			vlist->head->next == NULL;
			vlist->tail == vlist->head;
		}
	}
	else { // more than one element
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
			
		//delete node
		temp->prev = NULL;
		temp->next = NULL;
		vertex_list_node_del(temp);
	}

	assert(invariant(vlist));
}

void vertex_list_fini(struct vertex_list* vlist)
{
	while(vlist->head)
		vertex_list_rm(vlist, vlist->tail->data);
}

/*
int main(int argc, char** argv)
{

	struct vertex_list* lst;
	ALLOC_PTR(lst);
	if (lst == 0)
		return -ENOMEM;

	vertex_list_ini(lst);
	vertex_list_print(lst);

	//invariant test
	int rc = invariant(lst);
	printf("invariant of only ini list %i\n", rc);
	//end test of invariant
//**************************************************
	struct vertex* t1;
	ALLOC_PTR(t1);
	if (t1 == 0)
		return -ENOMEM;
	t1->weight = 3;
	rc = vertex_list_add(lst, t1);
	if(rc != 0)
		return rc;
	printf("weight: %i\n", lst->head->data->weight);

	//start________________test of insert and find
	struct vertex* t2;
	ALLOC_PTR(t2);
	if (t2 == 0)
		return -ENOMEM;
	t2->weight = 4;

	struct vertex* t3;
	ALLOC_PTR(t3);
	if (t3 == 0)
		return -ENOMEM;
	t3->weight = 5;

	struct vertex* ins;
	ALLOC_PTR(ins);
	if (ins == 0)
		return -ENOMEM;
	ins->weight = 6;

	printf("must be only one element\n");
	vertex_list_print(lst);
	//ins without add
	rc = vertex_list_ins(lst, ins, t2);
	if(rc == ERROR) {
		printf("can not find element or error(good)\n");
	}
	printf("must be only one element\n");
	vertex_list_print(lst);

	rc = vertex_list_add(lst, t2);
	if(rc != 0)
		return rc;

	//ins in head
	rc = vertex_list_ins(lst, t3, lst->head->data); //error
	if(rc != 0)
		return rc;

	struct vertex* t7;
	ALLOC_PTR(t7);
	if (t7 == 0)
		return -ENOMEM;
	t7->weight = 7;

	//ins in center
	rc =  vertex_list_ins(lst, t7, t2);
	if(rc != 0)
		return rc;

	//ins in tail
	struct vertex* t8;
	ALLOC_PTR(t8);
	if (t8 == 0)
		return -ENOMEM;
	t8->weight = 8;

	//ins in center
	rc =  vertex_list_ins(lst, t8, lst->tail->data);
	if(rc != 0)
		return rc;

	printf("all add and ins\n");
	vertex_list_print(lst);
	//end__________________test of insert

	//start test rm____________
	//rm from center
	vertex_list_rm(lst, t2);
	printf("remove center\n");
	vertex_list_print(lst);
	//rm from tail
	vertex_list_rm(lst, lst->tail->data);
	printf("remove tail\n");
	vertex_list_print(lst);

	//rm from head
	vertex_list_rm(lst, lst->head->data);
	printf("remove head\n");
	vertex_list_print(lst);

	printf("remove list\n");
	vertex_list_fini(lst);

	if(lst->head == lst->tail && lst->head == 0) {
		free(lst);
		lst = 0;
		printf("good exit, mem is free\n");
	}
	else
		perror("error fini");
	//end test rm____________
	return 0;
}
*/
