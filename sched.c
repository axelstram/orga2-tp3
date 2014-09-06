/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "gdt.h"
#include "tss.h"
#include "i386.h"

unsigned short tareas_corriendo[CANT_TAREAS]; //Cero si esta desalojada, indice_gdt en otro caso
unsigned short banderas_corriendo[CANT_TAREAS];

extern unsigned short ultima_tarea_ejecutada;

void sched_inicializar() {
	
	tss_inicializar_tarea_idle();
	ultima_tarea_ejecutada = 0;
	
	
	unsigned int i;
	unsigned short gdt_idx = 25; //Indice en la GDT del primer TSS de codigo
	
	for (i = 0; i < CANT_TAREAS; i++) {
		
		tareas_corriendo[i] = gdt_idx;
		banderas_corriendo[i] = gdt_idx+1;

		tss_inicializar(&tss_navios[i], gdt_idx, i);
		gdt_idx++;
		tss_inicializar(&tss_banderas[i], gdt_idx, i);
		gdt_idx++;
		
	}
		
}


unsigned short sched_proximo_indice() {
	
	unsigned short i = (ultima_tarea_ejecutada) % 8;
       
    while (tareas_corriendo[i] == 0) {
               
    	i = (i + 1) % 8; //Si no hay tareas corriendo, se cuelga.
               
    }
       
    return tareas_corriendo[i];
	
}


short sched_proxima_bandera() {
	
	static unsigned short i = 0;
	
	while(i < 8) {
		
		if (banderas_corriendo[i] != 0) {
			
			i++;
			return banderas_corriendo[i-1]; 
		
		}
			
		else {
			
			i++;
		
		}
			
	}
	
	i = 0;
	
	return -1;
	
}



//Devuelve el indice en la GDT del TSS de la tarea actual
unsigned short indice_gdt_tarea_actual() {
	
	unsigned short indice_gdt;
	
	for (indice_gdt = 24; indice_gdt < 41; indice_gdt++) {
		
		//0xb == TSS Busy
		if (gdt[indice_gdt].type == 0xb) {
			
			return indice_gdt;
			
		}
		
	}	
	
	return 0;
	
}


//Devuelve el indice en el arreglo de TSS de la tarea actual
unsigned short indice_arreglo_tss_tarea_actual() {

	unsigned short indice_arreglo = 0;
	unsigned short i;

	for (i = 25; i < 41; i+=2) {
		
		//0xb == TSS Busy
		if (gdt[i].type == 0xb) {
			
			return indice_arreglo;			

		} else {

			indice_arreglo++;

		}
		
	}

	return 0;	

}



unsigned short indice_arreglo_tss_bandera_actual() {

	unsigned short indice_arreglo = 0;
	unsigned short i;

	for (i = 26; i < 42; i+=2) {
		
		//0xb == TSS Busy
		if (gdt[i].type == 0xb) {
			
			return indice_arreglo;			

		} else {

			indice_arreglo++;

		}
		
	}

	return 0;	

}



void desalojar_tarea(unsigned short id_tarea) {

		tareas_corriendo[id_tarea] = 0;
		banderas_corriendo[id_tarea] = 0;

}


