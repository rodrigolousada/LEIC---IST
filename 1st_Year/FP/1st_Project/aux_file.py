tuplo_tabela = (('American Express', 'AE', ('34', '37'), ('15',)), ('Diners Club International', 'DCI', ('309', '36', '38', '39'), ('14',)), ('Discover Card', 'DC', ('65',), ('16',)), ('Maestro', 'M', ('5018', '5020', '5038'), ('13', '19')), ('Master Card', 'MC', ('50', '51', '52', '53', '54', '19'), ('16',)), ('Visa Electron', 'VE', ('4026', '426', '4405', '4508'), ('16',)), ('Visa', 'V', ('4024', '4532', '4556'), ('13', '16')))
#este tuplo representa todas as informacoes da tabela 1

def possiveis_prefixos(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis prefixos para gerar um numero emitido por essa entidade'''
    num_indices = len(tuplo_tabela)
    for i in range(num_indices):
        if abreviatura == tuplo_tabela [i] [1]:
            return tuplo_tabela [i] [2]
    
def possiveis_pref(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis prefixos para gerar um numero emitido por essa entidade'''
    if abreviatura == tuplo_tabela [0] [1]:
        return tuplo_tabela [0] [2]
    elif abreviatura == tuplo_tabela [1] [1]:
        return tuplo_tabela [1] [2]
    elif abreviatura == tuplo_tabela [2] [1]:
        return tuplo_tabela [2] [2]
    elif abreviatura == tuplo_tabela [3] [1]:
        return tuplo_tabela [3] [2]
    elif abreviatura == tuplo_tabela [4] [1]:
        return tuplo_tabela [4] [2]
    elif abreviatura == tuplo_tabela [5] [1]:
        return tuplo_tabela [5] [2]    
    elif abreviatura == tuplo_tabela [6] [1]:
        return tuplo_tabela [6] [2]
    
    
def possiveis_comprime(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis comprimentos para gerar um numero emitido por essa entidade'''
    if abreviatura == tuplo_tabela [0] [1]:
        return tuplo_tabela [0] [3]
    elif abreviatura == tuplo_tabela [1] [1]:
        return tuplo_tabela [1] [3]
    elif abreviatura == tuplo_tabela [2] [1]:
        return tuplo_tabela [2] [3]
    elif abreviatura == tuplo_tabela [3] [1]:
        return tuplo_tabela [3] [3]
    elif abreviatura == tuplo_tabela [4] [1]:
        return tuplo_tabela [4] [3]
    elif abreviatura == tuplo_tabela [5] [1]:
        return tuplo_tabela [5] [3]    
    elif abreviatura == tuplo_tabela [6] [1]:
        return tuplo_tabela [6] [3]
    
def possiveis_comprimentos(abreviatura):
    '''Recebe como argumento uma cadeia de caracteres correspondente a abreviatura de uma rede emissora e devolve um tuplo de cadeias de caracteres composto pelos possiveis comprimentos para gerar um numero emitido por essa entidade'''
    num_indices = len(tuplo_tabela)
    for i in range(num_indices):
        if abreviatura == tuplo_tabela [i] [1]:
            return tuplo_tabela [i] [3]