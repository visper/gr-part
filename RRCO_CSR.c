/*
3 массива:
1 - JA - хранит подряд столбцы в которых есть цифры
2 - AN - хранит список цифр подряд которые есть в матрице 
соответственно JA и AN по дляне одинаковые
3 - IA - последний элемент массива - число не нулевых элементов + 1
все остальные элементы по индекспм говорят о том какой номер по счету будет
у элемента в следующей строке. т.е. каждый индекс массива говорит о том что
(индекс - строка) в данной строке номер элемента начинается с такого-то номера

разряженная матрица in english
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "right_list.h"

//when programm worked like .h delete this
#define NUM_OF_NODES 100
//when programm worked like .h delete this

#define ERROR   -1
#define ONE     1
#define EMPTY   0
#define MORE    2

struct csr {
	struct vertex_list* ja; // columns
	struct vertex_list* an; // values
	struct vertex_list* ia; // row_ptr
};

/*
void init
void fini
int add
int ins
void rm
*/

void rrco_ini(struct csr* vcsr)
{
	ALLOC_PTR(vcsr->ia);
	if (vcsr->ia == 0)
		return;

	ALLOC_PTR(vcsr->an);
	if (vcsr->an == 0)
		return;

	ALLOC_PTR(vcsr->ja);
	if (vcsr->ja == 0)
		return;

	vertex_list_ini(vcsr->ia);
	vertex_list_ini(vcsr->an);
	vertex_list_ini(vcsr->ja);
}


//fuu for testing len of AN and JA
//they must be equal
//@1 - error
bool csr_test(const struct csr* vcsr)
{
	size_t ja = 0;
	//here maybe temp_ja I must write not inside fuu for write temp...=NULL
	struct vertex_list_node* temp_ja;
	for (temp_ja = vcsr->ja->head; temp_ja != NULL; ++ja)
		temp_ja = temp_ja->next;

	size_t an = 0;
	struct vertex_list_node* temp_an;
	for (temp_an = vcsr->an->head; temp_an != NULL; ++an)
		temp_an = temp_an->next;

	if (ja != 0 || an != 0)
		if (vcsr->ia == NULL)
			return 1;

	return ja != an ? 1 : 0;
}

//for testing num of elements
int csr_state(const struct csr* vcsr)
{
	//проверка на ошибку с количестве элементов
	int rc = csr_test(vcsr);
	if (rc != 0)
		return ERROR;

	//проверка каждого элемента на количество элементов,они должны совпадать
	//только в том случае если списки пусты или в списке только 1 элемент
	//или они пусты
	int ja = vertex_list_state(vcsr->ja);
	int an = vertex_list_state(vcsr->an);
	int ia = vertex_list_state(vcsr->ia);

	//если была только инициализация
	if (ja == EMPTY && an == EMPTY && ia == EMPTY)
		return EMPTY;

	//если всего в списке 1 элемент
	if (ja == ONE && an == ONE && ia == ONE)
		return ERROR; // because ia != ONE, always more than 1

	if (ja == ONE && an == ONE && ia == MORE)
		return ONE;

	//если много элементов
	if (ja == MORE && an == MORE && ia == MORE)
		return MORE;

	if (ja != an && ja != ia) {
		perror("something wrong with state ");
		return ERROR;
	}

	perror("no one case was not chosen in state");
	return ERROR;
}

int count_ia(struct csr* vcsr)
{
	struct vertex_list_node* temp;
	int counter = 0;
	for (temp = vcsr->ia->head; temp != vcsr->ia->tail; ++counter)
		temp = temp->next;

	return counter;
}

