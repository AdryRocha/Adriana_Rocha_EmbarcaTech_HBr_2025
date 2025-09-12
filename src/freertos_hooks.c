// freertos_hooks.c - Implementações obrigatórias do FreeRTOS

#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"

// Hook de stack overflow
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void) xTask;
    (void) pcTaskName;
    printf("ERRO: Stack overflow detectado na task: %s\n", pcTaskName);
    // Em um sistema embarcado, você pode querer resetar ou entrar em modo seguro
    for(;;) {
        tight_loop_contents();
    }
}

// Função de assert chamada quando configASSERT falha
void vAssertCalled(const char *pcFile, uint32_t ulLine) {
    (void) pcFile;
    (void) ulLine;
    printf("ASSERT falhou em %s linha %lu\n", pcFile, ulLine);
    
    // Desabilita interrupções e entra em loop infinito
    taskDISABLE_INTERRUPTS();
    for(;;) {
        tight_loop_contents();
    }
}

// Implementações específicas do ARM Cortex-M0+
// Estas funções são normalmente implementadas em assembly, 
// mas para compatibilidade básica podemos usar estas versões simplificadas

__attribute__((naked)) void vStartFirstTask(void) {
    __asm volatile (
        " ldr r0, =0xE000ED08   \n" /* Use the NVIC offset register to locate the stack. */
        " ldr r0, [r0]          \n"
        " ldr r0, [r0]          \n"
        " msr msp, r0           \n" /* Set the msp back to the start of the stack. */
        " cpsie i               \n" /* Globally enable interrupts. */
        " cpsie f               \n"
        " dsb                   \n"
        " isb                   \n"
        " svc 0                 \n" /* System call to start first task. */
        " nop                   \n"
        " nop                   \n"
    );
}

__attribute__((naked)) void SVC_Handler(void) {
    __asm volatile (
        " ldr r3, =pxCurrentTCB \n" /* Restore the context. */
        " ldr r1, [r3]          \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
        " ldr r0, [r1]          \n" /* The first item in pxCurrentTCB is the task top of stack. */
        " ldmia r0!, {r4-r7}    \n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
        " mov r8, r4            \n" /* Pop the high registers. */
        " mov r9, r5            \n"
        " mov r10, r6           \n"
        " mov r11, r7           \n"
        " msr psp, r0           \n" /* Remember the new top of stack for the task. */
        " subs r0, r0, #32      \n"
        " ldmia r0!, {r4-r7}    \n"
        " mov r1, #0            \n"
        " msr basepri, r1       \n"
        " bx r14                \n"
        " nop                   \n"
    );
}

__attribute__((naked)) void PendSV_Handler(void) {
    __asm volatile (
        " mrs r0, psp           \n"
        " isb                   \n"
        " ldr r3, =pxCurrentTCB \n" /* Get the location of the current TCB. */
        " ldr r2, [r3]          \n"
        " subs r0, r0, #32      \n" /* Make space for the remaining low registers. */
        " str r0, [r2]          \n" /* Save the new top of stack. */
        " stmia r0!, {r4-r7}    \n" /* Store the low registers that are not saved automatically. */
        " mov r4, r8            \n" /* Store the high registers. */
        " mov r5, r9            \n"
        " mov r6, r10           \n"
        " mov r7, r11           \n"
        " stmia r0!, {r4-r7}    \n"
        " push {r3, r14}        \n"
        " cpsid i               \n"
        " bl vTaskSwitchContext \n"
        " cpsie i               \n"
        " pop {r2, r3}          \n"
        " ldr r1, [r2]          \n"
        " ldr r0, [r1]          \n" /* The first item in pxCurrentTCB is the task top of stack. */
        " adds r0, r0, #16      \n" /* Move to the high registers. */
        " ldmia r0!, {r4-r7}    \n" /* Pop the high registers. */
        " mov r8, r4            \n"
        " mov r9, r5            \n"
        " mov r10, r6           \n"
        " mov r11, r7           \n"
        " msr psp, r0           \n" /* Remember the new top of stack for the task. */
        " subs r0, r0, #32      \n"
        " ldmia r0!, {r4-r7}    \n"
        " bx r3                 \n"
        " nop                   \n"
    );
}

// Funções de máscara de interrupção
uint32_t ulSetInterruptMask(void) {
    uint32_t ulReturn;
    
    __asm volatile (
        " mrs %0, basepri       \n"
        " mov r1, #0x50         \n"
        " msr basepri, r1       \n"
        : "=r" (ulReturn)
        :
        : "r1"
    );
    
    return ulReturn;
}

void vClearInterruptMask(uint32_t ulMask) {
    __asm volatile (
        " msr basepri, %0       \n"
        :
        : "r" (ulMask)
    );
}