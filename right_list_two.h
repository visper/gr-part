#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#define ALLOC_ARR(arr, count) ((arr) = malloc((count) * sizeof ((arr)[0])))
#define ALLOC_PTR(ptr) ALLOC_ARR(ptr, 1)

enum { EMPTY, ONE, MORE, ERROR = -1};

struct vertex2 {
	int column;
	int values;
};

struct vertex_list_node2 {
	struct vertex_list_node2* next;
	struct vertex_list_node2* prev;
	struct vertex2* data;
};

struct vertex_list2 {
	struct vertex_list_node2* head;
	struct vertex_list_node2* tail;
};

/*
void vertex_list_ini(struct vertex_list* vlist);
void vertex_list_fini(struct vertex_list* vlist);
int vertex_list_add(struct vertex_list* vlist, struct vertex* vertex);

//in this case I can`t insert element before head
int vertex_list_ins(struct vertex_list* vlist, struct vertex* given,
		    struct vertex* after);
void vertex_list_rm(struct vertex_list* vlist, struct vertex* vertex);
struct vertex_list_node* vertex_list_find(struct vertex_list* vlist, 
					  struct vertex* find);
int invariant(struct vertex_list* vlist);
void vertex_list_print(struct vertex_list* vlist);
*/

void vertex_list_node_del2(struct vertex_list_node2* node)
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
}

//identify state of list
//@0 if empty, @1 if only one element, @2 if more than 1 elem
int vertex_list_state2(const struct vertex_list2* vlist)
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

bool invariant2(const struct vertex_list2* vlist)
{
	return vertex_list_state2(vlist) != ERROR;//maybe =
}

void vertex_list_ini2(struct vertex_list2* vlist)
{
	vlist->head = NULL;
	vlist->tail = NULL;
}

 //maybe vertex is const, but warning
int vertex_list_add2(struct vertex_list2* vlist, struct vertex2* vertex)
{
	int rc = vertex_list_state2(vlist);

	struct vertex_list_node2* node;
	ALLOC_PTR(node);
	if (node == 0)
		return -ENOMEM;
	
	node->next = NULL;
	node->data = vertex;

	if (rc == MORE) {
		node->prev = vlist->tail;

		vlist->tail->next = node;
		vlist->tail = node;

		assert(invariant2(vlist));
		return 0;
	}
	if (rc == ONE) {
		node->prev = vlist->tail;

		vlist->tail = node;
		vlist->head->next = node;

		assert(invariant2(vlist));
		return 0;
	}
	if (rc == EMPTY) {
		node->prev = NULL;

		vlist->head = node;
		vlist->tail = vlist->head;

		assert(invariant2(vlist));
		return 0;
	}
	node->data = NULL;
	node->prev = NULL;
	vertex_list_node_del2(node);

	perror("no one case is not apply");
	return -EINVAL; // errno
} // -9 lines

//maybe I have reason to rewrite this fuu, to find from tail,
//because it is beter for fini
struct vertex_list_node* vertex_list_find2(const struct vertex_list2* vlist, 
					  const struct vertex2* find)
{
	struct vertex_list_node2* scan = NULL;

	for (scan = vlist->head; scan != NULL; scan = scan->next)
		if (scan->data == find)
			return scan;

	return NULL;
}

//one of "after" or "node" must be equal NULL
//in this function situation like in rm
//maybe given, node are const, but warning
int vertex_list_ins2(struct vertex_list2* vlist, struct vertex2* given,
		    const struct vertex2* after, 
		    struct vertex_list_node2* node)
{
	if ((after == NULL && node == NULL) || (after != NULL && node != NULL))
		return ERROR;

	//case in what we have only after, and we must find it 
	struct vertex_list_node2* temp = NULL;
	if (node == NULL) {
		temp = vertex_list_find2(vlist, after);
		assert(temp > 0);

		if (temp == vlist->tail) {
			//fuu of add to tail may to write singly
			int rc = vertex_list_add2(vlist, given);
			
			return rc != 0 ? rc : 0;
		}
	}
	//this pice of code is equal to node == 0 and after == 0
	if (temp == vlist->tail || node == vlist->tail) { //maybe confuse
		int rc = vertex_list_add2(vlist, given);
		return rc == 0 ? 0 : rc;
	}
	struct vertex_list_node2* elem_ins;
	ALLOC_PTR(elem_ins);
	if (elem_ins == 0)
		return -ENOMEM;	
	elem_ins->data = given;

	if (node == NULL) { // this pice and after == 0 I can i some way connect
		//printf("after: %i\n", temp->data->weight);
		elem_ins->prev = temp;
		elem_ins->next = temp->next;
		
		temp->next->prev = elem_ins;
		temp->next = elem_ins;
		
		return 0;
	}
	if (after == NULL) {
		//printf("after: %i\n", node->data->weight);
		elem_ins->prev = node;
		elem_ins->next = node->next;
		
		node->next->prev = elem_ins;
		node->next = elem_ins;
		
		return 0;
	}
	//if error with insertion, temp or elem_ins must be remove
	temp = NULL;

	elem_ins->prev = NULL;
	elem_ins->next = NULL;
	vertex_list_node_del2(elem_ins);

	perror("somthing wrong in insert");
	return ERROR;
}

void vertex_list_print2(const struct vertex_list2* vlist)
{
	printf("[ ");

	struct vertex_list_node2* print = NULL;
	for(print = vlist->head; print != NULL; print = print->next)
		printf("%i ", print->data->column);

	printf("]\n");
}

