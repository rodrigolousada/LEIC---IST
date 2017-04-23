import random
import interface

NLINHAS  = 4
NCOLUNAS = 4

#81115 Rodrigo Lousada
#81525 Carlos Antunes

#Segundo Projecto - Jogo 2048


#Tipos Abstratos de Dados (TAD)

def cria_coordenada(l, c):
    '''Este construtor recebe dois valores correspondentes a uma linha e uma coluna, inteiros compreendidos entre 1 e 4, e devolve a coordenada correspondente'''
    if isinstance(l, int) and isinstance(c, int) and 0<l<5 and 0<c<5:
        return (l, c)
    else:
        raise ValueError('cria_coordenada: argumentos invalidos')
    
def coordenada_linha(coordenada):
    '''Este selector recebe como argumento um elemento do tipo coordenada e devolve a linha respectiva.'''
    return coordenada[0]

def coordenada_coluna(coordenada):
    '''Este selector recebe como argumento um elemento do tipo coordenada e devolve a coluna respectiva.'''
    return coordenada[1]

def e_coordenada(universal):
    '''Este reconhecedor recebe um unico argumento e devolve True caso esse argumento seja do tipo coordenada, e False em caso contrario'''
    if isinstance(universal, tuple) and len(universal)==2:
        flag = True
        for e in universal:
            if not isinstance(e, int) or e<1 or e>4:
                flag = False
        return flag
    else:
        return False

def coordenadas_iguais(coordenada1, coordenada2):
    '''Este teste recebe dois elementos do tipo coordenada e devolve True caso esses argumentos correspondam a mesma posicao (l,c) do tabuleiro, e False em caso contrario.'''
    return coordenada1==coordenada2


#TAD tabuleiro

def cria_tabuleiro():
    '''Esta construtor nao recebe qualquer argumento, e devolve um elemento do tipo tabuleiro de acordo com a representacao interna escolhida. O tabuleiro vem vazio'''
    tabuleiro = {'pontuacao': 0}
    for l in range(1,5):
        for c in range(1,5):
            tabuleiro[(l, c)] = 0
    return tabuleiro


def tabuleiro_posicao(t, c):
    '''Este selector recebe como argumentos um elemento t do tipo tabuleiro e um elemento c do tipo coordenada e devolve um elemento do tipo inteiro, que corresponde ao valor na posicao do tabuleiro correspondente ao valor na posicao do tabuleiro correspondente a coordenada c.'''
    if e_coordenada(c):
        return t[(coordenada_linha(c),coordenada_coluna(c))]
    else:
        raise ValueError('tabuleiro_posicao: argumentos invalidos')
    
    
def tabuleiro_pontuacao(t):
    '''Este selector recebe como argumento um elemento t do tipo tabuleiro e devolve a pontuacao actual para o tabuleiro t.'''
    return t['pontuacao']

    
def tabuleiro_posicoes_vazias(t):
    '''Este selector recebe como argumentos um elemento t do tipo tabuleiro e devolve uma lista contendo as coordenadas de todas as posicoes vazias do tabuleiro t.'''
    lista = []
    for l in range(1,5):
        for c in range(1,5):
            if tabuleiro_posicao(t, cria_coordenada(l,c))==0:
                lista = lista + [cria_coordenada(l,c)]
    return lista
            
def tabuleiro_preenche_posicao(t, c, v):
    '''Este modificador recebe como argumentos um elemento t do tipo tabuleiro, um elemento c do tipo coordenada e um inteiro v, e modifica o tabuleiro t, colocando o valor v na posicao correspondente a coordenada c. A funcao devolve o tabuleiro modificado, mas caso c seja uma coordenada invalida ou v nao seja um inteiro a funcao gera um erro.'''
    if e_coordenada(c) and isinstance(v, int):
        t[(coordenada_linha(c),coordenada_coluna(c))] = v
        return t
    else:
        raise ValueError('tabuleiro_preenche_posicao: argumentos invalidos')
    
def tabuleiro_actualiza_pontuacao(t, v):
    '''Este modificador recebe como argumentos um elemento t do tipo tabuleiro e um inteiro v, nao negativo e multiplo de 4. Modifica o tabuleiro t, acrescentando ao valor da respectiva pontuacao v pontos. A funcao devolve o tabuleiro modificado, mas se v nao for um inteiro nao positivo ou multiplo de 4 ira gerar um erro.'''
    if isinstance(v, int) and v%4==0 and v>=0:
        t['pontuacao'] = tabuleiro_pontuacao(t) + v
        return t
    else:
        raise ValueError('tabuleiro_actualiza_pontuacao: argumentos invalidos')



#===============================================================================

tuplo_d = ('N', 'S', 'W', 'E')

def filtra(lista_aux):
    '''Funcao auxiliar que recebe uma lista e coloca todos os 0 no final'''
    lista_aux_2 = []
    lista_aux_3 = []
    for e in lista_aux:
        if e==0:
            lista_aux_2 = lista_aux_2 + [e]  
        else:
            lista_aux_3 = lista_aux_3 + [e]
    return lista_aux_3 + lista_aux_2
    
    
