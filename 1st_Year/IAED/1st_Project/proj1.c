/*      Projecto 1       */
/*       Grupo 43        */
/* 81115 Rodrigo Lousada */
/* 81538 Joao Pedrosa    */
/* 81525 Carlos Tejedor  */


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>


/* Constantes */
#define BOM 1
#define MAU 0
#define MAX_REG_BANCOS 1000
#define MAX_NOME 41

/* Definicao do tipo banco */
typedef struct {
	char nome[MAX_NOME];
	int classificacao;
	int ref;
	int inP;
	int outP;
	int outV;
	int outVM;
	int inV;
	int inVM;
	int parceiros;
} bank;

/* Prototipos */
void add_new_bank();
void rating(int class, int referencia);
void add_emprestimo();
void amort_emprestimo();
void emite_listagem();
void despromover();

int procura_indice(int referencia);
int count_bancos_bons();

/* Inicializacao de Variaveis Globais */
int rede[MAX_REG_BANCOS][MAX_REG_BANCOS];
bank bancos[MAX_REG_BANCOS];
int contador_bancos = 0;

/* Funcao MAIN */
int main() {
	
	char command;
	int referencia;
	
	while (1) {
		command = getchar();
		switch (command) {
			case 'a':
				add_new_bank();
				break;
			case 'k':
				scanf(" %d", &referencia);
				rating(MAU, referencia);
				break;
			case 'r':
				scanf(" %d", &referencia);
				rating(BOM, referencia);
				break;
			case 'e':
				add_emprestimo();
				break;
			case 'p':
				amort_emprestimo();
				break;
			case 'l':
				emite_listagem();
				break;
			case 'K':
				despromover();
				break;
			case 'x':
				printf("%d %d\n", contador_bancos, count_bancos_bons());
				return EXIT_SUCCESS;
			default:
				printf("ERRO: Comando desconhecido\n");
		}
		getchar();
	}
	return EXIT_FAILURE;
}

/* Funcoes Auxiliares */
int procura_indice(int referencia) 
{
	int i;
	
	for (i = 0; i < contador_bancos; i++) {
		if (referencia == bancos[i].ref)
			return i;
	}
	return EXIT_FAILURE;
}

int count_bancos_bons()
{
	int bons = 0, i;
	
	for (i = 0; i < contador_bancos; i++) {
		if (bancos[i].classificacao == BOM)
			bons++;
	}
	return bons;
}


/* Funcoes Pedidas */

void add_new_bank() 
{	
	/* Esta funcao adiciona um novo banco ao sistema ligando-o na rede aos outros ja existentes, sendo associada ao comando a */
	int i, j;
	
	for (i = 0; i <= contador_bancos; i++)
		rede[i][contador_bancos] = 0;
	for (j = 0; j < contador_bancos; j++)
		rede[contador_bancos][j] = 0;
	
	scanf(" %s %d %d", bancos[contador_bancos].nome, &bancos[contador_bancos].classificacao, &bancos[contador_bancos].ref);
	
	contador_bancos++;
}

void rating(int class, int referencia)
{
	/* Esta funcao classifica o banco da referencia dada como bom ou mau dependendo da variavel class que recebe, sendo associada aos comandos k, r, K  */
	
	int indice, i;
	
	indice = procura_indice(referencia);
	
	if (class == MAU) {
		bancos[indice].classificacao = MAU;
		for (i = 0; i < contador_bancos; i++) {
			if (rede[i][indice] != 0)
				bancos[i].outVM = bancos[i].outVM + rede[i][indice];
			if (rede[indice][i] != 0)
				bancos[i].inVM = bancos[i].inVM + rede[indice][i];
		}
	}
	else {
		bancos[indice].classificacao = BOM;
		for (i = 0; i < contador_bancos; i++) {
			if (rede[i][indice] != 0)
				bancos[i].outVM = bancos[i].outVM - rede[i][indice];
			if (rede[indice][i] != 0)
				bancos[i].inVM = bancos[i].inVM - rede[indice][i];
		}
	}
}

