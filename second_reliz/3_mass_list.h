//this .h file created for work with graph, struct - 3 mass
//it use functions from right list. this file used in another files,
//in this case "include" is comented

//#include "right_list.h"

struct three_mass {
	struct vertex_list* row;
	struct vertex_list* column;
	struct vertex_list* value;
};

//ini
void three_mass_ini(struct three_mass* vmass)
{
	ALLOC_PTR(vmass->row);
	if (vmass->row == 0)
		return;

	ALLOC_PTR(vmass->column);
	if (vmass->column == 0)
		return;

	ALLOC_PTR(vmass->value);
	if (vmass->value == 0)
		return;

	vertex_list_ini(vmass->row);
	vertex_list_ini(vmass->column);
	vertex_list_ini(vmass->value);
}

//fini
void three_mass_fini(struct three_mass* vmass)
{
	vertex_list_fini(vmass->row);
	vertex_list_fini(vmass->column);
	vertex_list_fini(vmass->value);
}

//invariant check that all lists be equal in lenght
bool three_mass_invariant(struct three_mass* vmass)
{
        struct vertex_list_node* scan = NULL;

	size_t row = 0;
        for (scan = vmass->row->head; scan != NULL; scan = scan->next)
		row++;

	size_t column = 0;
        for (scan = vmass->column->head; scan != NULL; scan = scan->next)
		column++;

	size_t value = 0;
        for (scan = vmass->value->head; scan != NULL; scan = scan->next)
		value++;

        return (row == column && column == value) ? 0 : ERROR;
}

//add
int three_mass_add(struct three_mass* vmass, struct vertex* row,
		   struct vertex* column, struct vertex* value)
{
	int rc = vertex_list_add(vmass->row, row);
	if (rc == -ENOMEM || rc == ENOMEM)
		return ENOMEM;
	if (rc == -EINVAL || rc == EINVAL)
		return EINVAL;
	assert(rc == 0);

	rc = vertex_list_add(vmass->column, column);
	if (rc == -ENOMEM || rc == ENOMEM)
		return ENOMEM;
	if (rc == -EINVAL || rc == EINVAL)
		return EINVAL;
	assert(rc == 0);

	rc = vertex_list_add(vmass->value, value);
	if (rc == -ENOMEM || rc == ENOMEM)
		return ENOMEM;
	if (rc == -EINVAL || rc == EINVAL)
		return EINVAL;
	assert(rc == 0);
	//	printf("invariant in add: %i\n", three_mass_invariant(vmass));
	assert(three_mass_invariant(vmass) == 0);

	return 0;
}

//ins
int three_mass_ins(struct three_mass* vmass, struct vertex* given_row,
		   struct vertex* given_column, struct vertex* given_value,
		   const struct vertex* after_row,
		   const struct vertex* after_column,
		   const struct vertex* after_value)
{
	int rc = vertex_list_ins(vmass->row, given_row, after_row);
	//	printf("first %i\n", rc);
	assert(rc == 0);

	rc = vertex_list_ins(vmass->column, given_column, after_column);
	//	printf("second %i\n", rc);
	assert(rc == 0);

	rc = vertex_list_ins(vmass->value, given_value, after_value);
	printf("third %i\n", rc);
	assert(rc == 0);

	assert(three_mass_invariant(vmass) == 0);
	return rc;
}

//rm
void three_mass_rm(struct three_mass* vmass, struct vertex* vertex_row, 
		   struct vertex* vertex_column, struct vertex* vertex_value)
{
	vertex_list_rm(vmass->row, vertex_row);
	vertex_list_rm(vmass->column, vertex_column);
	vertex_list_rm(vmass->value, vertex_value);

	assert(three_mass_invariant(vmass) == 0);
}

//print
void three_mass_print(struct three_mass* vmass)
{
	printf("row\n");
	vertex_list_print(vmass->row);

	printf("column\n");
	vertex_list_print(vmass->column);

	printf("value\n");
	vertex_list_print(vmass->value);
}
/*
int main(void)
{
	struct three_mass* mass = NULL;
	ALLOC_PTR(mass);
	if(mass == 0)
		return -ENOMEM;

	three_mass_ini(mass);

	struct vertex* v1 = NULL;
	ALLOC_PTR(v1);
	if (v1 == 0)
		return -ENOMEM;
	v1->weight = rand();

	struct vertex* v2 = NULL;
	ALLOC_PTR(v2);
	if (v2 == 0)
		return -ENOMEM;
	v2->weight = rand();

	struct vertex* v3 = NULL;
	ALLOC_PTR(v3);
	if (v3 == 0)
		return -ENOMEM;
	v3->weight = rand();

	struct vertex* v4 = NULL;
	ALLOC_PTR(v4);
	if (v4 == 0)
		return -ENOMEM;
	v4->weight = rand();

	struct vertex* v5 = NULL;
	ALLOC_PTR(v5);
	if (v5 == 0)
		return -ENOMEM;
	v5->weight = rand();

	struct vertex* v6 = NULL;
	ALLOC_PTR(v6);
	if (v6 == 0)
		return -ENOMEM;
	v6->weight = rand();

	int rc = three_mass_add(mass, v1, v2, v3);
	assert(rc == 0);

	three_mass_print(mass);

	rc = three_mass_ins(mass, v4, v5, v6, v1, v2, v3, NULL, NULL, NULL);
	printf("rc in main %i\n", rc);
	assert(rc == 0);
	three_mass_print(mass);

	three_mass_rm(mass, v1, v5, v6, NULL, NULL, NULL);

	three_mass_print(mass);

	three_mass_fini(mass);

	if(mass != NULL) {
		free(mass);
		mass = NULL;
	}

	return 0;
}
*/
