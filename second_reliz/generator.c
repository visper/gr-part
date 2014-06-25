/*
- задаемся количеством элементов
- генерируем элементы по заданным правилам
- обращаемся к элементам разными комбинациями
тут же тестируем с помошью написанного таймера время работы
разных функций
*/
#include <stdlib.h>
#include <stdio.h>
//#include <timer.h>
#include <assert.h>

#include "timer.h"
//#include "3_mass_te.h"

#include "common.h"
#include "right_list.h"
#include "3_mass_list.h"
#include "gen_matrix.h"
#include "3_mass_1list.h"
#include "list_cormen.h"

#include "list_support.h"
//add new struct here

#include "recursive_partition.h"

#include "tr_converter.h"

//#define NUM_OF_NODES 50 // 1000 element no more, SIGSEG

 //foo for work with one of realisation of 3 mass
 //realization is situated in .h file
int three_mass_1list(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	//start of test
	printf("start test 3_mass_1list_______________________\n");
	//alloc mem
	struct three_mass3* m3_1;
	ALLOC_PTR(m3_1);
	if (m3_1 == 0)
		return -ENOMEM;

	//variable for time check
	size_t start,end;
	start = checkpoint();
	three_mass_ini3(m3_1);
	end = checkpoint();
	printf("init 3_mass_1list ");
	report(end - start);
	printf("\n");

	start = checkpoint();
	//convert from matrix to this struct
	for(int i = 0; i < NUM_OF_NODES; ++i) {
		for(int j = 0; j < NUM_OF_NODES; ++j) {
			if (matrix[i][j] == 1) {
				struct vertex3* node;
				ALLOC_PTR(node);
				if (node == 0)
					return -ENOMEM;

				node->weight = 1;
				node->column = j;
				node->row = i;

				int rc = 0;
				rc  = three_mass_add3(m3_1, node);
				assert(rc == 0);
			}
		}
	}
	end = checkpoint();
	printf("add all matrix 3_mass_1list ");
	report(end - start);
	printf("\n");

	//insert some tests

	start = checkpoint();
	three_mass_fini3(m3_1);
	end = checkpoint();
	printf("fifi of 3_mass_1list ");
	report(end - start);
	printf("\n");

	//free the malloc
	if (m3_1->mass != NULL) {
		free (m3_1->mass);
		m3_1->mass = NULL;
	}
	if (m3_1 != NULL) {
		free (m3_1);
		m3_1 = NULL;
	}

	//end of test
	printf("end test 3_mass_1list_______________________\n");
	return 0;
}

 //foo for work with one of realisation of 3 mass
 //realization is situated in .h file
int three_mass_3list(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	printf("start test of 3_mass_3list++++++++++++++++\n");

	struct three_mass* m3_3;
	ALLOC_PTR(m3_3);
	if (m3_3 == 0)
		return -ENOMEM;

	size_t start, end;
	start = checkpoint();
	three_mass_ini(m3_3);
	end = checkpoint();
	printf("init of 3_mass_3list ");
	report(end - start);
	printf("\n");

	start = checkpoint();
	//convert from matrix to struct 
	for(int i = 0; i < NUM_OF_NODES; ++i) {
		for(int j = 0; j < NUM_OF_NODES; ++j) {
			if (matrix[i][j] == 1) {
				struct vertex* rr; // row
				ALLOC_PTR(rr);
				if (rr == 0)
					return -ENOMEM;

				rr->weight = i;

				struct vertex* col; // column
				ALLOC_PTR(col);
				if (col == 0)
					return -ENOMEM;

				col->weight = j;

				struct vertex* val; // value
				ALLOC_PTR(val);
				if (val == 0)
					return -ENOMEM;

				val->weight = 1;

				int rc = three_mass_add(m3_3, rr, col, val);
				assert(rc == 0);
			}
		}
	}
	end = checkpoint();
	printf("add of 3_mass_3list ");
	report(end - start);
	printf("\n");

	//insert some tests

	start = checkpoint();
	three_mass_fini(m3_3);
	end = checkpoint();
	printf("fifi of 3_mass_1list ");
	report(end - start);
	printf("\n");

	if (m3_3->row != NULL) {
		free (m3_3->row);
		m3_3->row = NULL;
	}
	if (m3_3->column != NULL) {
		free (m3_3->column);
		m3_3->column = NULL;
	}
	if (m3_3->value != NULL) {
		free (m3_3->value);
		m3_3->value = NULL;
	}

	if (m3_3 != NULL) {
		free (m3_3);
		m3_3 = NULL;
	}

	printf("end test of 3_mass_3list++++++++++++++++\n");
	return 0;
}

