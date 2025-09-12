#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// Hook chamado quando há overflow de stack
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("STACK OVERFLOW in task: %s\n", pcTaskName);
    // Em produção, você pode implementar reset do sistema
    while(1) {
        // Loop infinito para debug
    }
}

// Função de assert personalizada do FreeRTOS
void vAssertCalled(const char *pcFile, unsigned long ulLine)
{
    printf("ASSERT FAILED: %s:%lu\n", pcFile, ulLine);
    // Desabilita interrupções
    taskDISABLE_INTERRUPTS();
    while(1) {
        // Loop infinito para debug
    }
}

// Hook chamado quando o sistema está idle (opcional)
void vApplicationIdleHook(void)
{
    // Pode ser usado para colocar o sistema em sleep
}

// Hook chamado a cada tick do sistema (opcional)
void vApplicationTickHook(void)
{
    // Pode ser usado para tarefas periódicas rápidas
}

// Hook para malloc falhou (opcional)
void vApplicationMallocFailedHook(void)
{
    printf("MALLOC FAILED!\n");
    taskDISABLE_INTERRUPTS();
    while(1) {
        // Loop infinito
    }
}