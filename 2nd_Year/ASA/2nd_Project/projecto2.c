/* Libraries*/
#include <stdio.h>
#include <stdlib.h>

/* Constants */
#define TRUE 1
#define FALSE 0
#define INFINITO 2147435647

/* Definition of structures */
typedef struct adj {
	int prev;
	int value;
	int weight;
	struct adj* next;
} Adj;

typedef struct node {
	int distance;
	int sum_d;
	struct adj* next;
} Node;

typedef Node* Node_link;
typedef Adj* Adj_link;

/* Prototypes */
Node_link newNode();
Adj_link addEdge(int v1, int v2, int w, Node_link list[]);
void BellmanFord(Node_link list[], Adj_link edges_list[], int f_index, int nV, int nL);
void OurAlgorithm(Node_link list[], Adj_link edges_list[], int filials[], int nV, int nF, int nL);
void freeMemory(Node_link list[], int nV);

/* MAIN */
int main() {
	int i, f_index, nV, nF, nL, v1, v2, w;
	
	if(scanf("%d %d %d", &nV, &nF, &nL)==-1) perror("Error scanning");
	
	Adj_link edges_list[nL+1];
	Node_link list[nV+1];
	int filials[nF+1];
	
	/* Initiating the adj lists */
	for(i = 1; i<=nV; i++) list[i] = newNode();
	
	/* Adding Filials */
	for(i = 1; i<=nF; i++) {
		if(scanf("%d", &f_index)==-1) perror("Error scanning");
		filials[i] = f_index;
	}
	
	/* Adding Edges */
	for (i = 1; i<=nL; i++) { 
		if(scanf("%d %d %d", &v1, &v2, &w)==-1) perror("Error scanning");
		edges_list[i] = addEdge(v1, v2, w, list);
	}
	
	/* Meetingpoint Finder */
	if(nV+nL>0) OurAlgorithm(list, edges_list, filials, nV, nF, nL);
	
	/* Free all memory */
	freeMemory(list,nV);
	
	return 0;
}


/* Auxiliar Functions */
Node_link newNode() 
{
	Node_link new = (Node_link) malloc(sizeof(Node));
	new->distance = INFINITO;
	new->sum_d = 0;
	new->next = NULL;
	return new;
}

Adj_link addEdge(int v1, int v2, int w, Node_link list[])
{
	Adj_link i;
	Node_link n = list[v1];
	Adj_link new = (Adj_link) malloc(sizeof(Adj));
	new->prev = v1;
	new->value = v2;
	new->weight = w;
	
	i = n->next;
	new->next = i;
	n->next = new;
	
	return new;
}

void BellmanFord(Node_link list[], Adj_link edges_list[], int f_index, int nV, int nL) {
	int i;
	int flag = TRUE;
	Node_link n1, n2;
	Adj_link adja;
	
	/* Reinitiation of the distance variable for all nodes */
	for(i=1;i<=nV;i++) {
		n1 = list[i];
		n1->distance = INFINITO;
	}
	
	/* Set the distance variable as zero for the origin node  */
	n1 = list[f_index];
	n1->distance=0;
	
	/* Relax edges repeatdely */
	while(flag==TRUE) {
		flag = FALSE;
		for (i = 1; i<=nL; i++) { 
			adja = edges_list[i];
			n1 = list[adja->prev];
			n2 = list[adja->value];
			if(n2->distance > n1->distance + adja->weight && n1->distance!=INFINITO) {
				n2->distance = n1->distance + adja->weight;
				flag = TRUE;
			 }
		}
	}
}

void OurAlgorithm(Node_link list[], Adj_link edges_list[], int filials[], int nV, int nF, int nL) {
	int f_index, w, i1, i2, meetingpoint=1;
	int bad_flag=TRUE;
	Node_link v;
	
	/* Calculating sum_d for all nodes */
	for(i1 = 1; i1<=nF; i1++) {
		f_index=filials[i1];
		BellmanFord(list, edges_list, f_index, nV, nL);
		for(i2=1;i2<=nV;i2++){
			v = list[i2];
			if(v->sum_d!=INFINITO && v->distance!=INFINITO) v->sum_d += v->distance;
			else v->sum_d = INFINITO;
		}
	}
	
	/* Meetingpoint determination */
	if(list[1]->sum_d!=INFINITO) bad_flag=FALSE;
	for(i1 = 2; i1<=nV; i1++) {
		if((list[i1])->sum_d < (list[meetingpoint])->sum_d) meetingpoint=i1;
		if((list[i1])->sum_d!=INFINITO) bad_flag=FALSE;
	}
	
	
	/* Writing output */
	if(bad_flag==FALSE) {
		printf("%d %d\n", meetingpoint, (list[meetingpoint])->sum_d);
		for(i1=1;i1<=nF;i1++) {
			BellmanFord(list, edges_list, filials[i1], nV, nL);
			w = (list[meetingpoint])->distance;
			printf("%d ", w);
		}
		printf("\n");
	}
	else {
		printf("N\n");
	}
}

void freeMemory(Node_link list[], int nV) {
	int i;
	Adj_link aux;
	for(i=1; i<=nV; i++) {
		for(aux = list[i]->next; aux!=NULL; aux=aux->next) {
			free(aux);
		}
		free(list[i]);
	}
}
