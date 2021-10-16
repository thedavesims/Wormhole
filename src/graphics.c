#include <math.h>
#include <stdlib.h>
#include "wasm4.h"
#include "circle.h"

void circb(double in_x, double in_y, double radius)
{
    int x = in_x - radius;
    int y = in_y - radius;
    oval(x,y,radius*2,radius*2);
}

void draw_circles(struct circ** list, int shake_amount )
{
    struct circ *current = *list;
    int c_x = 0, c_y = 0;
    c_x = 80-current->x;
    c_y = 80-current->y;
    while(current)
    {
        if(current->radius > 65)
        {
            c_x = 80-current->x;
            c_y = 80-current->y;
        }
        current = current->next;
    }

    c_x += shake_amount;
    c_y += shake_amount;

    if(!*list)
    {
        return;
    }

    current = *list;
    while(current)
    {
        if(current->next)
        {
            current = current->next;
        } else {
            break;
        }
    }
    while(current)
    {
        *DRAW_COLORS = 0x20;
        circb(current->x+c_x,current->y+c_y,current->radius);
        int new_r = current->radius/15;
        if(current->type == 1)
        {
            current->angle = current->angle + current->rotate;
            current->angle %= 360;
        }
        if(new_r < 1)
        {
            new_r = 1;
        }

        if(current->type == 0)
        {
            *DRAW_COLORS = 0x21;
            circb(c_x + current->x + current->radius*cos(current->angle*M_PI/180.0),c_y + current->y + current->radius*sin(current->angle * M_PI/180.0),new_r);
        
        } else if(current->type == 1)
        {
            *DRAW_COLORS = 0x40;
            circb(c_x + current->x + current->radius*cos(current->angle*M_PI/180.0),c_y + current->y + current->radius*sin(current->angle * M_PI/180.0),new_r);
            for(int i = (int)new_r; i > 0; i--)
            {
                circb(c_x + current->x + current->radius*cos(current->angle*M_PI/180.0),c_y + current->y + current->radius*sin(current->angle * M_PI/180.0),i);
            }
        } else if (current->type == 2)
        {
             *DRAW_COLORS = 0x32;
            circb(c_x + current->x + current->radius*cos(current->angle*M_PI/180.0),c_y + current->y + current->radius*sin(current->angle * M_PI/180.0),new_r);
        }
        current = current->prev;
    }

}

void draw_player_circle(int player_angle, int player_radius)
{
    circb(80 + 65 * cos(M_PI/180.0 * (player_angle)),80 + 65 * sin(M_PI/180.0 * (player_angle)),player_radius);
}