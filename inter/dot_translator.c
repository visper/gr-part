/*
в этот файл каким-то макаром должна попасть матрица
*/
#include <stdio.h>
#include <errno.h>

#include "dot_interpreter.h"


int main(int argc, char **argv)
{

	/*
	  open file, option "w+" for overwrite file every time
	*/
	char * PATH;
	PATH = "/home/vis/work/graph_creator/graph.gv";
	
	FILE *stream;
	if((stream = fopen(PATH,"w+")) == NULL){
		fprintf(stderr, "%s error, can`t open file\n", PATH);
	}
	
	
	//this command for print into file
	/*
	  int d = 5;
	  fprintf(stream, "%d",d);
	*/
	
	//translator consist of
	
	/*Lexer*/
	
	/*Parser*/
	
	/*Code generator*/
	
	return 0;
}
