/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/


#include "screen.h"
#include "colors.h"
#include "i386.h"
#include "sched.h"





void imprimir_caracter(char c, int fila, int columna, int back_color, int fore_color, buffer pantalla) {

	short* dst;

	if (pantalla == estado) {
		
		dst = (short*) 0x2D000;

	} else {

		dst = (short*) 0x2E000;
	}	

	*(dst + VIDEO_COLS * fila + columna) = (short) ((back_color | fore_color) << 8) | c;

}



void imprimir_string(char* string, int fila, int columna, int back_color, int fore_color, buffer pantalla) {
	
	
	while (*string != 0) { //10 == new line
		
		imprimir_caracter(*string, fila, columna, back_color, fore_color, pantalla);

		columna++;
		string++;
		
	}
		
}

void imprimir_interrupcion(unsigned int id_tarea, char* string_interrupcion) {

	id_tarea = id_tarea;
	char* navio = "Navio  ";
	navio[6] = 49 + id_tarea;
	short* dst = (short*) 0x2D000;
	short c;

	int fila;
	int columna;
	//Limpio
	fila = 1;
	for (columna = 50; columna < 78; columna++) {
		
		imprimir_caracter(' ', fila, columna, C_BG_CYAN, C_FG_BLACK, estado);

	}

	fila = 16 + id_tarea;
	for (columna = 3; columna < VIDEO_COLS - 1; columna ++) {

		c = *(dst + VIDEO_COLS * fila + columna);
		c = (short) (((C_BG_RED | C_FG_WHITE) << 8) | c);
		imprimir_caracter(c, fila, columna, C_BG_RED, C_FG_WHITE, estado);

	}


	imprimir_string(navio, 1, 70, C_BG_CYAN, C_FG_BLACK, estado);
	imprimir_string(string_interrupcion, 1, 51, C_BG_CYAN, C_FG_BLACK, estado);
	imprimir_string(string_interrupcion, fila, 55, C_BG_RED, C_FG_WHITE, estado);
	
}


void imprimir_nombre_grupo()  {
	
	imprimir_string("Pistacho/Chungo", 0, 1, C_BG_BLACK, C_FG_WHITE, estado);

}



void limpiar_pantalla() {
	
	int fila;
	int columna;
	short* pantalla = (short*) 0xb8000;
	
	for (fila = 0; fila < VIDEO_FILS; fila++) {
		
		for (columna = 0; columna < VIDEO_COLS; columna++) {
			
			*(pantalla + VIDEO_COLS * fila + columna) = (short) ((C_BG_BLACK | C_FG_BLACK) << 8) | ' ';

		}
	 
	}

	
}

//Pinta el mapa de la Figura 10
void pintar_mapa() {

	int fila;
	int columna;

	/* Pintar tierra */
	for (fila = 0; fila < 3; fila++) {

		for (columna = 0; columna < VIDEO_COLS; columna++) {

			imprimir_caracter(' ', fila, columna, C_BG_GREEN, C_FG_BLACK, mapa); 

		}

	}

	for (columna = 0; columna < 16; columna++) {

		imprimir_caracter(' ', 3, columna, C_BG_GREEN, C_FG_BLACK, mapa);

	}
	/* Fin tierra */


	/* Pintar mar */
	for (columna = 16; columna < VIDEO_COLS; columna++) {

		imprimir_caracter(' ', 3, columna, C_BG_BLUE, C_FG_BLACK, mapa);

	}

	for (fila = 4; fila < VIDEO_FILS - 1; fila++) {

		for (columna = 0; columna < VIDEO_COLS; columna++) {

			imprimir_caracter(' ', fila, columna, C_BG_BLUE, C_FG_BLACK, mapa);

		}

	}
	/* Fin mar */

	/* Ultima linea */

	for (columna = 0; columna < VIDEO_COLS; columna++) {

		imprimir_caracter(' ', VIDEO_FILS, columna, C_BG_BLACK, C_FG_WHITE, mapa);

	}


}

