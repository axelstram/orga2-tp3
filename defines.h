/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_TAREAS             8

#define TAMANO_PAGINA           0x00001000

#define TASK_SIZE               2 * 4096


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CS_KERNEL			18
#define GDT_IDX_DS_KERNEL			19
#define GDT_IDX_CS_USER				20
#define GDT_IDX_DS_USER				21
#define GDT_IDX_AREA_PANTALLA		22
#define GDT_IDX_TAREA_INICIAL		23
#define GDT_IDX_TAREA_IDLE			24
#define GDT_IDX_TAREA1_CODIGO		25
#define GDT_IDX_TAREA1_BANDERA      26
#define GDT_IDX_TAREA2_CODIGO		27
#define GDT_IDX_TAREA2_BANDERA      28
#define GDT_IDX_TAREA3_CODIGO		29
#define GDT_IDX_TAREA3_BANDERA      30
#define GDT_IDX_TAREA4_CODIGO		31
#define GDT_IDX_TAREA4_BANDERA      32
#define GDT_IDX_TAREA5_CODIGO		33
#define GDT_IDX_TAREA5_BANDERA      34
#define GDT_IDX_TAREA6_CODIGO		35
#define GDT_IDX_TAREA6_BANDERA      36
#define GDT_IDX_TAREA7_CODIGO		37
#define GDT_IDX_TAREA7_BANDERA      38
#define GDT_IDX_TAREA8_CODIGO		39
#define GDT_IDX_TAREA8_BANDERA      40


/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE1              0x40000000 /* direccion virtual del codigo */
#define TASK_CODE2				0x40001000 

#define TASK_IDLE_CODE          0x40000000 /* direccion virtual del codigo de la tarea idle */
#define TASK_IDLE_STACK         0x003D0000 /* direccion virtual de la pila de la tarea idle */

#define TASK_ANCLA              0x40002000
#define TASK_ANCLA_FIS          0x00000000

#define AREA_TIERRA_INICIO      0x00000000  /* 0.0 MB     */
#define AREA_TIERRA_FIN         0x000FFFFF  /* 1.0 MB - 1 */
#define AREA_MAR_INICIO         0x00100000  /* 1.0 MB     */
#define AREA_MAR_FIN            0x0077FFFF  /* 7.5 MB - 1 */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_1_CODE_SRC_ADDR    0x00010000
#define TASK_2_CODE_SRC_ADDR    0x00012000
#define TAKS_3_CODE_SRC_ADDR    0x00014000
#define TASK_4_CODE_SRC_ADDR    0x00016000
#define TASK_5_CODE_SRC_ADDR    0x00018000
#define TASK_6_CODE_SRC_ADDR    0x0001A000
#define TASK_7_CODE_SRC_ADDR    0x0001C000
#define TASK_8_CODE_SRC_ADDR    0x0001E000

#define TASK_IDLE_CODE_SRC_ADDR 0x00020000

#endif  /* !__DEFINES_H__ */