def junta(lista_aux, t):
    '''Funcao auxiliar que recebe uma lista e o tabuleiro e junta numeros iguais nessa lista, actualizando a pontuacao'''
    for i in range(len(lista_aux)-1):
        if lista_aux[i]==lista_aux[i+1]:
            lista_aux[i] = 2*(lista_aux[i])
            t['pontuacao'] = t['pontuacao'] + lista_aux[i]
            lista_aux[i+1] = 0
    return lista_aux



def tabuleiro_reduz(t, d):
    '''Este modificador recebe como argumento um elemento t do tipo tabuleiro e uma cadeia de caracteres d correspondente a uma das 4 accoes possiveis. Em particular d devera ser uma das cadeias de caracteres 'N', 'S', 'W', 'E'. A funcao modifica o tabuleiro t, reduzindo-o na direccao d de acordo com as regras do jogo 2048. A funcao devolve o tabuleiro t modificado, incluindo a actualizacao da pontuacao. Caso d seja uma jogada invalida gera um erro.'''
    if d == tuplo_d[0]:
        return tabuleiro_reduz_cima(t)
    elif d == tuplo_d[1]:
        return tabuleiro_reduz_baixo(t)
    elif d == tuplo_d[2]:
        return tabuleiro_reduz_esquerda(t)
    elif d == tuplo_d[3]:
        return tabuleiro_reduz_direita(t)
    else:
        raise ValueError('tabuleiro_reduz: argumentos invalidos')
    

def tabuleiro_reduz_cima(t):
    lista_aux = []
    for coluna in range(1,5):
        for linha_1 in range(1,5):
            lista_aux = lista_aux + [tabuleiro_posicao(t, cria_coordenada(linha_1, coluna))]
        lista_aux = filtra(lista_aux)
        lista_aux = junta(lista_aux, t)
        lista_aux = filtra(lista_aux)
        for linha_2 in range(1,5):
            tabuleiro_preenche_posicao(t, cria_coordenada(linha_2, coluna), lista_aux[linha_2 - 1])
        lista_aux = []
    return t

def tabuleiro_reduz_baixo(t):
    lista_aux = []
    for coluna in range(1,5):
        for linha_1 in range(4, 0, -1):
            lista_aux = lista_aux + [tabuleiro_posicao(t, cria_coordenada(linha_1, coluna))] 
        lista_aux = filtra(lista_aux)
        lista_aux = junta(lista_aux, t)
        lista_aux = filtra(lista_aux)
        for linha_2 in range(4, 0, -1):
            tabuleiro_preenche_posicao(t, cria_coordenada(linha_2, coluna), lista_aux[len(lista_aux) - linha_2])
        lista_aux = []
    return t

def tabuleiro_reduz_esquerda(t):
    lista_aux = []
    for linha in range(1,5):
        for coluna_1 in range(1,5):
            lista_aux = lista_aux + [tabuleiro_posicao(t, cria_coordenada(linha, coluna_1))]
        lista_aux = filtra(lista_aux)
        lista_aux = junta(lista_aux, t)
        lista_aux = filtra(lista_aux)
        for coluna_2 in range(1,5):
            tabuleiro_preenche_posicao(t, cria_coordenada(linha, coluna_2), lista_aux[coluna_2 - 1])
        lista_aux = []
    return t

def tabuleiro_reduz_direita(t):
    lista_aux = []
    for linha in range(1,5):
        for coluna_1 in range(4, 0, -1):
            lista_aux = lista_aux + [tabuleiro_posicao(t, cria_coordenada(linha, coluna_1))] 
        lista_aux = filtra(lista_aux)
        lista_aux = junta(lista_aux, t)
        lista_aux = filtra(lista_aux)
        for coluna_2 in range(4, 0, -1):
            tabuleiro_preenche_posicao(t, cria_coordenada(linha, coluna_2), lista_aux[len(lista_aux) - coluna_2])
        lista_aux = []
    return t

#===============================================================================

def e_tabuleiro(possivel_t):
    '''Este reconhecedor recebe um unico argumento, devolvendo True se o seu argumento for do tipo tabuleiro, e False em caso contrario. Nao necessitando verificar se cada elemento e uma potencia de 2.''' 
    if isinstance(possivel_t, dict) and len(possivel_t)==17 and ('pontuacao' in possivel_t):
        for l in range(1,5):
            for c in range(1,5):
                if (l,c) not in possivel_t:
                    return False
        return True
    else:
        return False
    
def tabuleiro_terminado(t):
    '''Este reconhecedor recebe como argumentos um elemento t do tipo tabuleiro e devolve True caso o tabuleiro t esteja terminado, ou seja, caso esteja cheio e nao existam movimentos possiveis, e False em caso contrario.'''
    t_aux = copia_tabuleiro(t)
    t_aux_cima = tabuleiro_reduz(t_aux, 'N')
    t_aux_baixo = tabuleiro_reduz(t_aux, 'S')
    t_aux_direita = tabuleiro_reduz(t_aux, 'E')
    t_aux_esquerda = tabuleiro_reduz(t_aux, 'W')
    if len(tabuleiro_posicoes_vazias(t))==0 and t_aux_cima==t_aux_baixo==t_aux_direita==t_aux_esquerda==t:
        return True
    else:
        return False
    