/*
//недопилиная структура, надо проверять
int three_mass_te(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	printf("start test 3_mass_te_______________________\n");
	struct mass1* m3;
	ALLOC_PTR(m3);
	if (m3 == 0)
		return -ENOMEM;

	size_t start,end;
	start = checkpoint();
	mass_ini1(m3);
	end = checkpoint();
	printf("init 3_mass_te ");
	report(end - start);
	printf("\n");

	start = checkpoint();
	for(size_t i = 0; i < NUM_OF_NODES; ++i) {
		for(size_t j = 0; j < NUM_OF_NODES; ++j) {
			if (matrix[i][j] == 1) {
				struct data1* data;
				ALLOC_PTR(data);
				if (data == 0)
					return -ENOMEM;

				data->value = 1;
				data->column = j;
				data->row = i;

				int rc = 0;
				rc  = mass_add1(m3, data);
				assert(rc == 0);
			}
		}
	}
	end = checkpoint();
	printf("add all matrix 3_mass_te ");
	report(end - start);
	printf("\n");

	//insert some tests

	start = checkpoint();
	mass_fini1(m3);
	end = checkpoint();
	printf("fifi of 3_mass_te ");
	report(end - start);
	printf("\n");

	if (m3->str != NULL) {
		free (m3->str);
		m3->mass = NULL;
	}
	if (m3 != NULL) {
		free (m3);
		m3 = NULL;
	}

	printf("end test 3_mass_te_______________________\n");
	return 0;
}
*/
 /*
void prin_cormen(int matrix[NUM_OF_NODES][NUM_OF_NODES], 
		 struct vertex_list* vlist)
{
	for (int i = 0; i < NUM_OF_NODES; ++i) {
		printf ("line %i - ", i);
		struct vertex_list* lst;
		lst = (vlist + (i * sizeof(struct vertex_list*)));
		struct vertex_list_node* vertex;
		vertex = lst->head;
		lst = NULL;
		for (; vertex != NULL; vertex->next) {
			assert(vertex->data != NULL);
			printf (" %i ", vertex->data->weight);
		}
		printf ("\n");
	}
}
 */

  //fuu for work with list of lists for save graph
int list_cormen(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	printf ("start list cormen***************\n");

	struct vertex_list* list;
	ALLOC_ARR(list, NUM_OF_NODES);
	if(list == 0)
		return -ENOMEM;

	size_t start, end;
	start = checkpoint();
	int rc = create_list(list);
	if (rc != 0) {
		//assert(rc == 0);
		perror ("error of list init");
		return ERROR;
	}
	end = checkpoint();
	printf("init of list cormen ");
	report(end - start);
	printf("\n");

	start = checkpoint();
	for(int i = 0; i < NUM_OF_NODES; ++i) {
		for(int j = 0; j < NUM_OF_NODES; ++j) {
			if (matrix[i][j] == 1) {
				struct vertex* v;
				ALLOC_PTR(v);
				if (v == 0)
					return -ENOMEM;

				// j+1
				v->weight = j; //column

				int rc1 = add(list, v, i);
				assert (rc1 == 0);
			}
		}
	}
	end = checkpoint();
	printf("add in list cormen ");
	report(end - start);
	printf("\n");

	//insert some tests

	//for () //полносьтю распечатать список с next и prev

	//PARTITION
	struct vertex_list* list1;
	ALLOC_ARR(list1, NUM_OF_NODES);
	if (list1 == 0)
		return -ENOMEM;

	rc = create_list(list1);
	assert(rc == 0);

	struct vertex_list* list2;
	ALLOC_ARR(list2, NUM_OF_NODES);
	if (list2 == 0)
		return -ENOMEM;

	rc = create_list(list2);
	assert(rc == 0);

      	rc = recursive_part(list, list1, list2);
	assert (rc == 0);

	printf ("list___\n");
	print_list(list);
	
       	printf ("list___1___\n");
	print_list(list1); //del
	printf ("list___2___\n");
	print_list(list2); //del	

	int temp = vertex_num(list);
	printf ("number of vertex is %i\n", temp);

        temp = vertex_num(list1);
	printf ("number of vertex is %i\n", temp);

	temp = vertex_num(list2);
	printf ("number of vertex is %i\n", temp);

	print_head();
	tr_list_to_dot(list);//error

	//create matrix for translate list to matrix :)
//	int mm[NUM_OF_NODES][NUM_OF_NODES] = {0};
//	tr_list_to_matrix(list, mm);
//      tr_matrix_to_dot(mm);

	print_end();

/*
	for (int i = 0; i < NUM_OF_NODES; ++i) {
		printf ("line %i - ", i);
		struct vertex_list* lst;
		lst = (list + (i * sizeof(struct vertex_list*)));
		struct vertex_list_node* vertex;
		vertex = lst->head;
		list = NULL;
		for (; vertex != NULL; vertex->next) {
			assert(vertex->data != NULL);
			printf (" %i ", vertex->data->weight);
		}
		printf ("\n");
	}
*/
/*
	destroy_list(list1);
	destroy_list(list2);

	if (list1 != NULL) { // maybe this wrong
		free(list1);
		list1 = NULL;
	}
	if (list2 != NULL) { // maybe this wrong
		free(list2);
		list2 = NULL;
	}
*/
	start = checkpoint();
	//      	destroy_list(list);
	end = checkpoint();
	printf("fifi of list cormen");
	report(end - start);
	printf("\n");

	if (list != NULL) { // maybe this wrong
		free(list);
		list = NULL;
	}


	printf ("end list cormen***************\n");
	return 0;
}


int main(int argc, char** argv)
{
	//сначала представляем граф в виде двумерной матрицы
	int matrix[NUM_OF_NODES][NUM_OF_NODES] = {0};

//	printf ("gen = %i\n", gen_values(5, 10)); //8
//	printf("gen 2 = %i\n", 5 + rand() %10); //11

	matrix_gen(matrix);
	matrix_gen(matrix);
	matrix_gen(matrix);
	matrix_gen(matrix);
	// оставляет единицы в верхнем треугольнике
	// петель нет, но могут быть пустые строки
	up_triangle(matrix); 
	del_zero_lines(matrix);

	matrix_print(matrix);

	/*
	print_head();
      	tr_matrix_to_dot(&matrix);
	print_end();
	sleep(1);
	*/

	int rc = 0;
	rc = three_mass_1list(matrix);
	assert(rc == 0);

	rc = three_mass_3list(matrix);
	assert(rc == 0);

	rc = list_cormen(matrix);
	assert(rc == 0);

	//пишим функции для трансляции этой матрицы в разные
	//структуры данных с замерами времени


	return 0;
}
