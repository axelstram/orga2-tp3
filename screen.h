/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__


/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80

#include "defines.h"
#include "mmu.h"
  
typedef enum buffer {mapa, estado} buffer;

extern dir_tareas_t direcciones_tareas[CANT_TAREAS];
extern unsigned short tareas_corriendo[CANT_TAREAS];


void limpiar_pantalla();
void imprimir_caracter(char c, int fila, int columna, int back_color, int fore_color, buffer pantalla);
void imprimir_string(char* s, int fila, int columna, int back_color, int fore_color, buffer pantalla);
void imprimir_interrupcion(unsigned int id_tarea, char* string_interrupcion);
void imprimir_nombre_grupo();
void pintar_mapa();
void screen_pintar_pantalla();
void pintar_estado();
void imprimir_paginas_tarea(unsigned int id_tarea);
void imprimir_misil(unsigned int dir_misil_fisica);
int calcular_fila(unsigned int dir_fisica);
int calcular_columna(unsigned int dir_fisica);
void imprimir_en_pantalla(buffer pantalla);
void imprimir_bandera(char* buffer_bandera, unsigned int id_tarea);
void imprimir_ultimo_problema(unsigned int cr0, unsigned int cr2, unsigned int cr3, unsigned int cr4, unsigned int ebx, 
							  unsigned int ecx, unsigned int edx, unsigned int esi, unsigned int edi, unsigned int ebp, 
							  unsigned int esp, unsigned int eax, unsigned int cs, unsigned int ds, unsigned int es, 
                              unsigned int fs, unsigned int gs, unsigned int ss, unsigned int eflags, unsigned int eip);
void imprimir_relojes_tareas_y_banderas();
void imprimir_registro(char* string, int fila, int columna, int es_segmento, unsigned reg);
void imprimir_pagina(int fila, int bg_color, int fg_color, dir_tareas_t direcciones);
void iniciar_barcos_y_banderas();
void actualizar_relojes(char id_tarea_actual);


extern char* proximo_reloj();

#endif  /* !__SCREEN_H__ */
