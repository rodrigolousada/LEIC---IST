#81115 Rodrigo Lousada

#Algoritmo de Luhn

def last_digit(cc):
    '''Recebe uma cadeia de caracteres que representa um numero e devolve o ultimo digito'''
    last = eval(cc)%10
    return last

def remove(cc):
    '''Primeiro passo do Algoritmo de Luhn: Recebe como argumento uma cadeia de caracteres que representa um numero e devolve uma cadeia de caracteres que representa o mesmo numero sem o ultimo digito'''
    cc = eval(cc)//10
    return str(cc)

def inverte(cc):
    '''Segundo passo do Algoritmo de Luhn: Recebe como argumento uma cadeia de caracteres e devolve a mesma com a ordem dos seus caracteres invertida'''
    cc = cc[::-1]
    return cc

def tupler(cc):
    '''Recebe como argumento uma cadeia de caracteres e devolve um tuplo com cada um dos caracteres da cadeia como um elemento desse tuplo'''
    tuplo_1=()
    for caract in cc:
        tuplo_1 = tuplo_1 + (int(caract),)
    return tuplo_1


def duplicar_impares(cc):
    '''Terceiro passo do Algoritmo de Luhn (1/2): Recebe como argumento um tuplo e multiplica por 2 os digitos em posicoes impares'''
    num_indices = len(cc)
    tuplo_2=()
    for i in range(num_indices):
        if i%2==0:                    #temos de considerar as posicoes impares pares, pois costumamos comecar a contar a primeira posicao como a posicao 1 (numero impar) e o tuplo comeca a contar como posicao 0 (numero par)
            tuplo_2 = tuplo_2 + ((cc[i] * 2),)
        else:
            tuplo_2 = tuplo_2 + (cc[i],)
    return tuplo_2

def subtrair_impares(cc):
    '''Terceiro passo do Algoritmo de Luhn (2/2): Recebe como argumento um tuplo e subtrai 9 aos digitos em posicoes impares que sejam superiores a 9'''
    num_indices = len(cc)
    tuplo_3=()
    for i in range(num_indices):
        if i%2==0 and cc[i]>9:        # mais uma vez temos de considerar como se fossem as posicoes pares
            tuplo_3 = tuplo_3 + (cc[i]-9,)
        else:
            tuplo_3 = tuplo_3 + (cc[i],)
    return tuplo_3

     
def calc_soma(cc):
    '''Recebe como argumento uma cadeia de caracteres, representando um numero de cartao de credito sem o ultimo digito, e devolve a soma ponderada dos digitos de cc, calculada de acordo com o algoritmo de Luhn'''
    cc = inverte(cc)
    cc_tuplo = tupler(cc)
    cc_tuplo = duplicar_impares(cc_tuplo)
    cc_tuplo = subtrair_impares(cc_tuplo)
    result = 0
    for e in cc_tuplo:
        result = e + result
    return result

def luhn_verifica(cc):
    '''Recebe como argumento uma cadeia de caracteres, representando um numero de cartao de credito, e devolve True, se o numero passar o algoritmo de Luhn'''
    last = last_digit(cc)
    cc = remove(cc)
    cc = calc_soma(cc) + last         #antes de verificar se e divisivel por 10, somamos ainda o digito que retiramos ao inicio
    flag = False
    if int(cc) % 10 == 0:
        flag = True
    return flag




# Prefixo do Numero
    
def comeca_por(cad1, cad2):
    '''Recebe como argumentos duas cadeias de caracteres e devolve True apenas se a primeira comecar pela segunda'''
    flag = True
    cad1 = tupler(cad1)
    cad2 = tupler(cad2)
    if len(cad1)>=len(cad2):
        num_elem = len(cad2)
        for f in range(num_elem):
            if cad1[f]!=cad2[f]:
                flag = False
    else:
        flag = False
    return flag

def comeca_por_um(cad, t_cads):
    '''Recebe como argumentos uma cadeia de caracteres e um tuplo de cadeias de caracteres, e devolve True apenas se a cadeia de caracteres comecar por um dos elementos do tuplo'''
    num_elementos = len(t_cads)
    flag_1 = False
    for g in range(num_elementos):
        if comeca_por(cad, t_cads[g]) == True:
            flag_1 = True
    return flag_1




# Rede Emissora

tuplo_tabela = (('American Express', 'AE', ('34', '37'), ('15',)), ('Diners Club International', 'DCI', ('309', '36', '38', '39'), ('14',)), ('Discover Card', 'DC', ('65',), ('16',)), ('Maestro', 'M', ('5018', '5020', '5038'), ('13', '19')), ('Master Card', 'MC', ('50', '51', '52', '53', '54', '19'), ('16',)), ('Visa Electron', 'VE', ('4026', '426', '4405', '4508'), ('16',)), ('Visa', 'V', ('4024', '4532', '4556'), ('13', '16')))
#este tuplo representa todas as informacoes da tabela 1

