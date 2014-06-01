#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tr_generator_name.h"
//char* get_time();

#include "tr_dot_translator.h"

#ifndef NUM_OF_NODES
#define NUM_OF_NODES 50
#endif

FILE *f;

void print_head(void)
{
	f = fopen(get_time(), "w+");
	if (f == NULL) {
		perror("error open/create file");
		return;
	}
//	fprintf(f, "%s", "hello");//почему-то не пишет
	fprintf(f, "%s", _dir());
	fprintf(f, "%s", sep[9]);
	fprintf(f, "%s", sep[0]);
	fprintf(f, "\n");
//	printf("test\n");

/*
char* _dir()
sep[9] // " "
sep[0] // "{"
...
sep[4] // ";"
...
char* _dir_con()
...
sep[1] // "}"
*/
}

void print_end(void)
{
	fprintf(f, "\n");
	fprintf(f, "%s", sep[1]);

	fclose(f);
}

//int len_num_of_nodes(void)
int len_num(int num)
{
	int kol = 0;
//	int num = NUM_OF_NODES;
	if (num/10 == 0)
		return 1;
	else
	{
		while (num > 0) 
		{
			num = num/10;
			kol += 1;
		}
		return kol;
	}
}

//нужно создать имена соответствующие каждой вершине
char* gen_vertexes_names(void)
{
	//имя вершины будет состоять из vertex и цифр в зависимости от
	//количества вершин

	//длина имени одной вершины, 6 - vertex
	int vertex_name_len = 6 + len_num(NUM_OF_NODES);

	//здесь будут храниться все имена
	char* graph = NULL;
	graph = malloc(NUM_OF_NODES * vertex_name_len * sizeof(char));
	if (graph == 0) {
		perror("mem is full 1");
		return;
	}
	graph[0] = '\0';

	for (int i = 0; i < NUM_OF_NODES; ++i) {
		//v_name имя одной вершины
		char* v_name;
		v_name = malloc(vertex_name_len * sizeof(char));
		if (v_name == 0) {
			perror("mem is full 2");
			return;
		}
		v_name[0] = '\0';

		v_name = strcat(v_name, "vertex");

		char* num = NULL;
		num = malloc(len_num(NUM_OF_NODES) * sizeof(char));
		if (num == 0) {
			perror("mem is full 3");
			return;
		}
		num[0] = '\0';

		itoa(i, num);
		//нужно дописать перед цифрой ноль если длина числа не
		//соответствует длине NUM_OF_NODES чтобы 
		//все названия вершин были одинаковые
		if (len_num(i) < len_num(NUM_OF_NODES)) {
			//в зависимости на сколько меньше столько
			//нулей вперед и добавить
			int difference = len_num(NUM_OF_NODES) - len_num(i);
			for (int i = 0; i < difference; ++i)
				num = strcat(num, "0");

			reverse(num);
		}
		v_name = strcat(v_name, num);

		free(num);
		num = NULL;

		graph = strcat(graph, v_name);
	}
/*
	printf ("\n");
	printf ("%s", graph);
	printf ("\n");
*/
	return graph;
}

//error, write arguments in one line
/*
void tr_matrix_to_dot(int matrix[][NUM_OF_NODES])
{
//	int column, row;
//	column = row = NUM_OF_NODES;
//	if (matrix[0][0] != 1)
//		printf("%i\n", matrix[0][0]);

	char* nodes_names;
	nodes_names = gen_vertexes_names();

	//количество байт для сдивига в nodes_names
	size_t vertex_name_len = 6 + len_num(NUM_OF_NODES);
	
	//печатаем 1 раз имя текущей вершины
	char* temp_vertex;
	temp_vertex = malloc((vertex_name_len+1) * sizeof(char));
	temp_vertex[0] = '\0';

	int done = 0;
//	for (int i = 0; i < row; ++i) { // идем по строкам
	for (int i = 0; i < NUM_OF_NODES; ++i) { // идем по строкам
//		for (int j = 0; j < column; ++j) { // по столбцам
		for (int j = 0; j < NUM_OF_NODES; ++j) { // по столбцам
			if (matrix[i][j] == 1) {
				//записываем вершину от которой пойдут
				//потомки
				if (done == 0) {
					memcpy(temp_vertex, 
					  nodes_names + 
					  (i * (vertex_name_len*sizeof(char))),
					  vertex_name_len);
					temp_vertex[vertex_name_len] = '\0';
					fprintf (f, "%s", temp_vertex);
					done = 1;
				}
				//записываем потомков
				temp_vertex[0] = '\0';
				fprintf (f, "%s", _dir_con());

				memcpy(temp_vertex, nodes_names +
			               (j * vertex_name_len * sizeof(char)),
			               vertex_name_len);//maybe error

				temp_vertex[vertex_name_len] = '\0';

				fprintf (f, "%s", temp_vertex);
				temp_vertex[0] = '\0';
			}
//			if (j == column-1 && done == 1) {
			if (j == NUM_OF_NODES-1 && done == 1) {
				done = 0;
				//в конце ;
				fprintf (f, "%s", sep[4]);
				// и после чего \n
				fprintf (f, "%s", "\n");
			}
		}
	}

	if (temp_vertex != NULL) {
		free(temp_vertex);
		temp_vertex = NULL;
	}
}
*/

