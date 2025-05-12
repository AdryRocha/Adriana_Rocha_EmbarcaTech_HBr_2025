#ifndef DISPLAY_H
#define DISPLAY_H

#include"galtonboard.h"

//--------------------------------------------------------
// FUNCTION PROTOTYPES
//--------------------------------------------------------
void init_display(); // Initialize the display
void clear_display_buffer(); // Clear the display buffer
void ssd1306_update_display(); // Update the display with the current buffer
void ssd1306_setup(); // Setup the display with initial commands
void draw_histogram(int *histogram); // Draw the histogram on the display
void draw_ball(Ball *ball); // Draw a ball on the display
void update_display(Ball *balls, int ball_count, int *histogram); // Update the display with the current balls and histogram
void draw_probabilities(float left_prob); // Draw the left and right probabilities on the display

#endif