void pintar_estado(){

	int fila = 0;
	int columna = 0;

	/* Primera linea */ 
	for(columna = 0; columna < VIDEO_COLS; columna++) {

		imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_BLACK, estado);
	
	}


	/* Parte de banderas */
	for (fila = 1; fila < 15; fila++) {
		
		for(columna = 1; columna < 50; columna++) {
		
			imprimir_caracter(' ', fila, columna, C_BG_LIGHT_GREY, C_FG_WHITE, estado);
		
		}
		
	}

	/* Numeros navios */

	fila = 1;
	columna = 5;
	char* navio = "Navio 1"; 
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 17;
	navio = "Navio 2";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 30;
	navio = "Navio 3";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 42;
	navio = "Navio 4";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	

	fila = 9;
	columna = 5;
	navio = "Navio 5"; 
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 17;
	navio = "Navio 6";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 30;
	navio = "Navio 7";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
	columna = 42;
	navio = "Navio 8";
	imprimir_string(navio, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);


	/* Ultimo problema*/
	/* Barrita de arriba */
	fila = 1;
	for (columna = 50; columna < 78; columna++) {
		
		imprimir_caracter(' ', fila, columna, C_BG_CYAN, C_FG_BLACK, estado);

	}

	/* Rectangulo negro */
	for (fila = 2; fila < 15; fila++) {

		for(columna = 50; columna < 78; columna++) {

			imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_BLACK, estado);
	
		}

	}
	
	/*Delimitar ultimo problema*/
	for (fila = 1; fila < 15; fila++) {

		for(columna = 78; columna < VIDEO_COLS; columna++) {
		
			imprimir_caracter(' ', fila, columna, C_BG_LIGHT_GREY, C_FG_WHITE, estado);
		
			}
	
	}
	
	/*Fila intermedia*/
	fila = 15;
	for(columna = 1; columna < VIDEO_COLS; columna++) {

		imprimir_caracter(' ', fila, columna, C_BG_LIGHT_GREY, C_FG_WHITE, estado);
	
	}

	/*Estados */


	/* Barrita negra vertical */
	columna = 1;
	for (fila = 16; fila < 24; fila++) {

		imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_WHITE, estado);

	}

	/* Numeros */

	columna = 2;
	char numero = '1';

	for (fila = 16; fila < 24; fila++) {

		imprimir_caracter(numero, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
		numero++;

	}


	/* Info estados */
	for(fila = 16; fila < 24; fila++) {

		for(columna = 3; columna < 79; columna++) {
		
			imprimir_caracter(' ', fila, columna, C_BG_CYAN, C_FG_BLACK, estado);
		
		}
	
	}
	

	columna = 79;
	for(fila = 16; fila < 24; fila++) {

		imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_BLACK, estado);
	
	}

	/*Ultima linea */
	fila = 24;
	for(columna = 0; columna < VIDEO_COLS; columna++) {

		imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_BLACK, estado);

	}


	/* Linea vertical primera columna */
	columna = 0;
	for (fila = 1; fila < VIDEO_FILS; fila++) {

		imprimir_caracter(' ', fila, columna, C_BG_BLACK, C_FG_WHITE, estado);

	}

}



