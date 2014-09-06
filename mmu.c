/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

dir_tareas_t direcciones_tareas[CANT_TAREAS];

extern unsigned int offsets_funciones_bandera[CANT_TAREAS];

//------------------------------------------------------------------------------------------


void inicializar_tarea_idle() {
	
	tareas_t tareaIdle;

	unsigned int posicion_en_page_directory = 0x100;
	unsigned int attr = 0x3; //nivel kernel, presente, lectura/escritura
	unsigned int virtual = 0x40000000;
	unsigned int cr3_kernel = 0x28000;
	
	tareaIdle.page_directory_tarea = (page_directory_entry*) cr3_kernel; 
	tareaIdle.page_table1_tarea = (page_table_entry*) 0x9F000;
	inicializar_page_table(tareaIdle.page_table1_tarea);
	
	agregar_a_page_directory(tareaIdle.page_directory_tarea, tareaIdle.page_table1_tarea, posicion_en_page_directory, attr);
	attr = 0x7;
	mmu_mapear_pagina(virtual, cr3_kernel, TASK_IDLE_CODE_SRC_ADDR, attr);
	mmu_mapear_pagina(virtual + 0x1000, cr3_kernel, TASK_IDLE_CODE_SRC_ADDR + 0x1000, attr);
	
	tlbflush();
	
}


void inicializar_tareas(tareas_t* tareas) {
	
	
	int i;
	unsigned int tam_codigo_mas_bandera = 2 * TAMANO_PAGINA;
	unsigned int tam_paginas_tarea = 6 * TAMANO_PAGINA;
	
	for (i = 0; i < CANT_TAREAS; i++) {

		tareas[i].id_tarea = i;
		tareas[i].dir_fisica_codigo = 0x10000 + (tam_codigo_mas_bandera * i);
		tareas[i].page_directory_tarea = (page_directory_entry*) (0x31000 + (tam_paginas_tarea * i));
		tareas[i].page_table1_tarea = (page_table_entry*) (0x32000 + (tam_paginas_tarea * i));
		tareas[i].page_table2_tarea = (page_table_entry*) (0x33000 + (tam_paginas_tarea * i));
		tareas[i].page_table3_tarea = (page_table_entry*) (0x34000 + (tam_paginas_tarea * i));
		tareas[i].page_pila_codigo = 0x35000 + (tam_paginas_tarea * i);
		tareas[i].page_pila_bandera = 0x36000 + (tam_paginas_tarea * i);
		

		inicializar_page_directory(tareas[i].page_directory_tarea);
		inicializar_page_table(tareas[i].page_table1_tarea);
		inicializar_page_table(tareas[i].page_table2_tarea);
		inicializar_page_table(tareas[i].page_table3_tarea);
		

		//Agrego paginas del kernel y mapeo aca para hacerlo solo una vez
		unsigned int attr = 0x3; //nivel kernel, presente, lectura/escritura
		unsigned int posicion_en_page_directory = 0x0;
		unsigned int cant_entradas_a_mapear = 1024; //4 MB

		crear_identity_mapping(tareas[i].page_table1_tarea, posicion_en_page_directory, cant_entradas_a_mapear, attr);
		agregar_a_page_directory(tareas[i].page_directory_tarea, tareas[i].page_table1_tarea, posicion_en_page_directory, attr);
		posicion_en_page_directory = 0x1;
		cant_entradas_a_mapear = 896; //3,5 MB
		crear_identity_mapping(tareas[i].page_table2_tarea, posicion_en_page_directory, cant_entradas_a_mapear, attr);
		agregar_a_page_directory(tareas[i].page_directory_tarea, tareas[i].page_table2_tarea, posicion_en_page_directory, attr);


		//Agrego pagina de la tarea
		attr = 0x7; //nivel usuario, presente, lectura/escritura
		posicion_en_page_directory = 0x100;
		agregar_a_page_directory(tareas[i].page_directory_tarea, tareas[i].page_table3_tarea, posicion_en_page_directory, attr);

		tlbflush();

	}
	
	
}



void inicializar_page_directory(page_directory_entry* page_directory) {
	
	int i;
	
	for (i = 0; i < 1024; i++) {
				
		page_directory[i].present = 0;
		page_directory[i].read_write = 1;
		page_directory[i].user_supervisor = 0;
		page_directory[i].pwt = 0;
		page_directory[i].pcd = 0;
		page_directory[i].accessed = 0;
		page_directory[i].reserved1 = 0;
		page_directory[i].page_size = 0;
		page_directory[i].reserved2 = 0;
		page_directory[i].available = 0;
		page_directory[i].page_table_address = 0;
				
	}
		
	
}

