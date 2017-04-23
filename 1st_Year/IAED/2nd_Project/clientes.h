/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>

/* Definicao de estruturas */
typedef struct cliente{
	long ref;
	int nche, vche, nchb, vchb, height;
	struct cliente *l, *r;
} Cliente;

typedef Cliente* Cliente_link;

/* Prototipos */
void actualiza_clientes(int accao, long ref_emissor, long ref_beneficiario, int valor);
Cliente_link procura_apont_ref(Cliente_link h, long ref_wanted);

Cliente_link insert(Cliente_link h, long ref_wanted);
Cliente_link novocliente(long ref_wanted, Cliente_link l, Cliente_link r);
int height(Cliente_link h);
Cliente_link rotL(Cliente_link h);
Cliente_link rotR(Cliente_link h);
Cliente_link rotLR(Cliente_link h);
Cliente_link rotRL(Cliente_link h);
int Balance(Cliente_link h);
Cliente_link AVLbalance(Cliente_link h);


void infocliente();
void info();
int info_aux(Cliente_link h);
void saiclientes();
void libertacliente(Cliente_link h);
