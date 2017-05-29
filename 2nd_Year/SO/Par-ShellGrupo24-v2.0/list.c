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


void update_terminated_process(list_t *list, pid_t pid, int status, time_t endtime)
{
	lst_iitem_t *item;
	for(item = list->first; item!=NULL && item->pid!=pid; item = item->next);
	if(item != NULL) {
		item->endtime = endtime;
		item->status = status;
	}
	else { 
		perror("updating problem");
	}
}


void lst_print(list_t *list)
{
	lst_iitem_t *item;

	for(item = list->first; item!=NULL; item = item->next) {
		if(WIFEXITED(item->status)){
			int difference = item->endtime - item->starttime;
			printf("Process: %d(PID) ended with status %d with execution time %d seconds\n", item->pid, WEXITSTATUS(item->status), difference);
		}
		else{
			printf("Process: %d(PID) ended without calling exit\n", item->pid);
		}
	}
}
