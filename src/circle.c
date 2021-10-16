#include "circle.h"
#include "wasm4.h"
#include <stdlib.h>
#include <math.h>

void add_circle(struct circ** list, int x, int y)
{
    double a = rand() % 360;
    int type = 0;

    int r = rand() % 100;
    if(r < 70)
    {
        type = 1;
    } else 
    {
        type = 2;
    }

    struct circ *new_circle = malloc(sizeof (struct circ));
    
    if(new_circle != NULL)
    {
        new_circle->x = x;
        new_circle->y = y;
        new_circle->radius = 1;
        new_circle->rotate = (rand() % 6) - 3;
        new_circle->angle = rand() % 360;
        new_circle->type = type;

        new_circle->next = NULL;
        new_circle->prev = NULL;

        if(*list == NULL)
        {
            *list = new_circle;

        } else {
            struct circ *tail = *list;
            while(tail->next)
            {
                tail = tail->next;
            }
            if(tail)
            {
                new_circle->prev = tail;
                tail->next = new_circle;
            }
        }   
    }
    return; 
}

void update_circles(struct circ** list, double interval)
{
    struct circ *current = *list;
    int i = 0;
    int end = 0;
    while(current)
    {
        current = current->next; 
       ++i;
    }
    current = *list;

    while(current)
    {
        if(current->radius > 140)
        {
            if(current->prev != 0)
            {
                current->prev->next = current->next;
                struct circ *prev = current->prev;
                free(current);
                current = prev;

            } else {
                struct circ *next = current->next;
                *list = next;
                free(current);
                current = next;
                current->prev = NULL;
            }
        } else {
            current->radius += interval*current->radius;
            current = current->next;
        }
    }
}


void seed_circles(struct circ** list, int* x, int* y, double* angle, double* interval)
{
    for(int i = 0; i <= 150; ++i)
    {
        *angle += 0.1;
        *x = 100.0*sin(*angle * M_PI/180.0) * cos(*angle * M_PI/180.0);
        *y = 100.0*sin(*angle * M_PI/180.0) * sin(*angle * M_PI/180.0);
        if(i%10==0)
        {
            //add_circle();
            add_circle(list,*x,*y);
        }
        update_circles(list,*interval);
    }

}

void check_collisions(struct circ** list, double player_angle, int* lives, int* score, int *shake, double *interval)
{
    struct circ* collide = NULL;
    struct circ *current = *list;
    int i = 0;

    while(current->next)
    {
        current = current->next; 
    }
    while(current)
    {
        if(current->radius > 65)
        {
            collide = current;
            break;
        }
        current = current->prev;
        i++;
    }
    
    if (collide)
    {
        int player_360 = player_angle - 360 * floor(player_angle/360);
        int circle_360 = collide->angle - 360 * floor(collide->angle/360);

        if(abs(player_360 - circle_360) < 10 && collide->type != -1)
        {
            if(collide->type == 1)
            {
                --(*lives);
                *shake = 20;
                tone(250 | (150 << 16),2 | (20 << 8), 80, TONE_PULSE2);
            }
            if(collide->type == 2)
            {
                *interval += 0.001;
                ++(*score);
                if(*score % 10 == 0)
                {
                    ++(*lives);
                    tone(500 | 600 << 16,2 | (20 << 8), 80, TONE_PULSE2);

                } else
                {
                    tone(500 | 800 << 16,2 | (20 << 8), 80, TONE_PULSE2);
                }
            }
            collide->type = -1;

        }
    }
}
