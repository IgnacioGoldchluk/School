def  MfbComp(Q, Wo, R2):
    # Funcion que devuelve los componentes de un filtro pasabajos de 2do orden
    # con topología MFB, R2 es un valor de entrada adoptado por diseño y el
    # resto se calcula en base a los factores de ruido

    #Impongo Av, despues se puede ajustar con un bufer
    Av = 1-(1/Q)**2;

    #Criterio de diseño, R3 <= R2
    R3 = R2;

    #Calculo de C2, el hecho de R3 = R2 impone el valor de C2
    C2 = 1/(Wo*R2*Q*(2*(-Av)+1));

    #Calculo de C1
    C1 = Q/(Wo*R2*(1-Q*Wo*R2*C2*(1+Av)));

    #Calculo de R1
    R1 = R3/(-Av);

    #Redondear a 3 digitos y guardar como string
    R1 = str(round(R1,3))
    R3 = str(round(R3,3))
    Av = str(round(Av,3))
    C1 = '%s' % float('%.2g' % C1) #Muy chicos para funcion round
    C2 = '%s' % float('%.2g' % C2)
    

    
    return R1, R3, C1, C2, Av



# Programa para devolver los componentes optimos para filtro MFB para tesis de trabajo final
# Valor comercial = R2, los otros quedan a criterio del usuario

R_standard = [1, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2, 2.2, 2.4, 2.7, 3, 3.3, 3.6,\
              3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1]

Header = ['R1', 'R2', 'R3', 'C1', 'C2', 'Av']
Componentes = []
componentfile = open('components.txt', '+w')
separador = '   '
componentfile.write(separador.join(Header) + '\n')
componentfile.close()

length = len(R_standard)

print('Ingrese Wo')
Wo = input()
Wo = float(Wo)
print('Ingrese Q')
Q = input()
Q = float(Q)

for i in range(2,5):
    for j in range(0,length-1):
        R2 = R_standard[j]*(10**i)
        compList = MfbComp(Q, Wo, R2)
        componentfile = open('components.txt', 'a')
        componentfile.write(separador.join((compList))+'\n')
        componentfile.close()
