/*
 * FreeRTOS V202111.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include "pico.h"

/*-----------------------------------------------------------
 * Definições específicas da aplicação
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      ( 125000000UL )

#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
/* Back-compat para lwIP */
#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               8
#define configUSE_QUEUE_SETS                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TASK_NOTIFICATION_ARRAY_ENTRIES  3

/* Define to trap errors during development. */
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* Hooks */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* System */
#define configUSE_TASK_FPU_SUPPORT              0

/* IMPORTANT: Configuração necessária para lwIP */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 4

/* Stats/trace */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Co-routines */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         2

/* Timers de software */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            1024

/* Interrupções (Cortex-M0+ não usa prioridades NVIC em FreeRTOS) */
#define configKERNEL_INTERRUPT_PRIORITY         0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    0
#define configMAX_API_CALL_INTERRUPT_PRIORITY   0

/* SMP do port RP2040 (usaremos 1 core com FreeRTOS) */
#define configNUM_CORES                         1
#define configTICK_CORE                         0
#define configRUN_MULTIPLE_PRIORITIES           0
#define configUSE_CORE_AFFINITY                 0
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* ======= REQUERIDOS PELO PORT ======= */
/* RP2040 (Cortex-M0+) não tem MPU/FPU */
#define configENABLE_MPU                        0
#define configENABLE_FPU                        0

/* Assert */
void vAssertCalled( const char * pcFile, unsigned long ulLine );
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* ----------------------------------------------------------------
   Compat: async_context_freertos.c precisa de portCHECK_IF_IN_ISR()
   Implementamos sem cmsis_gcc.h, lendo o IPSR diretamente.
   ---------------------------------------------------------------- */
#if !defined(portCHECK_IF_IN_ISR)
static inline uint32_t freertos_read_ipsr(void) {
    uint32_t r;
    __asm volatile ("MRS %0, ipsr" : "=r"(r) :: );
    return r;
}
#define portCHECK_IF_IN_ISR() ( freertos_read_ipsr() != 0u )
#endif

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_uxTaskGetStackHighWaterMark2    0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* A header file that defines trace macro can be included here. */

/* Pico specific */
#define configSUPPORT_PICO_SYNC_INTEROP         1
#define configSUPPORT_PICO_TIME_INTEROP         1

/* LWIP compatibility defines */
#define configUSE_NEWLIB_REENTRANT              0
#define configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H 0

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP 0
/* Tamanho do heap usado por heap_4.c */
#ifndef configTOTAL_HEAP_SIZE
  /* 64 KB de heap. Ajuste conforme a necessidade do seu projeto. */
  #define configTOTAL_HEAP_SIZE              ( ( size_t ) ( 128 * 1024 ) )
#endif

/* Mutexes e APIs de consulta do dono do mutex (usadas por async_context_freertos) */

  

#ifndef INCLUDE_xSemaphoreGetMutexHolder
  #define INCLUDE_xSemaphoreGetMutexHolder         1
#endif
#ifndef INCLUDE_xSemaphoreGetMutexHolderFromISR
  #define INCLUDE_xSemaphoreGetMutexHolderFromISR  1
#endif

/* Habilita ponteiros de TLS (necessário para lwIP com NETCONN_SEM_PER_THREAD) */
#ifndef configNUM_THREAD_LOCAL_STORAGE_POINTERS
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    1   /* 1 já resolve; 5 dá folga */
#endif

#endif /* FREERTOS_CONFIG_H */
