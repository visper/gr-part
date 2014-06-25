//граф бьется на несколько частей рекурсивно
//Граф разбивается как будто графически пополам

//учитывая то что я заранее не знаю как выглядит граф, биться он будет
//так: 1 вершина в первый подграф, 2 вершина во второй подграф.

//функция работает со списками смежностей

//по необходимости включить инклюды

//обе части разбиения выводятся через параметры функции
//int recursive_part(const struct vertex_list* graph,
int recursive_part(struct vertex_list* graph,
		    struct vertex_list* subgraph1,
		    struct vertex_list* subgraph2)
{
	int turn = 0;
	struct vertex_list* lines = NULL; //for work with lines
	int times = 0;
	for (int i = 0; i < NUM_OF_NODES; ++i) {
		lines = graph + i * sizeof(struct vertex_list*); //line nodes
		struct vertex_list_node* nodes = NULL;
		for (nodes = lines->head; nodes != NULL; nodes = nodes->next) {
			assert (nodes->data != NULL);
			if (turn == 0) {
				int rc = add(subgraph1, nodes->data, i);
				assert (rc == 0);

				turn = 1;
				continue;
			}
			if (turn == 1) {
				int rc = add(subgraph2, nodes->data, i);
				assert (rc == 0);

				turn = 0;
				continue;
			}
			perror ("something wrong in patrtition");
			return ERROR;
		}
	}
	return 0;
}