void imprimir_paginas_tarea(unsigned int id_tarea) {
	
	unsigned int dir_virtual_primera_pagina = 0x40000000;
	unsigned int dir_virtual_segunda_pagina = 0x40001000;
	unsigned int dir_virtual_ancla = 0x40002000;
	unsigned int cr3_tarea =  0x31000 + (0x6000 * id_tarea);
	
	unsigned int dir_fisica_primera_pagina_actual = mmu_unmapear_pagina(dir_virtual_primera_pagina, cr3_tarea);
	unsigned int dir_fisica_segunda_pagina_actual = mmu_unmapear_pagina(dir_virtual_segunda_pagina, cr3_tarea);
	unsigned int dir_fisica_ancla_actual = mmu_unmapear_pagina(dir_virtual_ancla, cr3_tarea);
	
	unsigned int dir_fisica_primera_pagina_previa = direcciones_tareas[id_tarea].direccion_fisica_primera_pagina;
	unsigned int dir_fisica_segunda_pagina_previa = direcciones_tareas[id_tarea].direccion_fisica_segunda_pagina;
	unsigned int dir_fisica_ancla_previa = direcciones_tareas[id_tarea].direccion_fisica_ancla;
	
	
	int fila = 1; 
	int columna = 1;
	
	char caracter;
	//Mapa
	if (dir_fisica_primera_pagina_previa != dir_fisica_primera_pagina_actual ||
		dir_fisica_segunda_pagina_previa != dir_fisica_segunda_pagina_actual) {
		//si cambia la primera fisica, cambia tambien la segunda

		//cambio la pagina previa y tengo que ver si dejo una X, un numero, o vacio
	    caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_primera_pagina_previa); //ir a mmu.c para ver bien que hace

		fila = calcular_fila(dir_fisica_primera_pagina_previa);
		columna = calcular_columna(dir_fisica_primera_pagina_previa);

		if (caracter == ' ') {
			
			imprimir_caracter(caracter, fila, columna, C_BG_BLUE, C_FG_BLUE, mapa);
	
		} else {

			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}

		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_primera_pagina_actual);

		fila = calcular_fila(dir_fisica_primera_pagina_actual);
		columna = calcular_columna(dir_fisica_primera_pagina_actual);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);
	
		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}

		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_segunda_pagina_previa); //ir a mmu.c para ver bien que hace

		fila = calcular_fila(dir_fisica_segunda_pagina_previa);
		columna = calcular_columna(dir_fisica_segunda_pagina_previa);

		if (caracter == ' ') {

			imprimir_caracter(caracter, fila, columna, C_BG_BLUE, C_FG_BLUE, mapa);
	
		} else {

			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}

		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_segunda_pagina_actual);

		fila = calcular_fila(dir_fisica_segunda_pagina_actual);
		columna = calcular_columna(dir_fisica_segunda_pagina_actual);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);
	
		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}


		direcciones_tareas[id_tarea].direccion_fisica_primera_pagina = dir_fisica_primera_pagina_actual;
		direcciones_tareas[id_tarea].direccion_fisica_segunda_pagina = dir_fisica_segunda_pagina_actual;


	}


	if (dir_fisica_ancla_previa != dir_fisica_ancla_actual) {

		char caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_ancla_previa);

		fila = calcular_fila(dir_fisica_ancla_previa);
		columna = calcular_columna(dir_fisica_ancla_previa);

		if (caracter == ' ') {

			imprimir_caracter(caracter, fila, columna, C_BG_GREEN, C_FG_BLUE, mapa);
	
		} else {

			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}

		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_ancla_actual);

		fila = calcular_fila(dir_fisica_ancla_actual);
		columna = calcular_columna(dir_fisica_ancla_actual);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);
	
		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}

		direcciones_tareas[id_tarea].direccion_fisica_ancla = dir_fisica_ancla_actual;


	}


	//Estado
	int i = 0;

	for (fila = 16; fila <= 23; fila++) {

		if (tareas_corriendo[i] != 0) {

			imprimir_pagina(fila, C_BG_CYAN, C_FG_BLACK, direcciones_tareas[i]);
			
		}

		i++;

	}

}




