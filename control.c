/*
 *  Title: control.c
 *	Version:
 *  Description:
 *  Created:  16/07/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 *
 *	En este archivo se implementa la función PLL_SdC_function() en la cual debe ser incluida la implementación del PLL.
 *	Entradas:
 * 		Input10V: 0-10V
 *	Salidas:
 *		*UserLED / *UserGPIO: 0(0V):OFF-1(3.3V):ON
 *		Output3_3V: 0-3.3V
 */
#include <math.h>
int Output_Selector=0;

float PLL_SdC_function(float Input10V, int *UserLED, int *UserGPIO) // return Output3_3V
	{
	float Output3_3V=1.65;
	float InputPM1V, OutputPM1V; // Mapea entrada y salida a +-1V

	// Input conditioning
	InputPM1V = (Input10V/5.0)-1; // 5 o 2?
	// --**--**-- Inicio del código del alumno --**--**--

	//ECUACIONES EN DIFERENCIAS
	/*Integrador y(n)= 0.001*x(n-1) + y(n-1)
	  FPB        y(n)= 0.05351*x(n-1) + 0.9465*y(n-1)
	  GC         y(n)= 110*x(n)- 109.4*x(n-1) + y(n-1) */
	  
	float ig_n1=0.001, ig_d1=1; //Coeficientes integrador	
	float pb_n1=0.05351, pb_d1=0.9465; //Coeficientes pasabajos
	float gc_n1=110, gc_n2=-109.4, gc_d1=1; //Coeficientes compensador 
	static float in_ig=0, inprev_ig=0, out_ig=0, outprev_ig=0;  //Declaracion + inicializacion integrador
	static float in_gc=0, inprev_gc=0, out_gc=0, outprev_gc=0; //Declaracion + inicializacion comprensador
	static float in_pb=0, inprev_pb=0, out_pb=0, outprev_pb=0; //Declaracion + inicializacion pasabajos
    static float pi=3.14159; //para no escribirlo todo el tiempo
	static float wref; //Wref de 50Hz
    static float realim=0; //Inicializacion de realimentacion
    
	wref=2*pi*50;
    in_pb=InputPM1V*realim; //Inicializacion de la entrada
    

	//PASABAJOS
    out_pb = pb_n1*inprev_pb + pb_d1*outprev_pb; 
    in_gc= out_pb;
    
	//COMPARADOR
    out_gc = gc_n1*in_gc*(-1) + gc_n2*inprev_gc + gc_d1*outprev_gc; //multiplico por -1 por el restador
	in_ig = wref + out_gc;
	
	//INTEGRADOR
	out_ig = ig_n1*inprev_ig + ig_d1*outprev_ig;
	
	
	
	//ACTUALIZO FASE PARA EL INTEGRADOR ENTRE +-2PI
	 if (out_ig > 2*pi)
    {
		out_ig = out_ig - 2*pi;
	}
	else if (out_ig <= -2*pi)
	{	
		out_ig = out_ig+ 2*pi;
	}
	
	//REALIMENTACION
	realim=sin(out_ig);
	
	//n => n-1
	outprev_pb=out_pb;
	inprev_pb=in_pb;
	outprev_gc=out_gc;
	inprev_gc=in_gc;
	inprev_ig=in_ig;
	outprev_ig=out_ig;  


	OutputPM1V=InputPM1V;

		// Output Selector
			switch ( Output_Selector ) {
			case 0:
				OutputPM1V=out_pb; //FPB
			  break;
			case 1:
				OutputPM1V=out_gc; //COMPENSADOR
			  break;
			case 2:
				OutputPM1V=out_ig/(2*3.14159); //INTEGRADOR => FASE
			  break;
			case 3:
				OutputPM1V=realim; //REALIMENTACION => PASADAPOR SEN(X)
			case 4:
				OutputPM1V=InputPM1V; //PARA MEDIR LA ENTRADA
			default:
				OutputPM1V=0;
			  break;
			}

	// Output conditioning
	Output3_3V= (OutputPM1V+1.0)*3.3/2.0;

	return(Output3_3V);
	}