def valida_iin(cc):
    '''Recebe como argumento uma cadeia de caracteres, respresentando um numero de cartao de credito, e devolve uma cadeia correspondente a rede emissora do cartao'''
    if comeca_por_um(cc, tuplo_tabela[0][2]) == True and len(tupler(cc))==15:
        return tuplo_tabela[0][0]
    elif comeca_por_um(cc, tuplo_tabela[1][2]) == True and len(tupler(cc))==14:
        return tuplo_tabela[1][0]
    elif comeca_por_um(cc, tuplo_tabela[2][2]) == True and len(tupler(cc))==16:
        return tuplo_tabela[2][0]
    elif comeca_por_um(cc, tuplo_tabela[3][2]) == True and (len(tupler(cc))==13 or len(tupler(cc))==19):
        return tuplo_tabela[3][0]
    elif comeca_por_um(cc, tuplo_tabela[4][2]) == True and len(tupler(cc))==16:
        return tuplo_tabela[4][0]
    elif comeca_por_um(cc, tuplo_tabela[5][2]) == True and len(tupler(cc))==16:
        return tuplo_tabela[5][0]
    elif comeca_por_um(cc, tuplo_tabela[6][2]) == True and (len(tupler(cc))==13 or len(tupler(cc))==16):
        return tuplo_tabela[6][0]
    else:
        return ''
    



# MII

def categoria(cc):
    '''Recebe como argumento uma cadeia de caracteres, respresentando um numero de cartao de credito, e devolve uma cadeia correspondente a categoria da entidade correspondente ao primeiro caracter da cadeia'''
    cc_tuplo = tupler(cc)
    first_digit = cc_tuplo[0]
    if first_digit == 1:
        return 'Companhias aereas'
    elif first_digit == 2:
        return 'Companhias aereas e outras tarefas futuras da industria'
    elif first_digit == 3:
        return 'Viagens e entretenimento e bancario / financeiro'
    elif first_digit == 4 or first_digit == 5:
        return 'Servicos bancarios e financeiros'
    elif first_digit == 6:
        return 'Merchandising e bancario / financeiro'
    elif first_digit == 7:
        return 'Petroleo e outras atribuicoes futuras da industria'
    elif first_digit == 8:
        return 'Saude, telecomunicacoes e outras atribuicoes futuras da industria'
    elif first_digit == 9:
        return 'Atribuicao nacional'
    else:
        return 'Por favor, confirme se o primeiro digito e mesmo 0'
    
# Vamos entao criar a funcao verifica_cc

def verifica_cc(cc):
    '''Recebe como argumento um inteiro correspondente a um possivel numero de um cartao de credito e devolve, se este for valido, um tuplo contendo a categoria e a rede do cartao, ou, se este for invalido, a cadeia de caracteres 'cartao invalido' '''
    cc = str(cc)
    if luhn_verifica(cc) == True and valida_iin(cc)!='':
        tuplo_4 = (categoria(cc), valida_iin(cc))
        return tuplo_4
    else:
        return 'cartao invalido'



#Geracao do numero de um cartao de credito

from random import random


def possiveis_prefixos(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis prefixos para gerar um numero emitido por essa entidade'''
    num_indices = len(tuplo_tabela)
    for i in range(num_indices):
        if abreviatura == tuplo_tabela [i] [1]:
            return tuplo_tabela [i] [2]

    
def prefixo(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve aleatoriamente um dos possiveis prefixos para gerar um numero emitido por essa entidade'''
    tuplo_prefixos = possiveis_prefixos(abreviatura)
    indice_max = len(tuplo_prefixos)
    indice_escolhido = int(indice_max * random())
    prefixo = tuplo_prefixos[indice_escolhido]
    return prefixo



def possiveis_comprimentos(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis comprimentos para gerar um numero emitido por essa entidade'''
    num_indices = len(tuplo_tabela)
    for i in range(num_indices):
        if abreviatura == tuplo_tabela [i] [1]:
            return tuplo_tabela [i] [3]
    

def comprimento(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve aleatoriamente um dos possiveis comprimentos para gerar um numero emitido por essa entidade'''
    tuplo_comprimentos = possiveis_comprimentos(abreviatura)
    indice_max = len(tuplo_comprimentos)
    indice_escolhido = int(indice_max * random())
    comprimento = tuplo_comprimentos[indice_escolhido]
    return int(comprimento)
    

def gerador(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e gera um numero aleatorio de cartao de credito sem o ultimo digito emitido por essa entidade'''
    prefixo_cc = prefixo(abreviatura)
    comprimento_cc = comprimento(abreviatura) - len(tupler(prefixo_cc)) -1
    result = int(prefixo_cc)
    while comprimento_cc>0:
        result = result*10 + int(random()*10)
        comprimento_cc = comprimento_cc -1
    return result
        

def digito_verificacao(cc):
    '''Recebe como argumento uma cadeia de caracteres, representando um numero de cartao de credito, sem o ultimo digito, e devolve o ultimo digito, de verificacao, que lhe devera ser acrescentado, de forma a obter um numero de cartao valido'''
    soma = calc_soma(cc)
    resto = soma % 10
    if resto == 0:
        complemento = 0
    else:
        complemento = 10 - resto
    return str(complemento)


def gera_num_cc(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e gera um numero de cartao de credito valido emitido por essa entidade'''
    gerado = gerador(abreviatura)
    last_digit = digito_verificacao(str(gerado))
    final = gerado*10 + eval(last_digit)
    return final
    