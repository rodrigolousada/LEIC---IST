/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"

/* Inicializacao de Variaveis Globais */
Cliente_link top = NULL;

/* Funcoes Auxiliares */

void actualiza_clientes(int accao, long ref_emissor, long ref_beneficiario, int valor)
{
	/* Esta funcao e resonsavel por actualizar os dados dos clientes nos comandos cheque e processa. Ao nao encontrar a referencia do emissor e/ou benificiario a funcao ordena a sua criacao e inserecao na arvore AVL */
	Cliente_link emissor, beneficiario;
	
	if (accao == 1) {
		emissor = procura_apont_ref(top, ref_emissor);
		if (emissor == NULL) {
			top = insert(top, ref_emissor);
			emissor = procura_apont_ref(top, ref_emissor);
		}
		(emissor->nche)++;
		(emissor->vche) += valor;
		
		beneficiario = procura_apont_ref(top, ref_beneficiario);
		if (beneficiario == NULL) {
			top = insert(top, ref_beneficiario);
			beneficiario = procura_apont_ref(top, ref_beneficiario);
		}
		(beneficiario->nchb)++;
		(beneficiario->vchb) += valor;

	}
	
	else {
		emissor = procura_apont_ref(top, ref_emissor);
		(emissor->nche)--;
		(emissor->vche) -= valor;
		
		beneficiario = procura_apont_ref(top, ref_beneficiario);
		(beneficiario->nchb)--;
		(beneficiario->vchb) -= valor;
		}
}

int conta_act_clientes(Cliente_link h)
{
	/* Esta funcao e responsavel por contar o numero de cliente activos na arvore AVL */
	if (h==NULL) return 0;
	else if (h->nche + h->nchb != 0) return conta_act_clientes(h->r) + conta_act_clientes(h->l) + 1;
	else return conta_act_clientes(h->l) + conta_act_clientes(h->r);
}

/* Funcoes relacionadas com a Arvore AVL */

Cliente_link procura_apont_ref(Cliente_link h, long ref_wanted) 
{
	/* Esta funcao procura o cliente com a referencia desejada na arvore AVL e devolve um apontador para o mesmo. Caso a referencia nao exista a funcao devolve NULL */
	if (h==NULL) return NULL;
	if (ref_wanted == h->ref) return h;
	if (ref_wanted < h->ref) return procura_apont_ref(h->l, ref_wanted);
	else return procura_apont_ref(h->r, ref_wanted);
}

Cliente_link insert(Cliente_link h, long ref_wanted)
{
	/* Esta funcao insere um novo cliente com a referencia desejada na arvore AVL, que se ira organizar equilibrar novamente. Devolve sempre o head (ou neste caso top) da nova arvore gerada. */
	if (h == NULL) return novocliente(ref_wanted, NULL, NULL);
	if (ref_wanted < h->ref)
		h->l = insert (h->l, ref_wanted);
	else 
		h->r = insert (h->r, ref_wanted);
	h = AVLbalance(h);
	return h;
}

Cliente_link novocliente(long ref_wanted, Cliente_link l, Cliente_link r) 
{	
	/* Esta funcao cria e inicia um novo cliente retornando-o */
	Cliente_link novo = (Cliente_link) malloc(sizeof(Cliente));
	novo->ref = ref_wanted;
	novo->nche = novo->vche = novo->nchb = novo->vchb = 0;
	novo->l = l;
	novo->r = r;
	novo->height = 1;
	return novo;
}

int height(Cliente_link h)
{
	/* Esta funcao retorna o peso do no recebido */
	if (h == NULL) return 0;
	return h->height;
}

Cliente_link rotL(Cliente_link h)
{
	/* Esta funcao e responsavel pela operacao de rotacao a esquerda. Devolve a nova head da sub arvore recebida apos a rotacao*/
	int height_left, height_right;
	Cliente_link x = h->r;
	h->r = x->l;
	x->l = h;
	
	height_left = height(h->l);
	height_right = height(h->r);
	h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	height_left = height(x->l);
	height_right = height(x->r);
	x->height = height_left > height_right ? height_left + 1 : height_right + 1;
	return x;
}

