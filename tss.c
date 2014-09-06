/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "gdt.h"
#include "screen.h"
#include "colors.h"
#include "i386.h"

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

unsigned int offsets_funciones_bandera[CANT_TAREAS];


void tss_inicializar_tarea_idle() {

	 unsigned int page_directory_kernel = 0x28000;

    tarea_idle.ptl = 0x0;          
    tarea_idle.unused0 = 0x0;
    tarea_idle.esp0 = 0x2AFFF;      //pila de nivel cero idle
    tarea_idle.ss0 = 0x98;          //ds kernel
    tarea_idle.unused1 = 0x0;
    tarea_idle.esp1 = 0x0;         
    tarea_idle.ss1 = 0x0;          
    tarea_idle.unused2 = 0x0;      
    tarea_idle.esp2 = 0x0;         
    tarea_idle.ss2 = 0x0;          
    tarea_idle.unused3 = 0x0;
    tarea_idle.cr3 = page_directory_kernel;
    tarea_idle.eip = 0x40000000;
    tarea_idle.eflags = 0x202;
    tarea_idle.eax = 0x0;          
    tarea_idle.ecx = 0x0;
    tarea_idle.edx = 0x0;
    tarea_idle.ebx = 0x0;
    tarea_idle.esp = 0x40001C00;
    tarea_idle.ebp = 0x40001C00;
    tarea_idle.esi = 0x0;
    tarea_idle.edi = 0x0;
    tarea_idle.es = 0x98;           //ds kernel
    tarea_idle.unused4 = 0x0;
    tarea_idle.cs = 0x90;           //cs kernel
    tarea_idle.unused5 = 0x0;
    tarea_idle.ss = 0x98;           //ds kernel
    tarea_idle.unused6 = 0x0;
    tarea_idle.ds = 0x98;           //ds kernel
    tarea_idle.unused7 = 0x0;
    tarea_idle.fs = 0x98;           //ds kernel
    tarea_idle.unused8 = 0x0;
    tarea_idle.gs = 0x98;           //ds kernel
    tarea_idle.unused9 = 0x0;
    tarea_idle.ldt = 0x0;
    tarea_idle.unused10 = 0x0;
    tarea_idle.dtrap = 0x0;
    tarea_idle.iomap = 0x0;
   

    gdt[GDT_IDX_TAREA_IDLE].base_0_15  = ((unsigned int) &(tarea_idle)) & 0xFFFF;
    gdt[GDT_IDX_TAREA_IDLE].base_23_16 = (((unsigned int) &(tarea_idle)) & 0xFF0000) >>16;
    gdt[GDT_IDX_TAREA_IDLE].base_31_24 = (((unsigned int) &(tarea_idle)) & 0xFF000000) >> 24;
   

}

void tss_inicializar(tss* tarea, unsigned short gdt_idx, unsigned int id_tarea) {
   
    unsigned int page_directory_tarea = 0x31000 + (0x6000 * id_tarea);
    unsigned int page_pila_codigo = 0x35ffc + (0x6000 * id_tarea);
    unsigned int page_pila_bandera = 0x36ffc + (0x6000 * id_tarea);

    //int* p = (int*)  0x40001ffc;
	//int flag_address = *p + 0x40000000;


   
   
    //Si el indice es impar, es una TSS de codigo; sino, es de bandera
    if (gdt_idx % 2 != 0) {
               
        //TSS codigo
        tarea->ptl = 0x0;              
        tarea->unused0 = 0x0;  
        tarea->esp0 = page_pila_codigo;
        tarea->ss0 = 0x98;              //ds kernel
        tarea->unused1 = 0x0;
        tarea->esp1 = 0x0;             
        tarea->ss1 = 0x0;              
        tarea->unused2 = 0x0;  
        tarea->esp2 = 0x0;             
        tarea->ss2 = 0x0;              
        tarea->unused3 = 0x0;
        tarea->cr3 = page_directory_tarea;
        tarea->eip = 0x40000000;
        tarea->eflags = 0x202;
        tarea->eax = 0x0;              
        tarea->ecx = 0x0;
        tarea->edx = 0x0;
        tarea->ebx = 0x0;
        tarea->esp = 0x40001C00;
        tarea->ebp = 0x40001C00;
        tarea->esi = 0x0;
        tarea->edi = 0x0;
        tarea->es = 0xAB;               //ds usuario, rpl = 3
        tarea->unused4 = 0x0;
        tarea->cs = 0xA3;               //cs usuario, rpl = 3
        tarea->unused5 = 0x0;
        tarea->ss = 0xAB;               //ds usuario, rpl = 3
        tarea->unused6 = 0x0;
        tarea->ds = 0xAB;               //ds usuario, rpl = 3
        tarea->unused7 = 0x0;
        tarea->fs = 0xAB;               //ds usuario, rpl = 3
        tarea->unused8 = 0x0;
        tarea->gs = 0xAB;               //ds usuario, rpl = 3
        tarea->unused9 = 0x0;
        tarea->ldt = 0x0;
        tarea->unused10 = 0x0;
        tarea->dtrap = 0x0;
        tarea->iomap = 0x0;

	} else {
       
        //TSS bandera
        tarea->ptl = 0x0;              
        tarea->unused0 = 0x0;  
        tarea->esp0 = page_pila_bandera;       
        tarea->ss0 = 0x98;              //ds kernel
        tarea->unused1 = 0x0;
        tarea->esp1 = 0x0;             
        tarea->ss1 = 0x0;              
        tarea->unused2 = 0x0;  
        tarea->esp2 = 0x0;             
        tarea->ss2 = 0x0;              
        tarea->unused3 = 0x0;
        tarea->cr3 = page_directory_tarea;
        tarea->eip = 0x40000000 + offsets_funciones_bandera[id_tarea];
        tarea->eflags = 0x202;
        tarea->eax = 0x0;              
        tarea->ecx = 0x0;
        tarea->edx = 0x0;
        tarea->ebx = 0x0;
        tarea->esp = 0x40001FFC;
        tarea->ebp = 0x40001FFC;
        tarea->esi = 0x0;
        tarea->edi = 0x0;
        tarea->es = 0xAB;               //ds usuario, rpl = 3
        tarea->unused4 = 0x0;
        tarea->cs = 0xA3;               //cs usuario, rpl = 3
        tarea->unused5 = 0x0;
        tarea->ss = 0xAB;               //ds usuario, rpl = 3
        tarea->unused6 = 0x0;
        tarea->ds = 0xAB;               //ds usuario, rpl = 3
        tarea->unused7 = 0x0;
        tarea->fs = 0xAB;               //ds usuario, rpl = 3
        tarea->unused8 = 0x0;
        tarea->gs = 0xAB;               //ds usuario, rpl = 3
        tarea->unused9 = 0x0;
        tarea->ldt = 0x0;
        tarea->unused10 = 0x0;
        tarea->dtrap = 0x0;
        tarea->iomap = 0x0;

 	}


	gdt[gdt_idx].base_0_15 = ((unsigned int) tarea) & 0xFFFF;
	gdt[gdt_idx].base_23_16 = (((unsigned int) tarea) & 0xFF0000) >>16;
	gdt[gdt_idx].base_31_24 = (((unsigned int) tarea) & 0xFF000000) >> 24;
		
	
}

void renovar_banderas(){
		
	int i;
	
	for (i = 0; i < CANT_TAREAS; i++) {
		
		tss* tarea = &(tss_banderas[i]);
				
        tarea->eip = offsets_funciones_bandera[i] + 0x40000000;;
        tarea->esp = 0x40001FFC;
        tarea->ebp = 0x40001FFC;
        
	}
        
}