void agregar_a_page_directory(page_directory_entry* page_directory, page_table_entry* page_table, unsigned int pos, unsigned int attr) {
	
	unsigned int page_table_shift = (unsigned int) page_table;
	page_table_shift = page_table_shift >> 12;
	page_directory[pos].page_table_address = page_table_shift;
	page_directory[pos].present = attr & 0x1;
	page_directory[pos].read_write = (attr & 0x2) >> 1;
	page_directory[pos].user_supervisor = (attr & 0x4) >> 2;

}


void crear_identity_mapping(page_table_entry* page_table, unsigned int posicion_en_page_directory, unsigned int cant_entradas_a_mapear, unsigned int attr) {

	int i;
	unsigned int cantidad_maxima_entradas_table = 1024; 
	
	for (i = 0; i < cant_entradas_a_mapear; i++) {

		page_table[i].physical_page_address = i + (posicion_en_page_directory * cantidad_maxima_entradas_table);
		page_table[i].present = attr & 0x1;
		page_table[i].read_write = (attr & 0x2) >> 1;
		page_table[i].user_supervisor = (attr & 0x4) >> 2;
		
	}

	tlbflush();

}


void inicializar_page_table(page_table_entry* page_table) {
	
	int i;
	
	for (i = 0; i < 1024; i++) {
				
		page_table[i].present = 0;
		page_table[i].read_write = 1;
		page_table[i].user_supervisor = 0;
		page_table[i].pwt = 0;
		page_table[i].pcd = 0;
		page_table[i].accessed = 0;
		page_table[i].dirty_bit = 0;
		page_table[i].pat = 0;
		page_table[i].reserved = 0;
		page_table[i].available = 0;
		page_table[i].physical_page_address = 0;		
		
	}
	
}
	
	
//cr3 ya viene en 32 bits, no en 20
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned int attr) {
	
	page_directory_entry* page_directory = (page_directory_entry*) cr3;
	virtual = virtual >> 12; //borro el offset
	unsigned int pte = virtual & 0x3ff;
	virtual = virtual >> 10; //borro el table
	unsigned int pde = virtual;
	
	unsigned int page_table_address = (page_directory[pde].page_table_address << 12);
	
	page_table_entry* page_table = (page_table_entry*) page_table_address;

	page_table[pte].present = attr & 0x1;
	page_table[pte].read_write = (attr & 0x2) >> 1;
	page_table[pte].user_supervisor = (attr & 0x4) >> 2;
	page_table[pte].physical_page_address = (fisica >> 12);

}


unsigned int mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {

	page_directory_entry* page_directory = (page_directory_entry*) cr3;
	virtual = virtual >> 12; //borro el offset
	unsigned int pte = virtual & 0x3ff;
	virtual = virtual >> 10; //borro el table
	unsigned int pde = virtual;

	page_table_entry* page_table = (page_table_entry*) (page_directory[pde].page_table_address << 12);
	unsigned int fisica = (page_table[pte].physical_page_address << 12);

	return fisica;

}


void memcpy(char* dst, char* src, unsigned int size) {
	
	unsigned int i;
	
	for (i = 0; i < size; i++) {
		
		*dst = *src;
		dst++;
		src++;
	
	}

}


void mmu_inicializar_dir_tarea(tareas_t* tarea) {

	unsigned int dir_virtual_primera_pagina = 0x40000000;
	unsigned int dir_virtual_segunda_pagina = 0x40001000;
	unsigned int dir_virtual_ancla = 0x40002000;

	unsigned int dir_fisica_primera_pagina_mar = AREA_MAR_INICIO + (0x2000 * tarea->id_tarea);
	unsigned int dir_fisica_segunda_pagina_mar = AREA_MAR_INICIO + 0x1000 + (0x2000 * tarea->id_tarea);
	unsigned int dir_fisica_primera_pagina_tierra = tarea->dir_fisica_codigo;
	unsigned int dir_fisica_segunda_pagina_tierra = tarea->dir_fisica_codigo + 0x1000;

	//Mapeo las 2 paginas de codigo y la bandera de la tarea
	unsigned int cr3 = (unsigned int) tarea->page_directory_tarea;
	unsigned int attr = 0x7; //nivel usuario, presente, lectura/escritura

	mmu_mapear_pagina(dir_virtual_primera_pagina, cr3, dir_fisica_primera_pagina_mar, attr);
	mmu_mapear_pagina(dir_virtual_segunda_pagina, cr3, dir_fisica_segunda_pagina_mar, attr);
	
	attr = 0x5; //nivel usuario, presente, solo lectura
	mmu_mapear_pagina(dir_virtual_ancla, cr3, TASK_ANCLA_FIS, attr);

	//Copio los codigos de las tareas desde la tierra hacia el mar
	
	char* dst = (char*) dir_fisica_primera_pagina_mar;
	char* src = (char*) dir_fisica_primera_pagina_tierra;
	unsigned int size = TAMANO_PAGINA;

	memcpy(dst, src, size);

	dst = (char*) dir_fisica_segunda_pagina_mar;
	src = (char*) dir_fisica_segunda_pagina_tierra;

	memcpy(dst, src, size);
	

	direcciones_tareas[tarea->id_tarea].direccion_fisica_primera_pagina = dir_fisica_primera_pagina_mar;
	direcciones_tareas[tarea->id_tarea].direccion_fisica_segunda_pagina = dir_fisica_segunda_pagina_mar;
	direcciones_tareas[tarea->id_tarea].direccion_fisica_ancla = TASK_ANCLA_FIS;

	
	tlbflush();

}