Cliente_link rotR(Cliente_link h)
{
	/* Esta funcao e responsavel pela operacao de rotacao a direita. Devolve a nova head da sub arvore recebida apos a rotacao */
	int height_left, height_right;
	Cliente_link x = h->l;
	h->l = x->r;
	x->r = h;
	
	height_left = height(h->l);
	height_right = height(h->r);
	h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	height_left = height(x->l);
	height_right = height(x->r);
	x->height = height_left > height_right ? height_left + 1 : height_right + 1;
	return x;
}

Cliente_link rotLR(Cliente_link h)
{
	/* rotacao dupla esquerda direita */
	if (h==NULL) return h;
	h->l = rotL(h->l);
	return rotR(h);
}

Cliente_link rotRL(Cliente_link h)
{
	/* rotacao dupla direita esquerda */
	if (h==NULL) return h;
	h->r = rotR(h->r);
	return rotL(h);
}

int Balance(Cliente_link h)
{
	/* Esta funcao analisa o equilibrio da sub arvore apartir do no recebido */
	if(h==NULL) return 0;
	return height(h->l) - height(h->r);
}

Cliente_link AVLbalance(Cliente_link h)
{
	/* Esta funcao e responsavel por equilibrar a arvore AVL, devolvendo a head (ou neste caso top) da arvore */
	int balanceFactor;
	if (h==NULL) return h;
	
	balanceFactor = Balance(h);
	if (balanceFactor>1) {
		if (Balance(h->l) > 0) h=rotR(h);
		else h = rotLR(h);
	}
	else if (balanceFactor < -1) {
		if (Balance(h->r) < 0) h = rotL(h);
		else h = rotRL(h);
	}
	else {
		int height_left = height(h->l);
		int height_right = height(h->r);
		h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	}
	return h;
}

/* Funcoes Pedidas */
void infocliente()
{
	/* Esta funcao imprime a informacao referente ao cliente com a referencia escolhida. Caso o cliente nao exista nao imprime nada */
	long ref_wanted;
	Cliente_link i;
	
	scanf(" %ld", &ref_wanted);
	
	i = procura_apont_ref(top, ref_wanted);
	
	if(i!=NULL) printf("Cliente-info: %ld %d %d %d %d\n", i->ref, i->nche, i->vche, i->nchb, i->vchb);
}

void info()
{
	/* Esta funcao imprime a informacao sobre todos os clientes activos do sistema. Caso nao existam clientes activos imprime a frase No active clients */
	if (top==NULL) printf("No active clients\n");
	else {
		if (info_aux(top) == 0) printf("No active clients\n");
	}
}

			
int info_aux(Cliente_link h)
{
	/* Esta funcao gera o processo recursivo da funcao info */
	int aux, FLAG=0;
	
	if (h==NULL) return 0;
	aux = info_aux(h->l);
	if (h->nche + h->nchb != 0) {
		printf("*%ld %d %d %d %d\n", h->ref, h->nche, h->vche, h->nchb, h->vchb);
		FLAG = 1;
	}
	aux += info_aux(h->r) + FLAG;
	
	return aux;
}

void saiclientes()
{
	/* Esta funcao e responsavel pela parte respectiva aos clientes do comando sair imprimindo o numero de clientes activos no sistema. Chama ainda a funcao libertaclientes responsavel por libertar toda a memoria alocada para os clientes antes de sair do programa */
	int ncl;
	ncl = conta_act_clientes(top);
	printf("%d ", ncl);
	
	libertacliente(top);
}

void libertacliente(Cliente_link h) 
{	
	/* Esta funcao e responsavel por libertar toda a memoria alocada para os clientes antes de sair do programa */
	if (h!=NULL) {
		libertacliente(h->l);
		libertacliente(h->r);
		free(h);
	}
}
