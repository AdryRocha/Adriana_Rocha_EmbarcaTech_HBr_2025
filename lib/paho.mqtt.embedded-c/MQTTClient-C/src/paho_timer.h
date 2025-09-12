#ifndef PAHO_TIMER_H_LOCAL
#define PAHO_TIMER_H_LOCAL

// Definição local de Timer sem dependências externas
typedef struct Timer {
    unsigned long end_time;
} Timer;

// Declarações das funções (implementadas externamente)
void TimerInit(Timer* timer);
char TimerIsExpired(Timer* timer);
void TimerCountdownMS(Timer* timer, unsigned int ms);
void TimerCountdown(Timer* timer, unsigned int seconds);
int TimerLeftMS(Timer* timer);

#endif // PAHO_TIMER_H_