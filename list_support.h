//work with graph through list

//количество вершин с которыми смежна вершина
int line_zero(struct vertex_list* vlist)
{
	struct vertex_list_node* scan = NULL;
  //	struct vertex_list_node* scan = vlist->head;
	int count = 0;
	for (scan = vlist->head; scan != NULL; scan = scan->next) {
	  //for (; scan != NULL; scan = scan->next) {
		if (scan->data->weight != 0)
			++count;
	}
	//	assert (count < NUM_OF_NODES);
	return count;
}

int vertex_num(struct vertex_list* vlist)
{
	size_t j = 0;
	struct vertex_list* temp = vlist;
	//	temp = vlist;
	int count_nodes = 0;
	for(; j < NUM_OF_NODES; temp = temp + sizeof(struct vertex_list*)) {
		count_nodes += line_zero(temp);
		j++;
	}
	return count_nodes;
}
