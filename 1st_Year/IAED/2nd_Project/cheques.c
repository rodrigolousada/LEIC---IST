/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include "cheques.h"
#include "clientes.h"

/* Inicializacao de Variaveis Globais */
Cheque_link head = NULL, tail = NULL;

/* Funcoes Pedidas */
void novocheque()
{
	/* Esta funcao adiciona um novo cheque a pool de cheques a processar */
	Cheque_link novo = (Cheque_link) malloc(sizeof(Cheque));
	
	scanf(" %d %ld %ld %ld\n", &novo->valor, &novo->refe, &novo->refb, &novo->refc);
	
	actualiza_clientes(1, novo->refe, novo->refb, novo->valor);
	
	if (head==NULL) head = novo;
	else tail->next = novo;
	novo->next = NULL;
	tail = novo;
}


void processa()
{
	/* Esta funcao processa o cheque emitido ha mais tempo, e remove-o da pool de cheques a processar. Nao imprime qualquer output excepto se a pool de cheques for vazia, imprimindo a informacao Nothing to process. */
	Cheque_link aux;
	
	if (head==NULL) printf("Nothing to process\n");
	else {
		aux = head;
		head = head->next;
		
		actualiza_clientes(0, aux->refe, aux->refb, aux->valor);
		
		free(aux);
	}
}


void processaR()
{
	/* Esta funcao processa o cheque com a referencia dada, e remove-o da pool de cheques a processar. Este processo e feito colocando o cheque escolhido no inicio da lista FIFO e correndo a funcao processa. Nao imprime qualquer output excepto se nao exitir nenhum cheque com a referida referencia, imprimindo a informacao Cheque ... does not exist */
	long ref_wanted;
	Cheque_link i, ant = head;
	
	scanf(" %ld", &ref_wanted);
	
	for(i = head; i!=NULL && i->refc!=ref_wanted; ant = i, i = i->next);
	
	if(i==NULL) printf("Cheque %ld does not exist\n", ref_wanted);
	else if (i==head) processa();
	else {
		if(i==tail) tail=ant;
		ant->next = i->next;
		i->next = head;
		head = i;
		processa();
	}
}


void infocheque()
{
	/* Esta funcao imprime a informacao referente ao cheque com a referencia escolhida. Caso o cheque nao exista nao imprime nada */
	long ref_wanted;
	Cheque_link i;
	
	scanf(" %ld", &ref_wanted);
	
	for(i = head; i!=NULL && i->refc!=ref_wanted; i = i->next);
	
	if(i!=NULL) printf("Cheque-info: %ld %d %ld --> %ld\n", i->refc, i->valor, i->refe, i->refb);
}


void saicheques()
{
	/* Esta funcao e responsavel pela parte respectiva aos cheques do comando sair imprimindo o numero e o valor total de cheque por processar. Tambem e responsavel por libertar toda a memoria alocada para os cheque antes de sair do programa */
	int nch=0, vch=0;
	Cheque_link i, ant;
	
	for(i = head; i!=NULL; ant = i, i = i->next) {
		if (i!=head) free(ant);
		(nch)++;
		vch += i->valor;
	}
	if (i!=head) free(ant);
	printf("%d %d\n", nch, vch);
}
