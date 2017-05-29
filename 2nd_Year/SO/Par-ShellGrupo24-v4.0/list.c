/*
 * list.c - implementation of the integer list functions 
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"



list_t* lst_new()
{
   list_t *list;
   list = (list_t*) malloc(sizeof(list_t));
   list->first = NULL;
   return list;
}


void lst_destroy(list_t *list)
{
	struct lst_iitem *item, *nextitem;

	item = list->first;
	while (item != NULL){
		nextitem = item->next;
		free(item);
		item = nextitem;
	}
	free(list);
}


void insert_new_process(list_t *list, pid_t pid, time_t starttime)
{
	lst_iitem_t *item;

	item = (lst_iitem_t *) malloc (sizeof(lst_iitem_t));
	item->pid = pid;
	item->starttime = starttime;
	item->endtime = 0;
	item->status = -1;
	item->next = list->first;
	list->first = item;
}


int update_terminated_process(list_t *list, pid_t pid, int status, time_t endtime, int iteration, int total_time, FILE *myfile)
{
	lst_iitem_t *item;
	
	for(item = list->first; item!=NULL && item->pid!=pid; item = item->next);
	if(item != NULL) {
		item->endtime = endtime;
		item->status = status;
		total_time = lst_print(item, iteration, total_time, myfile);
	}
	else { 
		perror("updating problem");
	}
	return total_time;
}


int lst_print(lst_iitem_t *item, int iteration, int total_time, FILE *myfile)
{
	if(WIFEXITED(item->status)){
		int difference = item->endtime - item->starttime;
		total_time += difference;
		fprintf(myfile, "iteracao %d\npid: %d execution time %d s\ntotal execution time: %d s\n", iteration, item->pid, difference, total_time);
		fflush(myfile);
	}
	else{
		printf("Process: %d(PID) ended without calling exit\n", item->pid);
	}
	return total_time;
}