def tabuleiros_iguais(t1, t2):
    '''Este teste recebe como argumentos dois elementos t1 e t2 do tipo tabuleiro e devolve True caso t1 e t2 correspondam a dois tabuleiro com a mesma configuracao e pontuacao, e False em caso contrario.'''
    return t1==t2
    
def escreve_tabuleiro(t):
    '''A funcao escreve_tabuleiro recebe como argumento um elemento t do tipo tabuleiro e escreve para o ecra a representacao externa de um tabuleiro 2048. Verifica se t e um tabuleiro valido e, caso o argumento introduzido seja invalido, gera um erro'''
    if e_tabuleiro(t):
        for linha in range(1,5):
            print('[', tabuleiro_posicao(t, cria_coordenada(linha, 1)), ']', '[', tabuleiro_posicao(t, cria_coordenada(linha, 2)), ']', '[', tabuleiro_posicao(t, cria_coordenada(linha, 3)), ']', '[', tabuleiro_posicao(t, cria_coordenada(linha, 4)), '] ')
        print('Pontuacao:', tabuleiro_pontuacao(t))
    else:
        raise ValueError('escreve_tabuleiro: argumentos invalidos')
    

#Funcoes Adicionais

def pede_jogada():
    '''Esta funcao nao recebe qualquer argumento, limitando-se a pedir ao utilizador para introduzir uma direccao (N, S, E ou W). Caso o valor introduzido pelo utilizador seja invalido, a funcao deve pedir novamente a informacao de jogada ao utilizador. A funcao devolve uma cadeia de caracteres correspondente a direccao escolhida pelo utilizador.'''
    jogada = str(input('Introduza uma jogada (N, S, E, W): '))
    if jogada in tuplo_d:
        return jogada
    else:
        print('Jogada invalida.')
        return pede_jogada()
    
    
def copia_tabuleiro(t):
    '''Esta funcao recebe como argumento um elemento do tipo tabuleiro e devolve uma copia do mesmo.'''
    t1 = cria_tabuleiro()
    for l in range(1,5):
        for c in range(1,5):
            tabuleiro_preenche_posicao(t1, cria_coordenada(l,c), tabuleiro_posicao(t, cria_coordenada(l,c)))
    t1['pontuacao'] = tabuleiro_pontuacao(t)
    return t1

from random import *

def n_escolhido():
    '''Funcao que gera um 2 ou um 4 de acordo com as probilidades 0.8 e 0.2, respectivamente'''
    aux = int(random()*10)
    if aux<2:
        return 4
    else:
        return 2

def preenche_posicao_aleatoria(c1):
    '''Esta funcao recebe um elemento do tipo tabuleiro e preenche um posicao livre, escolhida aleatoriamente, com um dos numero 2 ou 4, de acordo com as probabilidades 0.8 e 0.2, respectivamente.'''
    posicoes_possiveis = tabuleiro_posicoes_vazias(c1)
    if len(posicoes_possiveis)>0:
        return tabuleiro_preenche_posicao(c1, posicoes_possiveis[int(random()*(len(posicoes_possiveis) - 1))], n_escolhido())

def desenha_tabuleiro(w, t):
    ''' desenha_tabuleiro : janela x tabuleiro -> {}
        desenha_tabuleiro(t) desenha na janela de jogo o tabuleiro de 2048 t.'''
    
    if not e_tabuleiro(t):
        raise ValueError ('escreve_tabuleiro: argumentos invalidos')
    
    for l in range(1, NLINHAS+1):
        for c in range(1, NCOLUNAS+1):
            w.draw_tile(tabuleiro_posicao(t, cria_coordenada(l, c)), l, c)
        
def joga_2048():
    '''...'''

    w = interface.window_2048()

    tabuleiro = cria_tabuleiro()
    preenche_posicao_aleatoria(tabuleiro)
    preenche_posicao_aleatoria(tabuleiro)
    escreve_tabuleiro(tabuleiro)
    tabuleiro_anterior = copia_tabuleiro(tabuleiro)
    
    quit = False
    
    while not quit:
        desenha_tabuleiro(w, t)
        jogada = w.get_play()
        if jogada == 'Q':
            quit = True

        while not tabuleiro_terminado(tabuleiro):
            tabuleiro_reduz(tabuleiro, pede_jogada())
            if tabuleiros_iguais(tabuleiro_anterior, tabuleiro):
                tabuleiro_anterior = copia_tabuleiro(tabuleiro)
                escreve_tabuleiro(tabuleiro)
            else:
                preenche_posicao_aleatoria(tabuleiro)
                tabuleiro_anterior = copia_tabuleiro(tabuleiro)
                escreve_tabuleiro(tabuleiro)

        w.step()

    print('Jogo terminado.')
    
if __name__ == "__main__":
    joga_2048()
