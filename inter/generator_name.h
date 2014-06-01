/*
в этот файл каким-то макаром должна попасть матрица
*/
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

#include <string.h>

#include "dot_translator.h"

// reverse:  переворачиваем строку s на месте 
void reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

// itoa:  конвертируем n в символы в s 
void itoa(int n, char s[])
{
	int i, sign;
 
	if ((sign = n) < 0)  // записываем знак 
		n = -n;          // делаем n положительным числом 
	i = 0;
	do {        //генерируем цифры в обратном порядке 
		s[i++] = n % 10 + '0';    //берем следующую цифру 
	} while ((n /= 10) > 0);     // удаляем 
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

char* get_time(void)
{
	time_t t;
	struct tm *t_m;
	t = time(NULL);
	t_m = localtime(&t);

	char* time;
	time = malloc(15*sizeof(char));
	itoa(t_m->tm_hour, time);
	time = strcat(time, ".");

	char* min;
	min = malloc(2*sizeof(char));
	itoa(t_m->tm_min, min);
	time = strcat(time, min);
	time = strcat(time, ".");

	char* sec;
	sec = malloc(2*sizeof(char));
	itoa(t_m->tm_sec, sec);
	time = strcat(time, sec);
	time = strcat(time, ".");

	char* day_of_year;
	day_of_year = malloc(3*sizeof(char));
	itoa(t_m->tm_yday, day_of_year);
	time = strcat(time, day_of_year);
	time = strcat(time, ".");

	time = strcat(time, "gv");

	free(day_of_year);
	free(min);
	free(sec);

	return time;
}
/*
int main(int argc, char **argv)
{

	printf("%s\n", get_time());
	/*
	  open file, option "w+" for overwrite file every time
	*/
/*
	char * PATH;
	PATH = "/home/vis/work/graph_creator/graph.gv";
	
	FILE *stream;
	if((stream = fopen(PATH,"w+")) == NULL){
		fprintf(stderr, "%s error, can`t open file\n", PATH);
	}
	
*/
	//this command for print into file
	/*
	  int d = 5;
	  fprintf(stream, "%d",d);
	*/
	
	//translator consist of
	
	/*Lexer*/
	
	/*Parser*/
	
	/*Code generator*/
/*	
	return 0;
}
*/
