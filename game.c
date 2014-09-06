/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "sched.h"
#include "i386.h"
#include "screen.h"
#include "isr.h"


unsigned int game_fondear(unsigned int dir_fisica_ancla_nueva) {
	
	unsigned short indice_tarea = indice_arreglo_tss_tarea_actual();
	tss tarea_actual = tss_navios[indice_tarea];
	unsigned int cr3_tarea_actual = tarea_actual.cr3;
	unsigned int dir_virtual_ancla = 0x40002000;

	//Mapeo la nueva pagina fisica
	unsigned int attr = 0x5; //nivel usuario, presente, lectura
	mmu_mapear_pagina(dir_virtual_ancla, cr3_tarea_actual, dir_fisica_ancla_nueva, attr);
	
	imprimir_paginas_tarea(indice_tarea);

    return TRUE;

}



unsigned int game_canonear(unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta) {
	
	if (dir_misil_fisica <= AREA_TIERRA_FIN) {

		unsigned short indice_tarea = indice_arreglo_tss_tarea_actual();
		desalojar_tarea(indice_tarea);
		_isr13();

	} else {

		char* src = (char*) dir_buffer_absoluta;
		char* dst = (char*) dir_misil_fisica;
		unsigned int size = 97;

		memcpy(dst, src, size);

		imprimir_misil(dir_misil_fisica);

	}
    
    return TRUE;

}



unsigned int game_navegar(unsigned int dir_primera_pag_fisica_nueva, unsigned int dir_segunda_pag_fisica_nueva) {

	unsigned short indice_tarea = indice_arreglo_tss_tarea_actual();

	if (dir_primera_pag_fisica_nueva <= AREA_TIERRA_FIN || dir_segunda_pag_fisica_nueva <= AREA_TIERRA_FIN) {

		desalojar_tarea(indice_tarea);
		_isr13();

	} else {

		tss tarea_actual = tss_navios[indice_tarea];
		unsigned int cr3_tarea_actual = tarea_actual.cr3;	
		unsigned int dir_primera_pag_virtual = 0x40000000;
		unsigned int dir_segunda_pag_virtual = 0x40001000;
		//Obtengo las paginas fisicas actuales
		unsigned int dir_primera_pag_fisica_actual = mmu_unmapear_pagina(dir_primera_pag_virtual, cr3_tarea_actual);
		unsigned int dir_segunda_pag_fisica_actual = mmu_unmapear_pagina(dir_segunda_pag_virtual, cr3_tarea_actual);

		//Mapeo las nuevas paginas fisicas
		unsigned int attr = 0x7; //nivel usuario, presente, lectura/escritura
		mmu_mapear_pagina(dir_primera_pag_virtual, cr3_tarea_actual, dir_primera_pag_fisica_nueva, attr);
		mmu_mapear_pagina(dir_segunda_pag_virtual, cr3_tarea_actual, dir_segunda_pag_fisica_nueva, attr);

		//Copio las paginas fisicas actuales a las nuevas direcciones
		char* src = (char*) dir_primera_pag_fisica_actual;
		char* dst = (char*) dir_primera_pag_fisica_nueva;
		unsigned int size = TAMANO_PAGINA;

		memcpy(dst, src, size);

		src = (char*) dir_segunda_pag_fisica_actual;
		dst = (char*) dir_segunda_pag_fisica_nueva;

		memcpy(dst, src, size);

		imprimir_paginas_tarea(indice_tarea);
	
	}

    return TRUE;

}