//need to know about size of matrix,
//fuu need for add info about data and column
//ja - column
//an - value
//add and insert in one fuu
//first element must be added to first(zero 0 ) row!!
int csr_add_ins(struct csr* vcsr, struct vertex* vertex, int row, int column)
{
//	assert(row < NUM_OF_NODES);
//	assert(column < NUM_OF_NODES);

	if (row >= NUM_OF_NODES || column >= NUM_OF_NODES)
		return ERROR;

	//here test col and row, for understand in what place elem must be add
	//если все строки заполнены количество элементов ia = NUM_OF_NODES+1
	//проверяется количесво элементов, т.е. последний элемент в ia
	//если строка пропущена то будут повторения чисел в ia

	int rc = csr_state(vcsr);

	if (rc == ERROR) {
		perror("some error in struct in csr_state");
		return ERROR;
	}
	if (rc == MORE) {
		//check maybe insert
		int rc1;
		rc1 = count_ia(vcsr);
		//случай если есть элементы только в первой строке
		//row counted from 0
		if (rc1 == 2 && row == 0 
		    && vcsr->ia->tail->data->weight < NUM_OF_NODES) {

			//проверка чтобы column был меньше использованых
			//если меньше - тогда insert
			if (vcsr->ja->tail->data->weight < NUM_OF_NODES
			    && vcsr->ja->tail->data->weight < column) {
				//maybe not weight
				//add column
				struct vertex* col = NULL;
				ALLOC_PTR(col);
				if (col == 0)
					return -ENOMEM;
				
				col->weight = column;
				int rc2 = vertex_list_add(vcsr->ja, col);
				assert(rc2 == 0);
				
				//add value
				rc2 = vertex_list_add(vcsr->an, vertex);
				assert(rc2 == 0);
				
				++vcsr->ia->tail->data->weight;
				
				rc2 = csr_test(vcsr);
				assert(rc2 == 0);
				//return 0;
				//here not return because check in end
			}
			else { //here we must find elem to insert
				//must find element
				//START FIND
				struct vertex_list_node* node;
				//нельзя добавить перед head
				for (node = vcsr->ja->tail; node != NULL; node->prev) {
					if (node->data->weight < column) {
						//add column
						struct vertex* col = NULL;
						ALLOC_PTR(col);
						if (col == 0)
							return -ENOMEM;
						
						col->weight = column;
						int rc2 = vertex_list_ins(vcsr->ja, 
									  col, NULL, node);
						asssert(rc2 == 0);
						//rc1 - is count
						int count = 0;
						for (node = vcsr->an->head;node != NULL;
						     node->next, ++count) {
							if (count == rc1) {
								int rc3 = vertex_list_ins(vcsr->an, vertex,
											  NULL, node);
								asssert(rc3 == 0);
								++vcsr->ia->tail->data->weight;
								rc3 = csr_state(vcsr);
								return rc == ERROR ? ERROR : 0;
							}
							else {
								perror("some error in find element");
								return ERROR;
							}
						}
						perror("some error in find element_rc3");
						return ERROR;
					}
					else {
						perror("some error in find element");
						return ERROR;
					}
				}
				perror("some error in find element_rc3");
				return ERROR;
				//END FIND
			}//else if (vcsr->ja->tail->data->weight < NUM_OF_NODES....
		}//if (rc1 == 2 && row == 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {

		//случай когда нужно добавить не в первую строку и в ia больше двух элементов
		if (rc1 == 2 && row != 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {




		}

		//случай когда нужно добавить не в первую строку и в ia больше двух элементов
		if (rc1 == 2 && row != 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {


		}
	}//MORE
	if (rc == ONE) { // first element must be in first row
		//check maybe insert



	}
	//в этом случае пофиг insert или add должен писаться только в первую строку
	if (rc == EMPTY) {
		rc = vertex_list_add(vcsr->an, vertex);
		assert(rc == 0);

		struct vertex* col = NULL;
		ALLOC_PTR(col);
		if (col == 0)
			return -ENOMEM;
		col->weight = column;
		rc = vertex_list_add(vcsr->ja, col);
		assert(rc == 0);

		//create row_ptr
		struct vertex* rrow = NULL;
		ALLOC_PTR(rrow);
		if (rrow == 0)
			return -ENOMEM;
		rrow->weight = 0; //count elemnts from "0"
		rc = vertex_list_add(vcsr->ia, rrow);
		assert(rc == 0);

		struct vertex* rrow_1 = NULL;
		ALLOC_PTR(rrow_1);
		if (rrow_1 == 0)
			return -ENOMEM;
		//number of elements in list
		rrow->weight = 1; // maybe "1" or "n+1"
		rc = vertex_list_add(vcsr->ia, rrow_2);
		assert(rc == 0);
	}

//at last check
	rc = csr_state(vcsr);
	return rc == ERROR ? ERROR : 0;
}

// ТУТ ПРОВЕРКА ЧТОБЫ IA НЕ БЫЛ ПУСТ
	//тест не превышается ли количество допустимых вершин
	if (vcsr->ia->tail->data->weight == NUM_OF_NODES-1)
		return ERROR;


//here test for insert if data must not be in tail

//before this must be check row, down write case in what add in last writen row

//here must be check for line in row if 
	if (vcsr->ja->tail->data->weight < NUM_OF_NODES && 
	    vcsr->ja->tail->data->weight < column) {// maybe not weight
		//add column
		struct vertex* col = NULL;
		ALLOC_PTR(col);
		if (col == 0)
			return -ENOMEM;

		col->weight = column;
		int rc = vertex_list_add(vcsr->ja, col);
		assert(rc == 0);

		//add value
		rc = vertex_list_add(vcsr->an, vertex);
		assert(rc == 0);

		++vcsr->ia->tail->data->weight;

		rc = csr_test(vcsr);
		assert(rc == 0);
		return 0;
	}


//void rm

void fini(struct csr* vcsr)
{
	vertex_list_fini(vcsr->ja);
	vertex_list_fini(vcsr->an);
	vertex_list_fini(vcsr->ia);
	//maybe here need test
	if (vcsr->ja != 0) {
		free(vcsr->ja);
		vcsr->ja = NULL;
	}
	if (vcsr->an != 0) {
		free(vcsr->an);
		vcsr->an = NULL;
	}
	if (vcsr->ia != 0) {
		free(vcsr->ia);
		vcsr->ia = NULL;
	}
}

int main(int argc, char** argv)
{
	struct csr* csr = 0;
	ALLOC_PTR(csr);
	if (csr == 0) 
		return -ENOMEM;

	rrco_ini(csr);
	int rc = csr_test(csr);
	assert(rc == 0);

	rc = csr_state(csr);
	if (rc == ERROR) {
		printf("test\n");
		printf("rc after csr_state: %i\n", rc);
		return -1;
	}

	printf("rc after csr_state: %i\n", rc);


/*
	fini(csr);
	if (csr != 0) {
		free (csr);
		csr = NULL;
	}
*/
	return 0;
}
