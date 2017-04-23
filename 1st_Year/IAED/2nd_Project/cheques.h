/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>

/* Definicao de estruturas */
typedef struct cheque {
	int valor;
	long refe, refb, refc;
	struct cheque *next;
} Cheque;

typedef Cheque* Cheque_link;

/* Prototipos */
void novocheque();
void processa();
void processaR();
void infocheque();
void saicheques();
