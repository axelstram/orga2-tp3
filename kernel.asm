; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include'imprimir.mac'


global start
global pantalla_activa




;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern deshabilitar_pic
extern resetear_pic
extern habilitar_pic

;; PANTALLA
extern limpiar_pantalla
extern pintar_mapa
extern pintar_estado
extern imprimir_nombre_grupo
extern imprimir_en_pantalla
extern iniciar_barcos_y_banderas
extern imprimir_relojes_tareas_y_banderas

;; PAGINACION
extern mmu_inicializar_dir_kernel
extern mmu_inicializar
extern mmu_inicializar_dir_tarea


;; TAREAS
extern inicializar_tareas
extern inicializar_tarea_idle
extern sched_inicializar
extern tss_inicializar_tarea_idle

;; MACROS
%macro cargar_page_directory_kernel_en_cr3 0

	mov eax, 0x28000 
    mov cr3, eax
	
%endmacro


%macro habilitar_paginacion 0
	
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	
%endmacro

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;
;; Punto de entrada del kernel.
BITS 16
start:

    ; Deshabilitar interrupciones
    cli

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; habilitar A20
    call habilitar_A20
    
    ; cargar la GDT
    lgdt [GDT_DESC]

    ; setear el bit PE del registro CR0
    mov eax, CR0
    or eax, 1
    mov cr0, eax

	jmp 0x90:mp                ;Cambio el CS para que apunte a la posicion 18 (18*8 = 0x90) de la GDT (CS_KERNEL)
    ;pasar a modo protegido
BITS 32

mp:
    ;acomodar los segmentos

    xor eax, eax
    mov ax, 00010011000b        ;posicion 19 de la GDT (DS_KERNEL)
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov fs, ax

    ; setear la pila
    
    mov esp, 0x27000
    
    call limpiar_pantalla
    

;------------------------------------------------------------


    call idt_inicializar

    lidt [IDT_DESC]
	
	call deshabilitar_pic
	call resetear_pic
	call habilitar_pic

	;Testeo una excepción
;	mov eax, 1
;	mov ebx, 0
;	div ebx
;------------------------------------------------------------
    ; inicializar el directorio de paginas
pantalla_activa: dd 0

    call mmu_inicializar

	cargar_page_directory_kernel_en_cr3

	; habilitar paginacion
	habilitar_paginacion

	call pintar_mapa  
	call pintar_estado
	call imprimir_nombre_grupo
    call iniciar_barcos_y_banderas
    call imprimir_relojes_tareas_y_banderas
	
	call sched_inicializar
		
	mov eax, 0xB8
	ltr ax		  		  ;Cargo tarea inicial
	
	jmp 0xC0:0		 	  ;Salto a la tarea idle
	


    ; Ciclar infinitamente (por si algo sale mal...)
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
