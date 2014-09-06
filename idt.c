/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
#include "isr.h"
#include "i386.h"
#include "colors.h"
#include "screen.h"


extern unsigned int pantalla_activa;


idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};


char* int66 = "Tarea en int66";
char* syscall = "Bandera en Syscall";
char* tiempo_excedido = "Tiempo Excedido";

char* interrupciones[20] = {

    /* 0 */  "Divide By Zero",
    /* 1 */  "Reserved",
    /* 2 */  "Non-Mascable Interrupt",
    /* 3 */  "BreakPoint Exception",
    /* 4 */  "Overflow Exception",
    /* 5 */  "Bound Range Exceeded",
    /* 6 */  "Invalid Opcode",
    /* 7 */  "Device Not Available Exception",
    /* 8 */  "Double Fault",
    /* 9 */  "Coprocessor Segment Overrun",
    /* 10 */ "TSS Invalid Exception",
    /* 11 */ "Segment Not Present Exception",
    /* 12 */ "Stack Fault",
    /* 13 */ "General Protection",
    /* 14 */ "Page Fault",
    /* 15 */ "Reserved",
    /* 16 */ "x87 FPU Floating Point Error",
    /* 17 */ "Alignment Check Exception",
    /* 18 */ "Machine Check Exception",
    /* 19 */ "SIMD Floating Point Exception"

};




/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero, privilegio)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x90;                                                                  \
    idt[numero].attr = (unsigned short) (0x8E00 | (privilegio << 13));                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);



void idt_inicializar() {
    //numero, privilegio
    IDT_ENTRY(0,0);
    IDT_ENTRY(1,0);
    IDT_ENTRY(2,0);
    IDT_ENTRY(3,0);
    IDT_ENTRY(4,0);
    IDT_ENTRY(5,0);
    IDT_ENTRY(6,0);
    IDT_ENTRY(7,0);
    IDT_ENTRY(8,0);
    IDT_ENTRY(9,0);
    IDT_ENTRY(10,0);
    IDT_ENTRY(11,0);
    IDT_ENTRY(12,0);
    IDT_ENTRY(13,0);
    IDT_ENTRY(14,0);
    IDT_ENTRY(15,0);
    IDT_ENTRY(16,0);
    IDT_ENTRY(17,0);
    IDT_ENTRY(18,0);
    IDT_ENTRY(19,0);
    IDT_ENTRY(32,0);
    IDT_ENTRY(33,0);
    IDT_ENTRY(80,3);
    IDT_ENTRY(102,3);

}



void interrupcion_teclado(char scancode) {
	
	//0x2 = 1, 0x3 = 2, ..., 0xb = 0
	
	
	char caracter;
    short fondo;

	switch(scancode) {
		
		case 2: caracter = '1';
                fondo = C_BG_RED;
				 break;
		case 3: caracter = '2';
                fondo = C_BG_BLUE; 
				 break;
		case 4: caracter = '3';
                fondo = C_BG_GREEN;
				 break;
		case 5: caracter = '4';
                fondo = C_BG_BLACK;
				 break;
		case 6: caracter = '5';
                fondo = C_BG_CYAN;
				 break;
		case 7: caracter = '6';
                fondo = C_BG_MAGENTA;
				 break;
		case 8: caracter = '7';
                fondo = C_BG_BROWN;
				 break;
		case 9: caracter = '8';
                fondo = C_BG_LIGHT_GREY;
				 break;
		case 10: caracter = '9';
                 fondo = C_BG_BLUE;
				  break;
		case 11: caracter = '0';
                 fondo = C_BG_CYAN;
				  break;
		
		default: break;
	}
	

	if (scancode >= 2 && scancode <= 11) {

        if (pantalla_activa == estado) {

            imprimir_caracter(caracter, 0, 79, fondo, C_FG_WHITE, estado);
            imprimir_en_pantalla(estado);

        } else {

            imprimir_caracter(caracter, 0, 79, fondo, C_FG_WHITE, mapa);
            imprimir_en_pantalla(mapa);

        }

    }
    

    
   	//breakpoint();

	
    //scancode 18 = 'E'
    if (scancode == 18) {
		
       pantalla_activa = estado;
       imprimir_en_pantalla(estado);
    
    }

    //scancode 50 = 'M'
    if (scancode == 50) {
       
       pantalla_activa = mapa;
       imprimir_en_pantalla(mapa);
    
    }


}
