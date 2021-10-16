#ifndef CIRCLE_H
#define CIRCLE_H

struct circ {
    double x, y, radius;
    int rotate, type, angle;
    struct circ *next;
    struct circ *prev;
};

void add_circle(struct circ** list, int x, int y);
void update_circles(struct circ** list, double interval);
void seed_circles(struct circ** list, int* x, int* y, double* angle, double* interval);
void check_collisions(struct circ** list, double player_angle, int* lives, int* score, int *shake, double *interval);

#endif