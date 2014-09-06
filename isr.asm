; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32


;; Arreglo de interrupciones
extern interrupciones
extern imprimir_interrupcion

;; PIC
extern fin_intr_pic1

;; Interrupciones
extern interrupcion_teclado

;;Scheduler
extern sched_proximo_indice
extern sched_proxima_bandera
extern indice_gdt_tarea_actual
extern id_tarea_actual
extern indice_arreglo_tss_bandera_actual
extern desalojar_tarea
extern indice_arreglo_tss_tarea_actual

;;Syscalls
extern game_fondear
extern game_canonear
extern game_navegar

;;MISC
extern imprimir_bandera
extern imprimir_ultimo_problema
extern renovar_banderas
extern int66
extern syscall
extern tiempo_excedido

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%define int_reloj 32
%define int_teclado 33
%define puerto_teclado 96	;0x60
%define int_80 80 			;0x50
%define int_102 102			;0x66

obtener_eip: mov eax, [esp]
			 ret

%macro pushear_todo 0
	
	push eax

	call obtener_eip

	push eax ;eip
	mov eax, [esp+4]	;recupero eax

    pushfd  ;flags
	push ss 
	push gs
	push fs
	push es
	push ds
	push cs
	
	push eax	

	push esp
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx

%endmacro




%macro ISR 1
global _isr%1

_isr%1:
.loopear:
	
	
	cli
	push eax
	mov eax, %1
	
	cmp DWORD eax, 8
	je .con_error_code
	
	cmp DWORD eax, 10
	je .con_error_code
	
	cmp DWORD eax, 11
	je .con_error_code
	
	cmp DWORD eax, 12
	je .con_error_code
	
	cmp DWORD eax, 13
	je .con_error_code
	
	cmp DWORD eax, 14
	je .con_error_code
	
	cmp DWORD eax, 17
	je .con_error_code
	
	jmp .sin_error_code
	

.con_error_code:

	pop eax
	
	push DWORD [esp+4] ;eip
    push DWORD [esp+12+4]  ;flags
	push DWORD [esp+20+8] ;ss
	push DWORD [esp+20+8] ;gs
	push DWORD [esp+20+8] ;fs
	push DWORD [esp+20+8] ;es
	push DWORD [esp+20+8] ;ds
	push DWORD [esp+8+28] ;cs
	
	push eax	

	mov DWORD eax, [esp+16+36]
	push eax ;esp
	
	jmp .seguir_pusheando

.sin_error_code:

	pop eax

	push DWORD [esp] ;eip
    push DWORD [esp+8+4]  ;flags
	push DWORD [esp+16+8] ;ss
	push DWORD [esp+20+8] ;gs
	push DWORD [esp+20+8] ;fs
	push DWORD [esp+20+8] ;es
	push DWORD [esp+20+8] ;ds
	push DWORD [esp+4+28] ;cs
	
	push eax	

	mov DWORD eax, [esp+12+36]
	push eax ;esp

.seguir_pusheando:
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	
	mov eax, cr4
	push eax
	mov eax, cr3
	push eax
	mov eax, cr2
	push eax
	mov eax, cr0
	push eax
	
	call imprimir_ultimo_problema

	add esp, 80

	pushad

	mov eax, interrupciones
	mov ebx, %1 
		
	mov eax, [eax + ebx * 4]
	
	push eax	;string interrupcion

	xor eax, eax
	call indice_arreglo_tss_tarea_actual

	mov ebx, eax
	xor eax, eax
	call indice_arreglo_tss_bandera_actual
	or eax, ebx
	
	push eax	;id_tarea

	call imprimir_interrupcion
	add esp, 8
	
	xor eax, eax
	xor edx, edx
	call indice_gdt_tarea_actual
	
	xor ebx, ebx
	mov bx, 2
	
	div bx
	
	cmp WORD dx, 0
	je .bandera
	jmp .tarea

	
.bandera:
	
	xor eax, eax
	call indice_arreglo_tss_bandera_actual
	push eax
	call desalojar_tarea
	add esp, 4
	jmp .fin

.tarea:

	xor eax, eax
	call indice_arreglo_tss_tarea_actual
	push eax
	call desalojar_tarea
	add esp, 4
	
	
.fin:
	popad
	sti	
	
	jmp 0xC0:0
	
	jmp $
	
%endmacro




;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'


;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;


ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
global ultima_tarea_ejecutada
extern pantalla_activa
extern imprimir_en_pantalla
extern actualizar_relojes

ultima_tarea_ejecutada: dd 0
contador_bandera: dd 0
offset: dd 0;
selector: dw 0


_isr32:

	pushad
	
	push DWORD [pantalla_activa]
	call imprimir_en_pantalla
	add esp, 4

	xor eax, eax
	xor edx, edx
	xor ebx, ebx
	call indice_gdt_tarea_actual
	cmp WORD ax, 24
	je .seguir

	mov bx, 2
	div bx

	cmp WORD dx, 0
	je .desalojar_tarea
	
.seguir:
	cmp DWORD [contador_bandera], 3
	je .imprimir_banderas

    inc DWORD [contador_bandera]

	xor eax, eax
    xor edx, edx
   	xor ebx, ebx

	call sched_proximo_indice
	
	;xchg bx, bx
	
    mov bx, ax
   	sub bx, 25
    shr bx, 1
	
	push eax
	push edx
	push ebx       
    call actualizar_relojes
    pop ebx
    pop edx
    pop eax

    inc bx

    ;xchg bx, bx

    cmp WORD bx, [ultima_tarea_ejecutada]
    je .noJump

    mov ecx, 8
    mul ecx	 	;eax * 8
    mov [selector], ax
    call fin_intr_pic1

    mov WORD [ultima_tarea_ejecutada], bx
    jmp far [offset]

    jmp .end



