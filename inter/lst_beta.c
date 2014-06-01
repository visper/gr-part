#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define TEST_TIMES 100000//100 000
#define RANDOM_NUM 99999

struct vertex {
 	int weight;
};

struct vertex_list_node {
 	struct vertex_list_node *next;
 	struct vertex_list_node *prev; 	struct vertex *data;
};

struct vertex_list {
	struct vertex_list_node *head;
	struct vertex_list_node *tail;
};

//initialization of struct
struct vertex_list* vertex_list_init(struct vertex_list *vlist)
{
	//выделение памяти под весь список
 	vlist = (struct vertex_list*)
 		malloc(sizeof(struct vertex_list));
 	if(vlist == 0)
		return 0;

 	vlist->head = (struct vertex_list_node*)
 		malloc(sizeof(struct vertex_list_node));
 	if(vlist == 0)
 		return 0;
	
 	vlist->tail = vlist->head;

 	vlist->head->data = 0;
 	vlist->head->prev = 0;
 	vlist->head->next = 0;

 	return vlist;
}

//add to tail
void vertex_list_add(struct vertex_list* vlist, struct vertex* node) 
{
	//check the struct is empty, no alloc
	if(vlist->head == vlist->tail 
	   && vlist->head->data->weight == 0)
		vlist->head->data = node;
	
	assert(vlist->head->data->weight != 0);
	
	struct vertex_list_node* temp = (struct vertex_list_node*)
		malloc(sizeof(struct vertex_list_node));
	temp->data = node;
	temp->next = 0;
	temp->prev = vlist->tail;
	
	vlist->tail->next = temp;
       
	vlist->tail = temp;
}

//function for print elements
void vertex_list_print(struct vertex_list* vlist)
{
	size_t counter_nodes = 0;
	struct vertex_list_node* temp = vlist->head;

//list is empty
	if(temp->data == 0) {
		printf("list is empty\n");
	} 
//list not empty
	else {
		while(temp) {
			printf("%i  ", temp->data->weight);
			temp = temp->next;
			counter_nodes++;
		}
	}

	temp = 0;
}

//function for find element
//return 0 = no element
struct vertex_list_node* vertex_list_find(struct vertex_list *vlist,
					  struct vertex* node)
{
//возможно тут отдельно под data нужно память выделить
	struct vertex_list_node* temp = vlist->head;
	
	while(temp) {
		if(temp->data->weight == node->weight)
			return temp;
		temp = temp->next;
	}

	temp = 0;
	return 0;
}

//remove element
//return !=0 if node not found
int vertex_list_remove(struct vertex_list* vlist, struct vertex* node)
{
	struct vertex_list_node* temp = 0;
	
	temp = vertex_list_find(vlist, node);
	if (temp == 0) {
		printf("no element\n");
		return -1;
	}

	free(temp->data);
	temp->data = 0;
	
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	temp->next = 0;
	temp->prev = 0;

	//if node == 0, remove from tail
	if(node == 0) {
                //if only one element
		if(vlist->head == vlist->tail) {
			if(vlist->head->data->weight == 0) {
                                //list is empty
				return 0;
			}
			else {
				if(vlist->head->data != 0) {
					free (vlist->head->data);
					vlist->head->data = 0;
				}
			}
		}
		free(vlist->tail->data);
		vlist->tail->data = 0;
		vlist->tail->next = 0;

		vlist->tail = vlist->tail->prev;
		vlist->tail->next->prev = 0;
		vlist->tail->next = 0;
	}
	temp = 0;
	return 0;
}

//if !0 fail
int vertex_list_insert(struct vertex_list* vlist, struct vertex* after,
		       struct vertex* node)
{
	struct vertex_list_node* temp = vertex_list_find(vlist,after);
	if(temp == 0) {
		printf("no element\n");
		return -1;
	}

	struct vertex_list_node* vertex = (struct vertex_list_node*)
		malloc(sizeof(struct vertex_list_node));
	vertex->data = node;

	vertex->next = temp->next;
	vertex->prev = temp;
	temp->next = vertex;
	vertex->next->prev = vertex;

	temp = 0;
	return 0;
}

