# Po for Poisson distribution for more than one channel
# Investigacion Operativa

import math
import numpy


Resultado=numpy.zeros((40,6))

for serv in range(2,6): #number of services
    k=0 # index
    for psi in numpy.arange(1.1, 5, 0.1):
        sumatoria=0
        for n in range((serv-1)):
            sumatoria=sumatoria+((psi**n)/math.factorial(n))
        termino=(serv*(psi**serv))/((serv-psi)*math.factorial(serv))
        Resultado[k,0]=psi
        temp=1/(sumatoria+termino)
        if (temp>0):
            Resultado[k,(serv-1)]=temp
        else:
            Resultado[k,(serv-1)]=0
        k=k+1

