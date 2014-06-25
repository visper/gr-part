#include "right_list_two.h"
#include "right_list.h"

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

//when programm worked like .h delete this
#define NUM_OF_NODES 100

struct csr {
	struct vertex_list2* jan; // columns, values
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
	ALLOC_PTR(vcsr->jan);
	if (vcsr->jan == 0)
		return;

	ALLOC_PTR(vcsr->ia);
	if (vcsr->ia == 0)
		return;

	vertex_list_ini(vcsr->jan);
	vertex_list_ini(vcsr->ia);
}


//fuu for testing len of AN and JA
//they must be equal
//@1 - error
bool csr_test(const struct csr* vcsr)
{
	size_t jan = 0;
	struct vertex_list_node2* temp_jan;
	for (temp_jan = vcsr->jan->head; temp_jan != NULL; ++jan)
		temp_jan = temp_jan->next;

	if (jan != 0 && vcsr->ia == NULL)
			return 1;

	return 0;
}

//for testing num of elements
int csr_state(const struct csr* vcsr)
{
	//проверка на ошибку с количеством элементов
	int rc = csr_test(vcsr);
	if (rc != 0)
		return ERROR;

	//проверка каждого элемента csr структуры на количество элементов
	int jan = vertex_list_state2(vcsr->jan);
	int ia = vertex_list_state(vcsr->ia);

	//если была только инициализация
	if (jan == EMPTY && ia == EMPTY)
		return EMPTY;

	//если всего в списке 1 элемент
	if (jan == ONE && ia == ONE)
		return ERROR; // because ia != ONE, always more than 1

	if (jan == ONE && ia == MORE)
		return ONE; //true

	//если много элементов
	if (jan == MORE && ia == MORE)
		return MORE;

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

int count_jan(struct csr* vcsr)
{
	struct vertex_list_node* temp;
	int counter = 0;
	for (temp = vcsr->jan->head; temp != vcsr->jan->tail; ++counter)
		temp = temp->next;

	return counter;
}

//need to know about size of matrix,
//fuu need for add info about data and column
//jan - column,value
//add and insert in one fuu
//first element must be added to first(zero 0 ) row!!
int csr_add_ins(struct csr* vcsr, struct vertex2* vertex, int row)
{
//	assert(row < NUM_OF_NODES);
//	assert(column < NUM_OF_NODES);

	if (row >= NUM_OF_NODES || vertex->column >= NUM_OF_NODES)
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
		//row counted from 0
		//случай если есть элементы только в первой строке
		if (rc1 == 2 && row == 0 
		    && vcsr->ia->tail->data->weight < NUM_OF_NODES) { 
			//проверка чтобы column был меньше использованых
			//если меньше - тогда insert
			if (vcsr->jan->tail->data->column < NUM_OF_NODES
			    && vcsr->jan->tail->data->column < vertex->column) {
				//add column, value
				int rc2 = vertex_list_add2(vcsr->jan, vertex);
				assert(rc2 == 0);
				
				++vcsr->ia->tail->data->weight;
				
				rc2 = csr_test(vcsr);
				assert(rc2 == 0);

				rc2 = csr_state(vcsr);
				return rc == ERROR ? ERROR : 0;
			}
			else { //here we must find elem to insert
				//must find element
				//START FIND
				struct vertex_list_node* node;
				//нельзя добавить перед head
				for (node = vcsr->jan->tail; node != NULL; node->prev) {
					if (node->data->column < column) {
						//add column, weight
						int rc2 = vertex_list_ins(vcsr->jan, vertex, 
									  NULL, node);
						asssert(rc2 == 0);

						++vcsr->ia->tail->data->weight;
				
						rc2 = csr_test(vcsr);
						assert(rc2 == 0);

						rc2 = csr_state(vcsr);
						return rc == ERROR ? ERROR : 0;
					}
				}
				perror("some error in find element_rc2");
				return ERROR;
				//END FIND
			}//if (vcsr->jan->tail->data->column < NUM_OF_NODES &&...
		}//if (rc1 == 2 && row == 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {

		//случай когда нужно добавить не в первую строку и в ia 2 элемента
		if (rc1 == 2 && row != 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {
			//посчитать в какую строку добавлять и соответственно добавить
			//нужное количество повторяющихся чисел
			int count_of_add_elem = row - 1;

			//какой-то алгоритм по добавлению новых элементов
			//temp is count of next num
			assert(vcsr->ia->tail->prev->data->weight != NULL);
			int temp = vcsr->ia->tail->prev->data->weight;
			for (size_t i = 0; i < count_of_add_elem; ++i) {
				struct vertex* ia_repeat = NULL;
				ALLOC_PTR(ia_repeat);
				if (ia_repeat == 0)
					return -ENOMEM;
				ia_repeat->weight = temp;
				rc = vertex_list_ins(vcsr->ia, ia_repeat, NULL, 
						     vcsr->ia->tail->prev);
				assert(rc == 0);
			}
			//at now in "ia" must number of next element repeat
			//equal times to number row

			//add element in tail
			//add column, value
			int rc2 = vertex_list_add2(vcsr->jan, vertex);
			assert(rc2 == 0);
			
			++vcsr->ia->tail->data->weight;
			
			rc2 = csr_test(vcsr);
			assert(rc2 == 0);
			
			rc2 = csr_state(vcsr);
			return rc == ERROR ? ERROR : 0;
		}

		//случай когда нужно добавить не в первую строку и в ia уже больше двух элементов
		if (rc1 > 2 && row != 0 && vcsr->ia->tail->data->weight < NUM_OF_NODES) {
			int col_elem = (count_ia(vcsr) - 1); // всего количество занятых строк
			if (col_elem > row) { // case in what need to insert
				//здесь insert
				//нужно понять куда вставлять

				//вычесть из количества элементов row
				int num_rows_bef_ins = col_elem - row;//в поиске идем с конца
				//эта строка может быть пустой 
				//тогда следующая цифра в списке будет такая же
				struct vertex_list_node* temp = NULL;
				temp = vcsr->ia->tail->prev;
				for (int i = num_rows_bef_ins; i != 0; temp->prev);
/*
//неправильно потому-что здесь нужно испотзовать поиск с count_jan а не ia
				//node элемент после которого вставлять в jan
				struct vertex_list_node2* node = NULL;
				node = vcsr->jan->tail;
				for (int i = num_rows_bef_ins; i != 0; node->prev);
*/

				//нашли элемент списка, но возможно тут ошибочка
				assert(temp->data->weight != NULL);
				assert(temp->next->data->weight != NULL);
				if (temp->data->weight == temp->next->data->weight) {
					//в данном случае строка пуста
					//найти в jan вершину после которой вставлять
					int rc2 = vertex_list_ins2(vcsr->jan, vertex,
								   NULL, node);
					assert(rc2 == 0);
					node = NULL;

					for(;temp != NULL; temp->next)
						++temp->data->weight;
					
					rc2 = csr_test(vcsr);
					assert(rc2 == 0);
					
					rc2 = csr_state(vcsr);
					return rc == ERROR ? ERROR : 0;
				}
				else {
					//строка заполнена нужно найти после какого
					//элемента вставлять


					
					//все последующие числа в ia увеличить на 1
				}



				//понять на какой строке результат и какие номера элементов там
				


				return
			}
			if (col_elem < row) { // case in what need to add elements
				//добавление в ia строки

				//какой-то алгоритм по добавлению новых элементов
				//temp is count of next num
				assert(vcsr->ia->tail->prev->data->weight != NULL);
				int temp = vcsr->ia->tail->prev->data->weight;
				for (size_t i = 0; i < col_elem; ++i) {
					struct vertex* ia_repeat = NULL;
					ALLOC_PTR(ia_repeat);
					if (ia_repeat == 0)
						return -ENOMEM;
					ia_repeat->weight = temp;
					rc = vertex_list_add(vcsr->ia, ia_repeat);
					assert(rc == 0);
				}
				//at now in "ia" must number of next element repeat
				//equal times to number row
				
				//add element in tail
				//add column, value
				int rc2 = vertex_list_add2(vcsr->jan, vertex);
				assert(rc2 == 0);
				
				++vcsr->ia->tail->data->weight;
				
				rc2 = csr_test(vcsr);
				assert(rc2 == 0);
				
				rc2 = csr_state(vcsr);
				return rc == ERROR ? ERROR : 0;



				return
			}
			if (col_elem == row) { // case in what can use case from more prev
				//добавить в ту же строку




				return
			}
		}
	}//MORE
	if (rc == ONE) { // first element must be in first row
		//check maybe insert



	}
	//в этом случае пофиг insert или add должен писаться только в первую строку
	if (rc == EMPTY) {
		rc = vertex_list_add(vcsr->jan, vertex);
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
	if (vcsr->ia->tail->data->weight >= NUM_OF_NODES)
		return ERROR;


//here test for insert if data must not be in tail

//before this must be check row, down write case in what add in last writen row

//here must be check for line in row if 
	if (vcsr->jan->tail->data->column < NUM_OF_NODES && 
	    vcsr->jan->tail->data->column < vertex->column) {// maybe not weight
		//add column, value
		int rc = vertex_list_add(vcsr->jan, vertex);
		assert(rc == 0);

		++vcsr->ia->tail->data->weight;

		rc = csr_test(vcsr);
		assert(rc == 0);
		return 0;
	}


//void rm

void fini(struct csr* vcsr)
{
	vertex_list_fini(vcsr->jan);
	vertex_list_fini(vcsr->ia);
	//maybe here need test
	if (vcsr->jan != 0) {
		free(vcsr->jan);
		vcsr->jan = NULL;
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