void mmu_inicializar_dir_kernel() {
	
	page_directory_entry* page_directory_kernel = (page_directory_entry*) 0x28000;
    page_table_entry* page_table1_kernel = (page_table_entry*) 0x29000;
    page_table_entry* page_table2_kernel = (page_table_entry*) 0x30000;

    unsigned int posicion_en_page_directory_kernel = 0;
    unsigned int cant_entradas_a_mapear = 1024; //4 MB
    unsigned int attr = 0x7; //nivel usuario, presente, lectura/escritura

    inicializar_page_directory(page_directory_kernel);
    inicializar_page_table(page_table1_kernel);
    agregar_a_page_directory(page_directory_kernel, page_table1_kernel, posicion_en_page_directory_kernel, attr);
    crear_identity_mapping(page_table1_kernel, posicion_en_page_directory_kernel, cant_entradas_a_mapear, attr);


    cant_entradas_a_mapear = 256; //1 MB
    attr = 0x3; //nivel kernel, presente, lectura/escritura
    crear_identity_mapping(page_table1_kernel, posicion_en_page_directory_kernel, cant_entradas_a_mapear, attr);


    cant_entradas_a_mapear = 896; //3,5 MB
    posicion_en_page_directory_kernel = 1;

   	attr = 0x7;
    inicializar_page_table(page_table2_kernel);
    agregar_a_page_directory(page_directory_kernel, page_table2_kernel, posicion_en_page_directory_kernel, attr);
    crear_identity_mapping(page_table2_kernel, posicion_en_page_directory_kernel, cant_entradas_a_mapear, attr);

}	


char hay_otras_tareas_en_la_misma_pagina(unsigned int id_tarea, unsigned int dir_fisica) {

	int i;
	int cantidad_de_tareas = 0;
	unsigned int tarea;

	for (i = 0; i < CANT_TAREAS; i++) {

		if (i != id_tarea) {

			if (direcciones_tareas[i].direccion_fisica_primera_pagina == dir_fisica) {

				cantidad_de_tareas++;
				tarea = i;

			}

			if (direcciones_tareas[i].direccion_fisica_segunda_pagina == dir_fisica) {

				cantidad_de_tareas++;
				tarea = i;

			}

			if (direcciones_tareas[i].direccion_fisica_ancla == dir_fisica) {

				cantidad_de_tareas++;
				tarea = i;

			}


		}

	}

	char res;

	if (cantidad_de_tareas == 0) {

		res = ' ';

	} else if (cantidad_de_tareas == 1) {

		res = 49 + tarea;

	} else {

		res = 'x';
	}

	return res;

}





void mmu_inicializar() {
	
	tareas_t tareas[CANT_TAREAS];

	mmu_inicializar_dir_kernel();
	inicializar_tarea_idle();
	inicializar_tareas(tareas);
	llenar_arreglo_offsets();


	int i;

	for (i = 0; i < CANT_TAREAS; i++) {

		mmu_inicializar_dir_tarea(&tareas[i]);
		
	}

	tlbflush();

}


void llenar_arreglo_offsets() {

	unsigned int* offset = (unsigned int*) 0x11ffc;
	int i;
	
	for (i = 0; i < CANT_TAREAS; i++) {
		
		offsets_funciones_bandera[i] = *offset;
		offset = offset + (0x2000 / 4);
		
	}
	
	
}


