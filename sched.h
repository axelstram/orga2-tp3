/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "defines.h"

void sched_inicializar();
unsigned short sched_proximo_indice();
unsigned short indice_gdt_tarea_actual();
unsigned short indice_arreglo_tss_tarea_actual();
unsigned short indice_arreglo_tss_bandera_actual();
void desalojar_tarea(unsigned short id_tarea);
short sched_proxima_bandera();


extern unsigned short tareas_corriendo[CANT_TAREAS];
extern unsigned short banderas_corriendo[CANT_TAREAS];



#endif	/* !__SCHED_H__ */
