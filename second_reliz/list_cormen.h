//this file based on right_list.h

//#include "right_list.h"

#ifndef NUM_OF_NODES
#define NUM_OF_NODES 50
#endif

int create_list(struct vertex_list* vlist)
{
	size_t j = 0;
	//malloc mem for NUM_OF_NODES * sizeof(list)
	struct vertex_list* i = NULL;
	for(i = vlist; j < NUM_OF_NODES; i = i + sizeof(struct vertex_list*)) {
		vertex_list_ini(i);
		j++;
	}
	return 0;
}

void destroy_list(struct vertex_list* vlist)
{
	size_t j = 0;
	for(struct vertex_list* i = vlist; j < NUM_OF_NODES; 
	    i = i + sizeof(struct vertex_list*)) {
		vertex_list_fini(i);
		j++;
	}
}

int add(struct vertex_list* vlist, struct vertex* vertex, int num_of_node)
{
	int rc=vertex_list_add((vlist+num_of_node*sizeof(struct vertex_list*)), 
				 vertex);
	return (rc != 0) ? rc : 0;
}

void rm(struct vertex_list* vlist, struct vertex* vertex, size_t num_of_node)
{
	vertex_list_rm((vlist+num_of_node*sizeof(struct vertex_list*)), vertex);
}

int insert(struct vertex_list* vlist,struct vertex* given,
                    const struct vertex* after, size_t num_of_node)
{
	int rc = 0;
	rc = vertex_list_ins((vlist+num_of_node*sizeof(struct vertex_list*)), 
			     given, after);
	return rc != 0 ? rc : 0;
}

void print_list(struct vertex_list* vlist)
{
	for(size_t i = 0; i < NUM_OF_NODES; ++i)
		vertex_list_print(vlist+i*sizeof(struct vertex_list*));
}

/*
int main(void)
{
	struct vertex_list* list;
	ALLOC_ARR(list, NUM_OF_NODES);
	if(list == 0)
		return -ENOMEM;

	int rc = create_list(list);
	assert(rc == 0);
	printf("test_1\n");

        struct vertex* v1 = NULL;
        ALLOC_PTR(v1);
        if (v1 == 0)
                return -ENOMEM;
        v1->weight = rand();

	rc = add(list, v1, 5);
	assert(rc == 0);

	print_list(list);

        struct vertex* v2 = NULL;
        ALLOC_PTR(v2);
        if (v2 == 0)
                return -ENOMEM;
        v2->weight = rand();

	rc = insert(list, v2, v1, NULL, 5);
	assert(rc == 0);
	print_list(list);

	rm(list, v1, NULL, 5);
	print_list(list);

	destroy_list(list);
	printf("test_2\n");

	//free the malloc()!

//	size_t j = NUM_OF_NODES;
//	for(struct vertex_list* i = 
//		    list + NUM_OF_NODES * sizeof(struct vertex_list*); 
//	    j > 0 ; i = i - sizeof(struct vertex_list*)) {
//		free(i);
//		//here I can`t write i = 0;
//		j++;
//	}

//	size_t j = 0;
//	for(struct vertex_list* i = list; j < NUM_OF_NODES; 
//	    i = i + sizeof(struct vertex_list*)) {
//		printf("test_3\n");
//		if (i != 0)
//			free(i);
//		//here I can`t write i = 0;
//		j++;
//	}

	printf("test_4\n");

	if (list != 0) { // maybe this wrong
		free(list);
		list = 0;
	}

	return 0;
}
*/
