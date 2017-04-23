/*   2º Projecto IAED    */
/*       Grupo 43        */
/* 81115 Rodrigo Lousada */
/* 81538 Joao Pedrosa    */
/* 81525 Carlos Tejedor  */


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cheques.h"
#include "clientes.h"


/* Funcao MAIN */
int main() {
	
	char command[12]; /* 12 = 11+1 sendo 11 o maior numero de caracteres possiveis num comando valido */
	
	while (1) {
		scanf("%s", command);
		if (strcmp(command, "cheque") == 0) novocheque();
		else if (strcmp(command, "processa") == 0) processa();
		else if (strcmp(command, "processaR") == 0) processaR();
		else if (strcmp(command, "infocheque") == 0) infocheque();
		else if (strcmp(command, "infocliente") == 0) infocliente();
		else if (strcmp(command, "info") == 0) info();
		else if (strcmp(command, "sair") == 0) {
			saiclientes();
			saicheques();
			return EXIT_SUCCESS;
		}
		else printf("ERRO: Comando desconhecido\n");
	}
	return EXIT_FAILURE;
}