//if node == NULL work with find, in another case rm node element
//must be 30-40 lines
void vertex_list_rm2(struct vertex_list2* vlist, struct vertex2* vertex, 
		    struct vertex_list_node2* node)
{
	//delete this when I know how to chose
  	assert(vertex == NULL || node == NULL);

	int rc = vertex_list_state2(vlist); 
	if (rc == EMPTY || rc == ERROR)
		return;

	struct vertex_list_node2* temp = NULL;
	if (node == NULL) {
		temp = vertex_list_find2(vlist, vertex);
		assert(temp > 0);
	}
	//here code for node and vertex equal
	//HERE MUST BE CHOSE WORK WITH node OR temp!!!!!!!!!!!!
	//I must know where is node - center, tail or head
	//now do for node, temp don`t use
	if (node == NULL) {
		if (temp == vlist->head) { // or temp
			if (vlist->head->next != NULL) {
				vlist->head = vlist->head->next;
				vlist->head->prev = NULL;
			}
			else
				vlist->tail = NULL;
			
			temp->next = NULL;
			vertex_list_node_del2(temp);

//			if (vlist->head == NULL)
			if (vlist->tail == NULL)
				vlist->head = NULL;
		}
		//here some error
		else if (temp == vlist->tail) { // or temp
			if (vlist->tail->prev != NULL) {
				vlist->tail = vlist->tail->prev;
				vlist->tail->next = NULL;
			}
			
			temp->prev = NULL;
			vertex_list_node_del2(temp);
			if (vertex_list_state2(vlist) == ONE) {
				vlist->head->next == NULL;
				vlist->tail == NULL; //this not work??? :(
			}
		}
		else { // more than one element
			//delete from list
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			
			//delete node
			temp->prev = NULL;
			temp->next = NULL;
			vertex_list_node_del2(temp);
		}
	}
	if (vertex == NULL) {
		if (node == vlist->head) { // or temp
			if (vlist->head->next != NULL) {
				vlist->head = vlist->head->next;
				vlist->head->prev = NULL;
			}
			else
				vlist->tail = NULL;
			
			node->next = NULL;
			vertex_list_node_del2(node);
			if (vlist->tail == NULL) {
				//some bug here, when "ONE", rm->head
				//after that apeared vlist->head->next->data
				//and this data = 0, in this case I write
				//vlist->head = NULL, to remove this problem
				vlist->head = NULL;
			}
		}
		else if (node == vlist->tail) { // or temp
			if (vlist->tail->prev != NULL) {
				vlist->tail = vlist->tail->prev;
				vlist->tail->next = NULL;
			}
			
			node->prev = NULL;
			vertex_list_node_del2(node);
			if (vertex_list_state2(vlist) == ONE) {
				vlist->head->next == NULL;
				vlist->tail == NULL; //this not work??? :(
			}
		}
		else { // more than one element
			//delete from list
			node->prev->next = node->next;
			node->next->prev = node->prev;
			
			//delete node
			node->prev = NULL;
			node->next = NULL;
			vertex_list_node_del2(node);
		}
	}
	assert(invariant2(vlist));
//	perror("somthing wrong with list_(rm)");
//	return;
}

void vertex_list_fini2(struct vertex_list2* vlist)
{
	while(vlist->head)
		vertex_list_rm2(vlist, NULL, vlist->tail);

}
/*
int main(int argc, char** argv)
{

	struct vertex_list* lst;
//	lst = malloc(1 * sizeof(lst[0]));
	ALLOC_PTR(lst);
	assert(lst > 0);

	vertex_list_ini(lst);

	//invariant test
	int rc = invariant(lst);
	printf("%i\n", rc);
	//end test of invariant

	struct vertex* t;
	ALLOC_PTR(t);
	t->weight = 3;

	rc = vertex_list_add(lst, t);
	if(rc != 0)
		return rc;

	printf("weight: %i\n", lst->head->data->weight);

	//start________________test of insert and find
	struct vertex* t2;
	ALLOC_PTR(t2);
	t2->weight = 4;

	struct vertex* t3;
	ALLOC_PTR(t3);
	t3->weight = 5;

	struct vertex* ins;
	ALLOC_PTR(ins);
	ins->weight = 6;

	vertex_list_print(lst);
	//ins without add
	rc =  vertex_list_ins(lst, ins, t2);
	if(rc == ERROR) {
		printf("can not find element or error\n");
	}

	rc = vertex_list_add(lst, t2);
	if(rc != 0)
		return rc;

	rc = vertex_list_add(lst, t3);
	if(rc != 0)
		return rc;

	//ins in center
	rc =  vertex_list_ins(lst, ins, t2);
	if(rc == ERROR) {
		printf("can not find element or error\n");
	}
	vertex_list_print(lst);

	//ins in tail
	rc =  vertex_list_ins(lst, ins, t3);
	if(rc == ERROR) {
		printf("can not find element or error\n");
	}
	vertex_list_print(lst);

	//ins after head
	rc =  vertex_list_ins(lst, ins, t);
	if(rc == ERROR) {
		printf("can not find element or error\n");
	}
	vertex_list_print(lst);
	//end__________________test of insert and find

	//start test rm____________
	//rm from head
	vertex_list_rm(lst, t);
	//rm from tail
	//not rm because remove from center

	//rm from center
	vertex_list_rm(lst, t3);
	vertex_list_print(lst);

	vertex_list_fini(lst);

	if(lst->head == lst->tail && lst->head == 0) {
		free(lst);
		lst = 0;
	}
	else
		perror("error fini");
	//end test rm____________
	return 0;
}
*/