.imprimir_banderas:
	call fin_intr_pic1
	xor eax, eax
	xor ebx, ebx
	call sched_proxima_bandera
	cmp WORD ax, -1
	je .resetear_contador_bandera

	
	mov bx, ax
	sub bx, 26
	shr bx, 1
	
	shl ax, 3
	mov [selector], ax
		
	jmp far [offset]
	
	jmp .end


.resetear_contador_bandera:
	mov dword [contador_bandera], 0
	call renovar_banderas
	jmp .seguir


.noJump:
	call fin_intr_pic1
	jmp .end


.desalojar_tarea:

	call fin_intr_pic1
	cli
	xor eax, eax
	call indice_arreglo_tss_bandera_actual
	push eax
	call desalojar_tarea
	add esp, 4

	xor eax, eax
	call indice_arreglo_tss_bandera_actual	;indice en el arreglo == id_tarea

	push DWORD [tiempo_excedido]
	push eax
	call imprimir_interrupcion
	add esp, 8

	pushear_todo
	call imprimir_ultimo_problema
	add esp, 68

	sti

	jmp 0xC0:0

	jmp .end


.end:
	popad
		
	iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:

	cli
	pushad
	call fin_intr_pic1
		
	xor eax, eax
	in al, puerto_teclado
	
	push eax
	call interrupcion_teclado
	add esp, 4
	
	popad
	sti
	iret


%define servicio_fondear 0x923
%define servicio_canonear 0x83A
%define servicio_navegar 0xAEF
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr80
global _isr102

;int 0x50
_isr80:


	cli
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	;Chequeo si no me llamo una bandera. Si es asi, la desalojo junto al codigo
	
	xor edi, edi
	xor edx, edx
	xor eax, eax
	
	
		
	call indice_gdt_tarea_actual

	mov esi, 2
	div esi
	;Si el indice es par, la interrupcion se llamo desde una funcion bandera.
	cmp WORD dx, 0
	je .desalojar_tarea

	pop edx
	pop ecx
	pop ebx
	pop eax
	
	cmp DWORD eax, servicio_fondear
	je .fondear

	cmp DWORD eax, servicio_canonear
	je .canonear

	cmp DWORD eax, servicio_navegar
	je .navegar

	jmp .fin

.fondear:

	push ebx
	call game_fondear
	add esp, 4
	jmp .fin


.canonear:
	
	push ecx	
	push ebx
	call game_canonear
	add esp, 8
	jmp .fin


.navegar:

	push ecx  		;dir_segunda_pag
	push ebx		;dir_primera_pag
	
;	xchg bx, bx
;	mov eax, 0xACA
	
	call game_navegar
	add esp, 8
	jmp .fin


.desalojar_tarea:
	
	;xchg bx, bx

	xor eax, eax
	call indice_arreglo_tss_bandera_actual	;indice en el arreglo == id_tarea
	push eax
	call desalojar_tarea					;desalojo codigo y bandera
	add esp, 4

	xor eax, eax
	call indice_arreglo_tss_bandera_actual	;indice en el arreglo == id_tarea

	push DWORD [syscall]
	push eax
	call imprimir_interrupcion
	add esp, 8

	pushear_todo
	call imprimir_ultimo_problema
	add esp, 68

	add esp, 16

	jmp .fin


.fin:
	pop ebp
	sti
	jmp 0xC0:0	;jump a la idle
	;xchg bx,bx
	iret




;int 0x66
_isr102:

	cli
	push edx
	push eax	;eax = dir buffer
	push ebx
	;Chequeo si no me llamo una tarea. Si es asi, la desalojo junto a la bandera
	xor eax, eax
	xor ebx, ebx
	xor edx, edx

	call indice_gdt_tarea_actual
	
	mov bx, 2
	div bx

	;Si el indice es impar, la interrupcion se llamo desde una tarea.
	cmp WORD dx, 0
	jne .desalojar_tarea
	
	xor eax, eax
	xor edx, edx
	call indice_arreglo_tss_bandera_actual
	mov edx, eax	;edx = indice

	pop ebx
	pop eax		;eax = direccion buffer
	
	push edx
	push eax	
	call imprimir_bandera
	add esp, 8
	
;	xchg bx, bx

	jmp .fin
	



.desalojar_tarea:	
	
	;xchg bx, bx

	xor eax, eax
	call indice_arreglo_tss_tarea_actual	;indice en el arreglo == id_tarea
	push eax
	call desalojar_tarea					;desalojo codigo y bandera
	add esp, 4
	
	xor eax, eax
	call indice_arreglo_tss_tarea_actual

	push DWORD [int66]
	push eax
	call imprimir_interrupcion
	add esp, 8


	pushear_todo
	call imprimir_ultimo_problema
	add esp, 68

	add esp, 12


	pop ebx
	pop eax
	
	jmp .fin



.fin:
	pop edx
	sti
	
	jmp 0xC0:0		 	  ;Salto a la tarea idle

	iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
global proximo_reloj
proximo_reloj:
    push ebx
    push ecx

    inc DWORD [reloj_numero]
    mov ebx, [reloj_numero]
    cmp ebx, 0x4
    jl .ok
    mov DWORD [reloj_numero], 0x0
.ok:
	mov ecx, [reloj_numero]
    lea ebx, [reloj+ecx]
    ;imprimir_texto_mp ebx, 1, 0x0f, 24, 79
    mov eax, ebx

    pop ecx
    pop ebx
    
    ret


