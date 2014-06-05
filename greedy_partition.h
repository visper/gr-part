//first realisation of greedy_partition
//NOT WORKED

int greedy_partition(const struct vertex_list* vlist)
{
	struct vertex_list* list1 = NULL;
	struct vertex_list* list2 = NULL;

	ALLOC_PTR(list1);
	if (list1 == 0)
		return -ENOMEM;

	ALLOC_PTR(list2);
	if (list2 == 0)
		return -ENOMEM;

	int rc = 0;
	rc = create_list(list1);
	assert(rc == 0);

	rc = create_list(list2);
	assert(rc == 0);

	//for marked nodes
	struct vertex_list3* three_list = NULL;
	ALLOC_PTR(three_list);
	if (three_list == 0)
		return -ENOMEM;

	vertex_list_init3(three_list);
	struct vertex3* v1 = NULL;
	ALLOC_PTR(v1);
	if (v1 == 0)
		return -ENOMEM;
	v1->weight = 1;

	struct vertex3* v2 = NULL;
	ALLOC_PTR(v2);
	if (v2 == 0)
		return -ENOMEM;
	v2->weight = 1;

	rc = vertex_list_add3(three_list, v1);
	rc = vertex_list_add3(three_list, v2);

	struct now {
		row;
		column;
	}

	struct now work1;
	struct now work2;

	if (line_zero(vlist) > 2) {
		struct vertex* t1 = NULL;
		struct vertex* t2 = NULL;

		struct vertex_list_node* scan = NULL;
		int done = 0;
		int done_4_list2 = 0;
		//выбираем первый и последние элементы в первой строке
		for(scan = vlist->head; scan != NULL; scan->next) {
			if(done == 0 && t1 == NULL && scan->data->weight != 0) {
				t1 = scan->data;
				done = 1;
				three_list->head->data->row = 0;
				three_list->head->data->column = 
					scan->data->weight;
				work1.column = scan->data->weight;
				work1.row = 1;
				rc = add(list1, scan->data, 0);
				assert (rc == 0);
			}
			if (done == 1 && scan->data->weight != 0) {
				t2 = scan->data;
				three_list->tail->data->row = 0;
				three_list->tail->data->column = 
					scan->data->weight;
				work2.column = scan->data->weight;
				work2.row = 1;
				if (done_4_list2 == 0) {
					rc = add(list2, scan->data, 0);
					assert (rc == 0);
					done_4_list2 = 1;
				}
				else {
					list2->head->data = scan->data;
				}
			}
		}
		rc = add(list1, t1, 0);
		assert (rc == 0);

		rc = add(list2, t2, 0);
		assert (rc == 0);
	}
	else {
		perror ("error chose vertex");
		return -1;
	}
	//first 2 vertex are chosen and marked
/*
-Среди всех немаркированных вершин соседних к этой части, выбирается одна с 
минимальным числом немаркированных соседей
-Маркировать её и добавить в текущую часть.
Пока эта часть не станет достаточно большой.

Если не маркированные вершины остались, выбираем одну смежную к текущей части 
с минимальным числом немаркированных соседей как начальную вершину для 
следующей части.
*/
	int chose_part = 0; // 0 - first, 1 - second

	struct now work;

	struct vertex_list* list = NULL;
	struct vertex_list* find = NULL;
	do {
		if (chose_part == 0) {
			list = list1;
			work = work1;
		}
		if (chose_part == 1) {
			list = list2;
			work = work2;
		}

//-Среди всех немаркированных вершин соседних к этой части, выбирается одна с 
//минимальным числом немаркированных соседей
		//обход всего подграфа это цикл
		size_t j = 0;
		struct vertex_list* tt = NULL;
		tt = list + j * sizeof(struct vertex_list*);
		for (; j < NUM_OF_NODES; 
		     tt = list + j * sizeof(struct vertex_list*)) {

			//тут будет работа с vlist, т.к. в list
			//уже куски от главной матрицы
			//находим в vlist соответствующий элемент из list
			struct vertex_list_node* vlist_equal = NULL;
			vlist_equal = vlist + j * sizeof(struct vertex_list*);
			//сейчас сравниваемые строки равны vlist_equal и tt
			//теперь пройтись по строке в list и найти соответствие 
			//в vlist_equal
			struct vertex_list_node* scan_line = NULL;
			for (scan_line = tt->head; scan_line != 0; 
			     scan_line = scan_line->next) {
				//сейчас выбирается последовательно 
				//каждый элемент из подграфа и ищется 
				//соответствие в vlist
				struct vertex_list_node* vfind = NULL;
				vfind = vertex_list_find (vlist, 
							  scan_line->data);



			}




			//осматриваем все его смежные вершины

			//каждой вершине ищим смежные 

			//пробегаемся по строке

			//в каждом элементе строки есть смежная вершина
			
			

		  for (size_t i = 0; i < NUM_OF_NODES; ++i) {
			  if ()

		  }
			//проверяем на маркировку
		//смотрим на вершины соседнии с ним по очереди

		//проверяем каждую вершину, маркирована или нет
		            //если не маркирована сколько не маркированых рядом


			++j;
		}
		tt = NULL;



		//НЕПРАВИЛЬНО!!!
		find = vlist + work.column * sizeof(struct vertex_list*);
		//если строка пуста
		if (line_zero(find) == 0) {
			//если список пуст нужно искать другую вершину
			//для добавления

		}
		else {
			for ()


		}

	}
	while()

}
