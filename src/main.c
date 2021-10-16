#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "utilities.h"
#include "wasm4.h"
#include "images.h"
#include "music.h"
#include "circle.h"
#include "graphics.h"

#define SHAKE 8

void update () 
{
    static struct circ *circle_list = NULL;

    static int t = 0;
    static double angle = 0.0f;
    static int x = 0, y = 0;
    static double interval = 0.01f;
    static int beat = 0;
    static int current_note = 0;
    static double player_angle = 0.0f;
    static int player_radius = 100;
    static int score = 0;
    static int lives = 3;
    static int shake = 0;
    static int shake_amount = 0;
    static int state = 0;
    static int game_over_y = -30;

    uint8_t gamepad = *GAMEPAD1;
    static int draw_start = 0;

    music_beat(&beat,&current_note);

    if (state == 0) //title screen
    {
        *DRAW_COLORS = 0x4320;
        blit((const uint8_t*)wormhole, 0, 0, wormholeWidth, wormholeHeight, wormholeFlags);
        *DRAW_COLORS = 0x3;

        if(t%30 == 0)
        {
            draw_start = !draw_start;
            tracef("%d",draw_start);
        }

        if(draw_start)
        {
            text("PRESS START",80-(11*8)/2,110);
        }

        *DRAW_COLORS = 0x3;
        text("C on WASM-4",80-(11*8)/2,135);
        text("DMG84-GBJAM9-2021",80-(17*8)/2,144);

        if (gamepad && t > 30) 
        {
            state = 1;
            t = 0;
            seed_circles(&circle_list,&x,&y,&angle,&interval);
 
        }

    } else if (state == 1) //gameplay
    {
        //update
        if(player_radius > 5 && t % 2 == 0)
        {
            player_radius--;
        }
        if(lives == 0)
        {
            state = 2;
            t = 0;
            return;
        }
        char output[255];
        
        if(shake > 0)
        {
            --shake;
            if(shake == 0)
            {
                shake_amount = 0;
            } else {
                shake_amount = rand()%SHAKE - SHAKE/2;
            }
        }

        if (gamepad & BUTTON_LEFT) {
            player_angle -= 2;
        } else if(gamepad & BUTTON_RIGHT) {
            player_angle += 2;
        }

        check_collisions(&circle_list,player_angle,&lives,&score,&shake,&interval);

        angle += + 0.1;
        x = 100.0*sin(angle * M_PI/180.0) * cos(angle * M_PI/180.0);
        y = 100.0*sin(angle * M_PI/180.0) * sin(angle * M_PI/180.0);

        if(t % 10 == 0)
        {
            add_circle(&circle_list,x,y);
        }

        update_circles(&circle_list,interval);

        //draw 
        *DRAW_COLORS = 0x20;
        draw_circles(&circle_list,shake_amount);
        *DRAW_COLORS = 0x40;
        draw_player_circle(player_angle,player_radius);

        *DRAW_COLORS = 0x04;
        text("Score: ",1,1);
        text(inttostr(score),49,1);
        char* lives_s = inttostr(lives);
        int lives_x =  160 - strlen(lives_s) * 8;
        text("Lives: ", lives_x-(6*8),1);
        text(lives_s,lives_x,1);

    }
    else if (state == 2) //gameover
    {
       *DRAW_COLORS = 0x20;
        draw_circles(&circle_list,shake_amount);
        
        if(game_over_y < 40)
        {
            game_over_y++;
        }

        *DRAW_COLORS = 0x4320;
        blit((const uint8_t*)gameover, 80-gameoverWidth/2, game_over_y, gameoverWidth, gameoverHeight, gameoverFlags);

        *DRAW_COLORS = 0x2;
        rect(0,99,160,9);
        *DRAW_COLORS = 0x4;
        text("You scored:",80-(11*8)/2, 100);
        *DRAW_COLORS = 0x2;
        rect(0,119,160,9);
        *DRAW_COLORS = 0x4;
        char* score_s = inttostr(score);
        text(score_s,80-(strlen(score_s)*8)/2,120);

        if (gamepad && t > 60) 
        {

            state = 0;
            t = 0;
            angle = 0;
            x = 0;
            y = 0;
            interval = 0.01f;
            current_note = 0;
            player_angle = 0;
            player_radius = 100;
            score = 0;
            lives = 3;
            shake = 0;
            shake_amount = 0;
            state = 0;
            game_over_y = -30;

            struct circ* current = circle_list;

            while(current)
            {
                struct circ* temp = current;
                current = current->next;
                free(temp);
            }
            circle_list = NULL;
            seed_circles(&circle_list,&x,&y,&angle,&interval);
        }
    }
     ++t;
}