void tr_matrix_to_dot(int matrix[][NUM_OF_NODES])
{
	char* nodes_names;
	nodes_names = gen_vertexes_names();

	//количество байт для сдивига в nodes_names
	size_t vertex_name_len = 6 + len_num(NUM_OF_NODES);
	
	//печатаем 1 раз имя текущей вершины
	char* temp_vertex;
	temp_vertex = malloc((vertex_name_len+1) * sizeof(char));
	temp_vertex[0] = '\0';

	int done = 0;
	for (int i = 0; i < NUM_OF_NODES; ++i) { // идем по строкам
		for (int j = 0; j < NUM_OF_NODES; ++j) { // по столбцам
			if (matrix[i][j] == 1) {
				//записываем вершину от которой пойдут
				//потомки
				memcpy(temp_vertex, 
				       nodes_names + 
				       (i * (vertex_name_len*sizeof(char))),
				       vertex_name_len);
				temp_vertex[vertex_name_len] = '\0';
				fprintf (f, "%s", temp_vertex);
				//записываем потомков
				temp_vertex[0] = '\0';
				fprintf (f, "%s", _dir_con());

				memcpy(temp_vertex, nodes_names +
			               (j * vertex_name_len * sizeof(char)),
			               vertex_name_len);//maybe error

				temp_vertex[vertex_name_len] = '\0';

				fprintf (f, "%s", temp_vertex);
				temp_vertex[0] = '\0';
				//в конце строки;
				fprintf (f, "%s", sep[4]);
				// и после чего \n
				fprintf (f, "%s", "\n");
			}
		}
	}

	if (temp_vertex != NULL) {
		free(temp_vertex);
		temp_vertex = NULL;
	}
}

void tr_list_to_dot(struct vertex_list* vlist)
{
	printf("test2\n");

	char* nodes_names;
	nodes_names = gen_vertexes_names();
	//	printf ("\n%s\n", nodes_names);

	//количество байт для сдивига в nodes_names
	size_t vertex_name_len = 6 + len_num(NUM_OF_NODES);
	
	//печатаем 1 раз имя текущей вершины
	char* temp_vertex;
	temp_vertex = malloc((vertex_name_len+1) * sizeof(char));

	size_t j = 0;
	struct vertex_list* i;
//	for (i = vlist; j < NUM_OF_NODES; i = i + sizeof(struct vertex_list*)) {
      	for (i = vlist; j < 19; i = i + sizeof(struct vertex_list*)) {
		if ( vertex_list_state(i) == EMPTY ) {
			j++;
			continue;
		}

		//через -> печатаем вершины с которыми есть соединение
		struct vertex_list_node* node;
//		for (node = vlist->head; node != NULL; node = node->next) {
		for (node = i->head; node != NULL; node = node->next) {

			memcpy(temp_vertex, 
			       nodes_names + (j*(vertex_name_len*sizeof(char))),
			       vertex_name_len);
			temp_vertex[vertex_name_len] = '\0';

			fprintf (f, "%s", temp_vertex);
			temp_vertex[0] = '\0';

			fprintf (f, "%s", _dir_con());
			//memcpy error
			memcpy(temp_vertex, 
			       nodes_names+(node->data->weight * 
					    (vertex_name_len * sizeof(char))),
			       vertex_name_len);

			temp_vertex[vertex_name_len] = '\0';//del
			fprintf (f, "%s", temp_vertex);
			temp_vertex[0] = '\0';

			//в конце строки;
			fprintf (f, "%s", sep[4]);
			// и после чего \n
			fprintf (f, "%s", "\n");
		}
		j++;
		printf("%i\n", j);
	}
	if (temp_vertex != NULL) {
		free(temp_vertex);
		temp_vertex = NULL;
	}
}

/*
int main(int argc, char** argv)
{
	print_head();

//	char* nodes_names;
//	nodes_names = gen_vertexes_names();
//	tr_list_to_dot(vlist);
	char* t = gen_vertexes_names();
//	for(int i = 0; i < 5; ++i)
//		printf("%c ", t[i]);

//	printf("%zu\n", strlen(t));

	printf("\n%s\n", t);

	print_end();

	return 0;
}
*/

//digraph{

	//connections

//	var1 -> oper1;
//	var1 ->	oper2;
//	var2 -> var12 -> oper4 -> var124 -> oper7;
//	var3 -> oper2;
//	var4 -> oper2;
//	var5 -> oper2 -> var345 -> oper5;
//	var6 -> oper4;
//	var7 -> oper3 -> var73 -> oper5 -> var34573 -> oper6;
//	var8 -> oper6 -> var348 -> oper7 -> var3486 -> oper8 -> var_result;

/*
	var1 -> oper1;
	var2 -> oper1;
	var12 -> oper4;
	var124 -> oper7;
	var3 -> oper2;
	var4 -> oper2;
	var5 -> oper2; 
	var345 -> oper5;
	var6 -> oper4;
	var7 -> oper3;
	var73 -> oper5;
	var34573 -> oper6;
	var8 -> oper6;
	var348 -> oper7;
	var3486 -> oper8;

	oper2 -> var345;
	oper3 -> var73;
	oper5 -> var34573;
	oper6 -> var348;
	oper4 -> var124;
	oper7 -> var3486;
	oper1 -> var12;
	oper8 -> var_result;
*/
//      }