void imprimir_pagina(int fila, int bg_color, int fg_color, dir_tareas_t direcciones) {

	char c;
	int columna = 3;
	char* string = "P1: 0x        ";
	unsigned int primera_pagina = direcciones.direccion_fisica_primera_pagina;
	unsigned int segunda_pagina = direcciones.direccion_fisica_segunda_pagina;
	unsigned int tercera_pagina = direcciones.direccion_fisica_ancla;

	c = (char) ((primera_pagina >> 28) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[6] = c;

	c = (char) ((primera_pagina >> 24) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[7] = c;

	c = (char) ((primera_pagina >> 20) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[8] = c;

	c = (char) ((primera_pagina >> 16) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[9] = c;

	c = (char) ((primera_pagina >> 12) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[10] = c;

	c = (char) ((primera_pagina >> 8) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[11] = c;

	c = (char) ((primera_pagina >> 4) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[12] = c;

	c = (char) (primera_pagina & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[13] = c;

	imprimir_string(string, fila, columna, fg_color, bg_color, estado);



	columna = 18;
	string = "P2: 0x        ";
	
	c = (char) ((segunda_pagina >> 28) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[6] = c;

	c = (char) ((segunda_pagina >> 24) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[7] = c;

	c = (char) ((segunda_pagina >> 20) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[8] = c;

	c = (char) ((segunda_pagina >> 16) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[9] = c;

	c = (char) ((segunda_pagina >> 12) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[10] = c;

	c = (char) ((segunda_pagina >> 8) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[11] = c;

	c = (char) ((segunda_pagina >> 4) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[12] = c;

	c = (char) (segunda_pagina & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[13] = c;

	imprimir_string(string, fila, columna, fg_color, bg_color, estado);


	columna = 33;
	string = "P3: 0x        ";

	c = (char) ((tercera_pagina >> 28) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[6] = c;

	c = (char) ((tercera_pagina >> 24) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[7] = c;

	c = (char) ((tercera_pagina >> 20) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[8] = c;

	c = (char) ((tercera_pagina >> 16) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[9] = c;

	c = (char) ((tercera_pagina >> 12) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[10] = c;

	c = (char) ((tercera_pagina >> 8) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[11] = c;

	c = (char) ((tercera_pagina >> 4) & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[12] = c;

	c = (char) (tercera_pagina & 0xF);
	c = c + 48;
	if (c > 57) {c = c + 7;}
	string[13] = c;

	imprimir_string(string, fila, columna, fg_color, bg_color, estado);

}





void imprimir_misil(unsigned int dir_misil_fisica) {

	static int ultima_posicion = -1;

	int fila;
	int columna;
	//Primer misil
	if (ultima_posicion == -1) {

		fila = calcular_fila(dir_misil_fisica); 
		columna = calcular_columna(dir_misil_fisica);

		imprimir_caracter(' ', fila, columna, C_BG_LIGHT_GREY, C_FG_WHITE, mapa);
		ultima_posicion = dir_misil_fisica;

	} else {

		fila = calcular_fila(ultima_posicion);
		columna = calcular_columna(ultima_posicion);
		imprimir_caracter(' ', fila, columna, C_BG_BLUE, C_FG_BLUE, mapa);

		fila = calcular_fila(dir_misil_fisica);
		columna = calcular_columna(dir_misil_fisica);
		imprimir_caracter(' ', fila, columna, C_BG_LIGHT_GREY, C_FG_WHITE, mapa);

		ultima_posicion = dir_misil_fisica;
	

	}


}


void imprimir_ultimo_problema(unsigned int cr0, unsigned int cr2, unsigned int cr3, unsigned int cr4, unsigned int ebx, 
							  unsigned int ecx, unsigned int edx, unsigned int esi, unsigned int edi, unsigned int ebp, 
							  unsigned int esp, unsigned int eax, unsigned int cs, unsigned int ds, unsigned int es, 
                              unsigned int fs, unsigned int gs, unsigned int ss, unsigned int eflags, unsigned int eip) 

{


	int fila = 2;
	int columna = 50;
	char* string = "EAX         ";
	int es_segmento = 0;

	imprimir_registro(string, fila, columna, es_segmento, eax);


	fila = 3;
	string = "EBX         ";

	imprimir_registro(string, fila, columna, es_segmento, ebx);


	fila = 4;
	string = "ECX         ";

	imprimir_registro(string, fila, columna, es_segmento, ecx);	


	fila = 5;
	string = "EDX         ";

	imprimir_registro(string, fila, columna, es_segmento, edx);


	fila = 6;
	string = "ESI         ";

	imprimir_registro(string, fila, columna, es_segmento, esi);


	fila = 7;
	string = "EDI         ";

	imprimir_registro(string, fila, columna, es_segmento, edi);
	

	fila = 8;
	string = "EBP         ";

	imprimir_registro(string, fila, columna, es_segmento, ebp);


	fila = 9;
	string = "ESP         ";

	imprimir_registro(string, fila, columna, es_segmento, esp);

	
	fila = 10;
	string = "EIP         ";

	imprimir_registro(string, fila, columna, es_segmento, eip);


	fila = 11;
	string = "CR0         ";

	imprimir_registro(string, fila, columna, es_segmento, cr0);


	fila = 12;
	string = "CR2         ";

	imprimir_registro(string, fila, columna, es_segmento, cr2);


	fila = 13;
	string = "CR3         ";

	imprimir_registro(string, fila, columna, es_segmento, cr3);


	fila = 14;
	string = "CR4         ";

	imprimir_registro(string, fila, columna, es_segmento, cr4);


	es_segmento = 1;
	fila = 2;
	columna = 64;
	string = "CS        ";

	imprimir_registro(string, fila, columna, es_segmento, cs);


	fila = 3;
	string = "DS         ";

	imprimir_registro(string, fila, columna, es_segmento, ds);


	fila = 4;
	string = "ES         ";

	imprimir_registro(string, fila, columna, es_segmento, es);


	fila = 5;
	string = "FS         ";

	imprimir_registro(string, fila, columna, es_segmento, fs);


	fila = 6;
	string = "GS         ";

	imprimir_registro(string, fila, columna, es_segmento, gs);


	fila = 7;
	string = "SS         ";

	imprimir_registro(string, fila, columna, es_segmento, ss);


	es_segmento = 0;

	fila = 8;
	string = "EFLAGS ";

	imprimir_string(string, fila, columna, C_BG_BLACK, C_FG_WHITE, estado);

	fila = 9;
	string = "            ";
	imprimir_registro(string, fila, columna, es_segmento, eflags);


}



void imprimir_registro(char* string, int fila, int columna, int es_segmento, unsigned reg) {

	char c;

	if (!es_segmento) {

		c = (char) ((reg >> 28) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[4] = c;

		c = (char) ((reg >> 24) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[5] = c;

		c = (char) ((reg >> 20) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[6] = c;

		c = (char) ((reg >> 16) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[7] = c;

		c = (char) ((reg >> 12) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[8] = c;

		c = (char) ((reg >> 8) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[9] = c;

		c = (char) ((reg >> 4) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[10] = c;

		c = (char) (reg & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[11] = c;

	} else {

		c = (char) ((reg >> 12) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[3] = c;

		c = (char) ((reg >> 8) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[4] = c;

		c = (char) ((reg >> 4) & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[5] = c;

		c = (char) (reg & 0xF);
		c = c + 48;
		if (c > 57) {c = c + 7;}
		string[6] = c;

	}

	imprimir_string(string, fila, columna, C_BG_BLACK, C_FG_WHITE, estado);

}




void imprimir_en_pantalla(buffer pantalla) {

	char* src;
	char* dst = (char*) 0xb8000;
	unsigned int size = 4000;

	if (pantalla == estado) {

		src = (char*) 0x2D000;

	} else {

		src = (char*) 0x2E000;

	}

	memcpy(dst, src, size);

}





int calcular_fila(unsigned int dir_fisica) {

	int fila = dir_fisica / 0x1000;
	fila = fila / 80;

	return fila; 

}






int calcular_columna(unsigned int dir_fisica) {

	int columna = (dir_fisica / 0x1000) % 80;
	
	return columna;

}





void imprimir_bandera(char* buffer_bandera, unsigned int id_tarea) {

	unsigned int offset = 0x2D000;		//Le pongo la base del buffer
	
		switch(id_tarea) {				//Le agrego el offset correspondiente segun navio.
		
		case 0: offset += 484;
                break;
		case 1: offset += 508; 
				 break;
		case 2: offset += 532;
				 break;
		case 3: offset += 556;
				 break;
		case 4: offset += 1604;
				 break;
		case 5: offset += 1628;
				 break;
		case 6: offset += 1652;
				 break;
		case 7: offset += 1676;
				 break;
		
		default: break;
	}
	
	int i = 0;
	
	//Tengo que escribirlo por fila, por eso el for.
	
	while( i < 5 ){
		memcpy( (char*)offset, buffer_bandera, 20);
		
		offset += 160;
		buffer_bandera += 20;
		i++;
	}

	
}



void iniciar_barcos_y_banderas() {

	int id_tarea;
	char caracter;
	int fila;
	int columna;

	for (id_tarea = 0; id_tarea < CANT_TAREAS; id_tarea++) {

		unsigned int dir_fisica_primera_pagina = direcciones_tareas[id_tarea].direccion_fisica_primera_pagina;
		unsigned int dir_fisica_segunda_pagina = direcciones_tareas[id_tarea].direccion_fisica_segunda_pagina;
		unsigned int dir_fisica_ancla = direcciones_tareas[id_tarea].direccion_fisica_ancla;


		//Primera pagina
		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_primera_pagina);
		fila = calcular_fila(dir_fisica_primera_pagina);
		columna = calcular_columna(dir_fisica_primera_pagina);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}


		//Segunda pagina
		caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_segunda_pagina);
		fila = calcular_fila(dir_fisica_segunda_pagina);
		columna = calcular_columna(dir_fisica_segunda_pagina);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}


		//Ancla
				caracter = hay_otras_tareas_en_la_misma_pagina(id_tarea, dir_fisica_ancla);
		fila = calcular_fila(dir_fisica_ancla);
		columna = calcular_columna(dir_fisica_ancla);

		if (caracter == ' ') {

			caracter = 49 + id_tarea;
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		} else {

			caracter = 'x';
			imprimir_caracter(caracter, fila, columna, C_BG_RED, C_FG_WHITE, mapa);

		}


	}


}




void imprimir_relojes_tareas_y_banderas() {

	int fila = 24;
	int columna = 0;
	int i = 0;
	char id_tarea;
	char id_bandera;
	char* reloj = proximo_reloj();

	imprimir_caracter(*reloj, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, mapa);
	imprimir_caracter(*reloj, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);

	imprimir_caracter(*reloj, fila, 79, C_BG_BLACK, C_FG_WHITE, mapa);
	imprimir_caracter(*reloj, fila, 79, C_BG_BLACK, C_FG_WHITE, estado);


	for (columna = 3; columna <= 26; columna += 3) {

		id_tarea = 49 + i;

		imprimir_caracter(id_tarea, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
		imprimir_caracter(*reloj, fila, columna+1, C_BG_LIGHT_GREY, C_FG_BLACK, estado);

		imprimir_caracter(id_tarea, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, mapa);
		imprimir_caracter(*reloj, fila, columna+1, C_BG_LIGHT_GREY, C_FG_BLACK, mapa);

		i++;

	}

	i = 0;

	for (columna = 28; columna <= 51; columna += 3) {

		id_bandera = 49 + i;

		imprimir_caracter(id_bandera, fila, columna, C_BG_BROWN, C_FG_BLACK, estado);
		imprimir_caracter(*reloj, fila, columna+1, C_BG_BROWN, C_FG_BLACK, estado);

		imprimir_caracter(id_bandera, fila, columna, C_BG_BROWN, C_FG_BLACK, mapa);
		imprimir_caracter(*reloj, fila, columna+1, C_BG_BROWN, C_FG_BLACK, mapa);

		i++;

	}


}




void actualizar_relojes(char id_tarea_actual) {


	char* reloj = proximo_reloj();
	
	int fila = 24;
	int columna = 0;
	int i = 0;
	char id_tarea;

	//Relojes tareas
	for (columna = 3; columna <= 26; columna += 3) {

		id_tarea = 49 + i;

		if (tareas_corriendo[i] == 0) {
			
			imprimir_caracter(id_tarea, fila, columna, C_BG_RED, C_FG_WHITE, mapa);
			imprimir_caracter(' ', fila, columna+1, C_BG_RED, C_FG_WHITE, mapa);

			imprimir_caracter(id_tarea, fila, columna, C_BG_RED, C_FG_WHITE, estado);
			imprimir_caracter(' ', fila, columna+1, C_BG_RED, C_FG_WHITE, estado);
			
		}
		
		if (id_tarea_actual == i) {

			imprimir_caracter(id_tarea, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, mapa);
			imprimir_caracter(*reloj, fila, columna+1, C_BG_LIGHT_GREY, C_FG_BLACK, mapa);

			imprimir_caracter(id_tarea, fila, columna, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
			imprimir_caracter(*reloj, fila, columna+1, C_BG_LIGHT_GREY, C_FG_BLACK, estado);
			
		}
		

		i++;

	}

	
	i = 0;
	//Relojes banderas
	for (columna = 28; columna <= 51; columna += 3) {

		id_tarea = 49 + i;

		if (banderas_corriendo[i] == 0) {
			
			imprimir_caracter(id_tarea, fila, columna, C_BG_RED, C_FG_WHITE, mapa);
			imprimir_caracter(' ', fila, columna+1, C_BG_RED, C_FG_WHITE, mapa);

			imprimir_caracter(id_tarea, fila, columna, C_BG_RED, C_FG_WHITE, estado);
			imprimir_caracter(' ', fila, columna+1, C_BG_RED, C_FG_WHITE, estado);
			
		}

		if (id_tarea_actual == i) {
			
			imprimir_caracter(id_tarea, fila, columna, C_BG_BROWN, C_FG_BLACK, mapa);
			imprimir_caracter(*reloj, fila, columna+1, C_BG_BROWN, C_FG_BLACK, mapa);

			imprimir_caracter(id_tarea, fila, columna, C_BG_BROWN, C_FG_BLACK, estado);
			imprimir_caracter(*reloj, fila, columna+1, C_BG_BROWN, C_FG_BLACK, estado);
			
		}


		i++;

	}
	
	imprimir_caracter(*reloj, fila, VIDEO_COLS-1, C_BG_BLACK, C_FG_WHITE, mapa);
	imprimir_caracter(*reloj, fila, VIDEO_COLS-1, C_BG_BLACK, C_FG_WHITE, estado);

}
