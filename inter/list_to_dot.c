#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator_name.h"
//char* get_time();

//#include "dot_translator.h"

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

	fprintf(f, "%s", _dir());
	fprintf(f, "%s", sep[9]);
	fprintf(f, "%s", sep[0]);
	fprintf(f, "\n");

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
	graph = malloc(vertex_name_len * NUM_OF_NODES * sizeof(char));
//	if (graph == 0)
//		return -ENOMEM;

	for (int i = 0; i < NUM_OF_NODES; ++i) {
		//v_name имя одной вершины
		char* v_name;
		v_name = malloc(vertex_name_len * sizeof(char));
//		if (v_name == 0)
//			return -ENOMEM;

		v_name = strcat(v_name, "vertex");

		char* num = NULL;
		num = malloc(len_num(NUM_OF_NODES) * sizeof(char));
//		if (num == 0)
//			return -ENOMEM;

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



int main(int argc, char** argv)
{
//	print_head();
	char* nodes_names;
	nodes_names = gen_vertexes_names();


	return 0;
}


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