//free the malloc of list
void vertex_list_fini(struct vertex_list* vlist) 
{
	//remore nodes by vertex_list_remove with second
        //agrument = 0;
	int check = 0;
	while(vlist->head != vlist->tail) {
		check = vertex_list_remove(vlist,0);
		//with 0 value return will be always 0
		if(check != 0)
			printf("error of remove operation\n");
	}

	if(vlist->head->data != 0) {
		free(vlist->head->data);
		vlist->head->data = 0;
	}
	
	if(vlist->head != 0) {
		free(vlist->head);
		vlist->head = 0;
	}
	if(vlist != 0) {
		free(vlist);
		vlist = 0;
	}
}
  
int main(int argc, char** argv){ 


//_____testing all functions without connections with other
  //test for init
	struct vertex_list *ptr = 0;
	
 	ptr = vertex_list_init(ptr);
	if(ptr == 0) {		
		printf("error allocation memmory\n");
		return -1;
	}	
 	assert (ptr != 0);

 	if(errno != 0){
 		printf("memory os full\n");
 		return -1;
 	}
	assert(ptr->head->data == 0);

//test add


//test find

//test rm

//test insert

//test print

//test fini
	fini(ptr);
	assert(ptr == 0);
/*
//___________________TEST_______________________

//add element to tail
	for(size_t i = 0; i < TEST_TIMES; ++i) {
		struct vertex* node = (struct vertex*)
			malloc(sizeof(struct vertex));
		node->weight = 1 + rand() % RANDOM_NUM;
	
		vertex_list_add(ptr, node);
	}
        //print
	vertex_list_print(ptr);

//remove element
	size_t counter = 0;
	for(size_t i = 0; i < TEST_TIMES/2; ++i) {
		struct vertex* v = (struct vertex*)
			malloc(sizeof(struct vertex));
		v->weight = 1 + rand() % RANDOM_NUM;
		
		size_t check = vertex_list_remove(ptr,v);
		if(check != 0) ++counter;
	}
	printf("num of remover elements is %zu\n", counter);
        //-1 no element to remove
	//print
	vertex_list_print(ptr);
	
//add	
	for(size_t i = 0; i <= counter; ++i) {
		struct vertex* node = (struct vertex*)
			malloc(sizeof(struct vertex));
		node->weight = 1 + rand() % RANDOM_NUM;
	
		vertex_list_add(ptr, node);
	}
        //print
	vertex_list_print(ptr);

//remove all elements, testing delete of last element
	for(size_t i = 0; i < TEST_TIMES; ++i) {
		size_t check = vertex_list_remove(ptr,0);
	}

	//testing deleting of empty list
	vertex_list_fini(ptr);

        //testing to print of no vlist
	vertex_list_print(ptr);
	

//secont init
 	struct vertex_list *ptr1 = 0;

 	ptr = vertex_list_init(ptr1);
	if(ptr1 == 0) {		
		printf("error allocation memmory\n");
		return -1;
	}	
 	assert (ptr1 != 0);

 	if(errno != 0){
 		printf("memory os full\n");
 		return -1;
 	}

//second add
	for(size_t i = 0; i < TEST_TIMES - 20; ++i) {
		struct vertex* node = (struct vertex*)
			malloc(sizeof(struct vertex));
		node->weight = 1 + rand() % RANDOM_NUM;
	
		vertex_list_add(ptr1, node);
	}
        //print
	vertex_list_print(ptr1);

//insertion and remove elements

        //insertion of 30
	for(size_t i = 0; i < 30; ++i) {
		struct vertex* node = (struct vertex*)
			malloc(sizeof(struct vertex));
		node->weight = 1 + rand() % RANDOM_NUM;

		int check = vertex_list_insert(ptr1, node, node);
		if(check != 0) {
			--i;
			if(node != 0) {
				free(node);
				node = 0;
			}
		}
	}
        //print
	vertex_list_print(ptr1);

	//delete of 10 elements
	for(size_t i = 0; i < 10; ++i) {
		struct vertex* node = (struct vertex*)
			malloc(sizeof(struct vertex));
		node->weight = 1 + rand() % RANDOM_NUM;

		int check = vertex_list_remove(ptr1, node);
		if(check != 0) --i;
		if(node != 0) {
			free(node);
			node = 0;		
		}
	}
        //print
	vertex_list_print(ptr1);

	vertex_list_fini(ptr1);
	

*/

 	return 0;
 }
