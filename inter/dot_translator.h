/*
 *this file include all avaliable words, operations in dot language
 *
 *all functions returned avaliable expressions
 *in dot language for requests parameters  
 *or for request funcions
*/


/*
  enumeration of all avaliable separators
  * 0 - {
  * 1 - }
  * 2 - [
  * 3 - ]
  * 4 - ;
  * 5 - "
  * 6 - =
  * 7 - ,
  * 8 - .
  * 9 - " "
*/
char* sep[] = {"{", "}", "[", "]", ";", "\"", " = ", ",", ".", " ", "\0"};

/*
  definition function of directed graph
*/
char* _dir(void)
{
	return "digraph";
}

/*
  definition function of undirected graph
*/
char* _undir(void)
{
	return "graph";
}

/*
  definition function of directed graph
*/
char* _dir_con(void)
{
	return "->";
}

/*
  connection for underected graph
*/
char* _undir_con(void)
{
	return "-";
}