void add_emprestimo()
{
	/* Esta funcao adiciona um emprestimo, sendo associada ao comando e */
	int ref1, ref2, valor, i1, i2;
	
	scanf(" %d %d %d", &ref1, &ref2, &valor);
	i1 = procura_indice(ref1);
	i2 = procura_indice(ref2);
	
	if (rede[i1][i2]==0){
		bancos[i2].inP++;
		bancos[i1].outP++;
	}
	
	bancos[i2].inV = bancos[i2].inV + valor;
	bancos[i1].outV = bancos[i1].outV + valor;
	
	if (bancos[i2].classificacao == MAU)
		bancos[i1].outVM = bancos[i1].outVM + valor;
	if (bancos[i1].classificacao == MAU)
		bancos[i2].inVM = bancos[i2].inVM + valor;
		
	rede[i1][i2] = rede[i1][i2] + valor;
}

void amort_emprestimo()
{
	/* Esta funcao amortiza um emprestimo reduzindo a divida por parte dum banco, sendo associada ao comando p */
	int ref1, ref2, valor, i1, i2;
	
	scanf(" %d %d %d", &ref1, &ref2, &valor);
	i1 = procura_indice(ref1);
	i2 = procura_indice(ref2);
	if (valor > 0 && valor <= rede[i2][i1])
		
		bancos[i2].outV = bancos[i2].outV - valor;
		bancos[i1].inV = bancos[i1].inV - valor;
		
		if (bancos[i2].classificacao == MAU)
			bancos[i1].inVM = bancos[i1].inVM - valor;
		if (bancos[i1].classificacao == MAU)
			bancos[i2].outVM = bancos[i2].outVM - valor;

		rede[i2][i1] = rede[i2][i1] - valor;
		
		if (rede[i2][i1] == 0 && rede[i1][i2] == 0){
			bancos[i2].outP--;
			bancos[i1].inP--;
		}
}

void emite_listagem()
{
	/* Esta funcao emite diferentes listagens de acordo com os tres tipos (1,2,3) definidos no enunciado do projecto, sendo associada ao comando l */
	int identif, i, j, rep, n_partn, contador = 0;
	
	scanf(" %d", &identif);
	if (identif == 0) {
		for (i = 0; i < contador_bancos; i++)
			printf("%d %s %d\n", bancos[i].ref, bancos[i].nome, bancos[i].classificacao);
	}
	else if (identif == 1) {
		for (i = 0; i < contador_bancos; i++)
			printf("%d %s %d %d %d %d %d %d %d\n", bancos[i].ref, bancos[i].nome, bancos[i].classificacao, bancos[i].inP, bancos[i].outP, bancos[i].outV, bancos[i].outVM, bancos[i].inV, bancos[i].inVM);
	}
	else {
		for (i = 0; i < contador_bancos; i++) {
			for (j = 0; j < contador_bancos; j++)
				if (rede[i][j] != 0 && rede[j][i] != 0)
					rep++;
			bancos[i].parceiros = bancos[i].inP + bancos[i].outP - rep;
			rep = 0;
		}
		
		for (n_partn = 0; n_partn < contador_bancos; n_partn++) {
			for (i = 0; i < contador_bancos; i++)
				if (bancos[i].parceiros == n_partn)
					contador++;
			if (contador != 0)
				printf("%d %d\n", n_partn, contador);
			contador = 0;
		}
	}
}

void despromover()
{
	/* Esta funcao despromove o banco bom em maior dificuldades, utilizando a funcao rating para o classificar como mau, e sendo associada ao comando K. */
	int i, pior_investidor = -1;
	
	
	for (i = 0; i < contador_bancos; i++) {
		if (bancos[i].classificacao == BOM)
			if (pior_investidor == -1 || bancos[i].outVM >= bancos[pior_investidor].outVM)
				pior_investidor = i;
	}
	
	if (count_bancos_bons() == 0 || bancos[pior_investidor].outVM == 0)
		printf("%d %d\n", contador_bancos, count_bancos_bons());
	else {
		rating(MAU, bancos[pior_investidor].ref);
		printf("*%d %s %d %d %d %d %d %d %d\n", bancos[pior_investidor].ref, bancos[pior_investidor].nome, bancos[pior_investidor].classificacao, bancos[pior_investidor].inP, bancos[pior_investidor].outP, bancos[pior_investidor].outV, bancos[pior_investidor].outVM, bancos[pior_investidor].inV, bancos[pior_investidor].inVM);
		printf("%d %d\n", contador_bancos, count_bancos_bons());
	}
}
