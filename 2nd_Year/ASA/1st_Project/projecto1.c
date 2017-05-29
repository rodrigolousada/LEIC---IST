/* Libraries*/
#include <stdio.h>
#include <stdlib.h>

/* Definition of structures */
typedef struct adj {
	int value;
	struct adj* next;
} Adj;


typedef struct node {
	int t_discover;
	int low;
	int visited;
	int has_parent;
	int articulation_p;
	struct adj* next;
} Node;

typedef Node* Node_link;
typedef Adj* Adj_link;

/* Prototypes */
Node_link newNode();
void addEdge(int v1, int v2, Node_link list[]);
void TarjanVisit(Node_link u, int time, Node_link list[], Node_link parent);
void freeMemory(Node_link list[], int nV);

/* MAIN */
int main() {
	int i, nV, nL, v1, v2, time = 0, fundamentals = 0, first = -1, last = -1;
	
	if(scanf("%d %d", &nV, &nL)==-1) perror("Error scanning");
	
	Node_link list[nV+1];
	
	/* Initiating the adj lists */
	for(i = 1; i<=nV; i++) list[i] = newNode();
	
	/* Adding Edges */
	for (i = 1; i<=nL; i++) { 
		if(scanf("%d %d", &v1, &v2)==-1) perror("Error scanning");
		addEdge(v1, v2, list);
	}
	
	/* Articulation Point Finder from Tarjan Algorithm */
	if(nV+nL>0)TarjanVisit(list[1], time, list, NULL);
	
	/* Writing articulation points */
	for(i = 1; i<=nV; i++) {
		if(list[i]->articulation_p==1) {
			fundamentals++;
			if(first==-1) first = i;
			last = i;
		}
	}
	if(first!=-1) printf("%d\n%d %d\n", fundamentals, first, last);
	else printf("0\n-1 -1\n");
	
	/* Free all memory */
	freeMemory(list,nV);
	
	return 0;
}


/* Auxiliar Functions */
Node_link newNode() 
{
	Node_link new = (Node_link) malloc(sizeof(Node));
	new->t_discover = -1;
	new->low = -1;
	new->visited = 0;
	new->has_parent = 0;
	new->articulation_p = 0;
	new->next = NULL;
	return new;
}

void addEdge(int v1, int v2, Node_link list[])
{
	Adj_link i;
	Node_link n1 = list[v1];
	Node_link n2 = list[v2];
	Adj_link new1 = (Adj_link) malloc(sizeof(Adj));
	Adj_link new2 = (Adj_link) malloc(sizeof(Adj));
	new1->value = v1;
	new2->value = v2;
	
	i = n1->next;
	new2->next = i;
	n1->next = new2;
	
	i = n2->next;
	new1->next = i;
	n2->next = new1;
}

void TarjanVisit(Node_link u, int time, Node_link list[], Node_link parent) {
	Adj_link a;
	Node_link u2;
	int children = 0;
	u->t_discover = u->low = time++;
	u->visited = 1;
	for(a = u->next; a!=NULL; a = a->next) {
		u2 = list[a->value];
		if (u2->visited==0) {
			u2->has_parent = 1;
			children++;
			TarjanVisit(u2, time, list, u);
			if(u2->low < u->low ) u->low = u2->low;
			if(u->has_parent == 0 && children > 1) u->articulation_p = 1; /* 1 */
			if(u->has_parent == 1 && (u2->low >= u->t_discover)) u->articulation_p = 1; /* 2 */
		}
		else if (u2 != parent) {
			if (u2->t_discover < u->low ) u->low = u2->t_discover;
		}
	}
}

void freeMemory(Node_link list[], int nV) {
	int i;
	Adj_link a, b;
	for(i=1; i<=nV; i++) {
		for(b = list[i]->next, a=b->next; a!=NULL; b=a, a=a->next) {
			free(b);
		}
		if(b!=NULL)free(b);
		free(list[i]);
	}
}
