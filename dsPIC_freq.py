# Optimal values for FSYS = 32MHz in dsPIC33FJ128GP802%%
# FSYS = FOSC/2
# FOSC = FRC*M/(N1*N2)


MIN = 1000; # Initial MIN value, set as high as possible
FOSC = 64
FRC = 7.37

for N1 in range(2,9): # Stops at 9 because FRC/N1 > 0.8MHz
    for M in range(13*N1,27*N1):# Valid range for the VCO freq
        for N2 in range(2,8,2): # It's actually powers of 2 but this is easier to code
            FOBTENIDA = FRC*M/(N1*N2)
            DIF = abs(FOSC - FOBTENIDA)
            if ((DIF<MIN) and (N2!=6) and (FOBTENIDA <= FOSC)): # Check if valid
                MIN = DIF
                N1_BEST = N1
                N2_BEST = N2
                M_BEST  =  M
                FBEST = FRC*M/(N1*N2)

print('N1 = ' + str(N1_BEST) + '\nN2 = ' + str(N2_BEST) + '\nM = ' + str(M_BEST))
