#ifndef GRAPHICS_H
#define GRAPHICS_H

struct circ;

void circb(double in_x, double in_y, double radius);
void draw_circles(struct circ** list, int shake_amount);
void draw_player_circle(int player_angle, int player_radius);

#endif