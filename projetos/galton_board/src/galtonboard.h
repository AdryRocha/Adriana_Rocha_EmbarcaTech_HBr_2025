#include "stdbool.h"
#ifndef GALTON_H
#define GALTON_H

#define MAX_BALLS 10 // Maximum number of balls
#define SSD1306_WIDTH 128 // Width of the display
#define SSD1306_HEIGHT 64 // Height of the display
#define CHANNELS 16 // Number of channels
#define CHANNEL_WIDTH (SSD1306_WIDTH / CHANNELS) // Width of each channel

typedef struct { // Struct to represent a ball
    float x, y; // Position of the ball
    bool active; // Is the ball active?
    int collisions; // Number of collisions
} Ball;

extern int histogram[CHANNELS]; // Histogram to count balls in each channel
extern int total_balls; // Total number of balls
extern float left_prob; // Probability of going left
extern float right_prob; // Probability of going right

// Funções de aleatoriedade
uint32_t get_random_32(void); // Função para obter um número aleatório de 32 bits
bool random_direction(void); // Função para determinar a direção aleatória (esquerda ou direita)

void test_randomness(int trials); // Test the randomness of the ball's direction
void calculate_statistics(); // Calculate statistics of the histogram
void init_ball(Ball *ball); // Initialize a ball
void update_ball(Ball *ball); // Update the position of a ball
void register_ball_landing(Ball *ball); // Register the landing of a ball
float get_left_probability(); // Get the left probability
float get_right_probability(); // Get the right probability

#endif