/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"

/* Estructuras */

typedef struct page_directory_entry {
	
	
	unsigned char present:1;
	unsigned char read_write:1;
	unsigned char user_supervisor:1;
	unsigned char pwt:1;
	unsigned char pcd:1;
	unsigned char accessed:1;
	unsigned char reserved1:1;
	unsigned char page_size:1;		
	unsigned char reserved2:1;
	unsigned char available:3;
	unsigned int  page_table_address:20;
	
	
} __attribute__((__packed__, aligned (4))) page_directory_entry;




typedef struct page_table_entry {
	
	
	unsigned char present:1;
	unsigned char read_write:1;
	unsigned char user_supervisor:1;
	unsigned char pwt:1;
	unsigned char pcd:1;
	unsigned char accessed:1;
	unsigned char dirty_bit:1;
	unsigned char pat:1;			
	unsigned char reserved:1;
	unsigned char available:3;
	unsigned int  physical_page_address:20;
	
		
} __attribute__((__packed__, aligned (4))) page_table_entry;



typedef struct tareas_t {
	
	page_directory_entry* page_directory_tarea;
	page_table_entry* page_table1_tarea;
	page_table_entry* page_table2_tarea;
	page_table_entry* page_table3_tarea;
	unsigned int page_pila_codigo;
	unsigned int page_pila_bandera;
	unsigned int dir_fisica_codigo;
	unsigned int id_tarea;

} __attribute__((__packed__)) tareas_t;


typedef struct dir_tareas_t {

	unsigned int direccion_fisica_primera_pagina;
	unsigned int direccion_fisica_segunda_pagina;
	unsigned int direccion_fisica_ancla;

} __attribute__((__packed__)) dir_tareas_t;



extern dir_tareas_t direcciones_tareas[CANT_TAREAS];

/* Funciones */

void inicializar_tareas(tareas_t* tareas);
void inicializar_tarea_idle();
void inicializar_page_directory(page_directory_entry* page_directory_kernel);
void inicializar_page_table(page_table_entry* page_table);
void agregar_a_page_directory(page_directory_entry* page_directory, page_table_entry* page_table, unsigned int pos, unsigned int attr);
void crear_identity_mapping(page_table_entry* page_table, unsigned int posicion_en_page_directory, unsigned int cant_entradas_a_mapear, unsigned int attr);
void mmu_inicializar_dir_kernel();
void mmu_inicializar_dir_tarea(tareas_t* tarea);
void memcpy(char* dst, char* src, unsigned int size);
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned int attr);
unsigned int mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);
char hay_otras_tareas_en_la_misma_pagina(unsigned int id_tarea, unsigned int dir_fisica);
void mmu_inicializar();
void llenar_arreglo_offsets();


#endif	/* !__MMU_H__ */
