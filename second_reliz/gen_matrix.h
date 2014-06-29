#include <sys/time.h>

//#define NUM_OF_NODES 50

//generated gen values
int gen_values(int start_value, int finish_value)
{
  //	srand(0xCAFEBABE);
	struct timeval t;
	gettimeofday (&t, NULL);
	srand(t.tv_usec * t.tv_sec);
	return start_value + rand() %finish_value;
}

//fuu count zeroes in line of matrix
int count_no_zero(int matrix[NUM_OF_NODES][NUM_OF_NODES], int row)
{
	int count = 0;
	for (size_t i = 0; i < NUM_OF_NODES; ++i) {
		if (matrix[row][i] == 1)
			++count;
	}
	return count;
}

//do that matrix on main diagonal have only zero and in down triangle 
//only zero too
// all down triangle == 0 and no loop
void up_triangle(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	for(size_t i = 0; i < NUM_OF_NODES; ++i) {
		for(size_t j = 0; j < NUM_OF_NODES; ++j) {
			if (matrix[i][j] == 1) 
				matrix[i][j] = 0;

			if (j > i)
				break;
		}
	}
}

// delete zero lines
void del_zero_lines(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	for(size_t i = 0; i < NUM_OF_NODES; ++i) {
		if (i == NUM_OF_NODES-1) //on last line must no elements
			break;

		int rc;
		rc = count_no_zero(matrix, i);
		if (rc == 0 && i > NUM_OF_NODES - 6) { // 6 lines ca be empty
			int node = gen_values(i, NUM_OF_NODES);
			while (node < i && node >= NUM_OF_NODES) {
				node = gen_values(i, NUM_OF_NODES);
			}
//	      			assert (node >= i && node < NUM_OF_NODES);
			matrix[i][node] = 1;
		}
	}
}

//строим матрицу с единицами над диаганалью
//AHTUNG!!! хороший вриант генерации матрицы одной функцией,
// но получается оч много связей
void matrix_gen_right(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	//первый элемент всегда должен быть в первой строке
	//в каждой строке хотябы одна вершина
	for (size_t i = 0; i < NUM_OF_NODES; ++i) {
		//сначала методом генерации случайного числа
		//выбираем количество вершин в строке
		// их может быть от 1 до NUM_OF_NODES/2
		//чтобы сохранить правило разряженой матрицы
		int num_nodes = gen_values(i, ((NUM_OF_NODES/2)-1));
//		if (num_nodes > NUM_OF_NODES)
//			continue;

		if (i > 0) { // step back if num of elem in line == 0
			if (count_no_zero(matrix, i-1) == 0)
				--i;
		}

		//generation elements in line
		for (size_t j = 0; j < num_nodes; ++j) {
			//в строке должно быть вершин больше 0
			//но меньше NUM_OF_NODES/2

			if (i == (NUM_OF_NODES-1)) // in last line must no elem
				break;

			int new_elem = 0;			
			new_elem = gen_values(i + 1, NUM_OF_NODES);
			while (new_elem <= i || new_elem > NUM_OF_NODES) {
				new_elem = gen_values(i + 1, NUM_OF_NODES);
			}
			matrix[i][new_elem] = 1;

//			if (j == (num_nodes - 1))
//				prev = 0;

/*
			if (prev < i)
				continue;
			if (prev > NUM_OF_NODES)
				continue;
			if (i == prev) //delete main diagonal
				continue;
*/
		}
	}
	//если использовать этот способ нужно будет подчистить 
	//под диагональю
}

//generate random matrix
void matrix_gen(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	//первый элемент всегда должен быть в первой строке
	//в каждой строке хотябы одна вершина

//	matrix_ini(matrix);

	for (size_t i = 0; i < NUM_OF_NODES; ++i) {
		//сначала методом генерации случайного числа
		//выбираем количество вершин в строке
		// их может быть от 1 до NUM_OF_NODES/2
		//чтобы сохранить правило разряженой матрицы
		int num_nodes = gen_values(1, ((NUM_OF_NODES/2)-1));
		int prev = 0;
		if (i > 0) {
			if (count_no_zero(matrix, i-1) == 0)
				--i;
		}

		for (size_t j = 0; j < num_nodes; ++j) {
			//в строке должно быть вершин больше 0
			//но меньше NUM_OF_NODES/2

			prev = gen_values(prev, NUM_OF_NODES); //error
			if (prev > NUM_OF_NODES)
				continue;
			if (i == prev)
				continue;
			matrix[i][prev] = 1;

			if (j == (num_nodes - 1))
				prev = 0;
		}
	}
}

//foo for printing of matrix
void matrix_print(int matrix[NUM_OF_NODES][NUM_OF_NODES])
{
	printf("{\n");
	for (size_t i = 0; i < NUM_OF_NODES; ++i) {
		for (size_t j = 0; j < NUM_OF_NODES; ++j) {
			printf ("%i ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("}\n");
}
