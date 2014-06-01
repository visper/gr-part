
int greedy_partition (struct vertex_list* vlist, struct vertex_list* vlist1, 
		      struct vertex_list* vlist2)
{
	//структура которая хранит маркированные вершины
	struct vertex_list3* three_list = NULL;
	ALLOC_PTR(three_list);
	if (three_list == 0)
		return -ENOMEM;

	//инициализация структуры которая хранит маркированые вершины
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

	//column можно тоже заполнить так как первые все равно выбираются
	//с первой строки т.е. с нулевой
	v1->row = 0;
	v2->row = 0;

	//создаются две вершины с единичными весами которые будут записаны
	//в структуру для маркировки а так же в две структуры
	//содержащие подграф
	rc = vertex_list_add3(three_list, v1);
	rc = vertex_list_add3(three_list, v2);

	//временная структура для хранения текущего контекста в зависимости
	//от работы с определенной структурой, так же будет использоваться
	//для хранения лучшей вершины для добавления в структура хранящей
	//соответствующий подграф
	struct now {
		row;
		column;
	}

	//соответственно две структуры
	struct now work1, work2;

	//первоначальное разбиение в котором выбираются первые две 
	//вершины от которых пойдет разбиение
	//для этого проверяется первая строка на то чтобы там было больше
	//двух элементов из которых выбирается 2 в качестве первых
	//элементов для двух подграфов
	if (line_zero(vlist) > 2) {
		//выбираем первый и последние элементы в первой строке
		//первый для первого подграфа, второй для второго
		//к vlist ничего не добавляется т.к. ищется в первой строке

		three_list->head->data->row = 0; // первая строка нулевая
		three_list->head->data->column = vlist->head->data->weight;
		work1.column = vlist->head->data->weight;
		work1.row = 0;
		rc = add(list1, vlist->head->data, 0);
		assert (rc == 0);

		three_list->tail->data->row = 0; // первая строка нулевая
		three_list->tail->data->column = vlist->tail->data->weight;
		work1.column = vlist->tail->data->weight;
		work1.row = 0;
		rc = add(list2, vlist->tail->data, 0);
		assert (rc == 0);
	}
	//если меньше двух вершин - ошибка
	else {
		perror ("error chose vertex, num of vertex in first row < 2");
		return -1;
	}

	// this for choce part to work 
	// 0 - first, 1 - second
	int chose_part = 0; 

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

		if (chose_part == 0)
			chose_part = 1;
		else 
			chose_part = 0;
	}
	//если не маркированные вершины остались
	while()








}
