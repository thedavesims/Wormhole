#include "wasm4.h"
int freqs[] = {61,93,129,149,191,240,293};
int notes[] = {0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,1,6,5,4,3,2,1,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,1,6,5,4,3,2,1};

void music_beat(int *beat, int *current_note)
{
    ++(*beat);

    if (*beat % 80 == 0)
    {
        tone(500 , 2 | (20 << 8), 80, TONE_NOISE);
    } else if(*beat % 40 == 0)
    {
        tone(250,2 | (20 << 8), 80, TONE_NOISE);
    }
    if(*beat % 10 == 0) 
    {
        int note = notes[*current_note];
        int freq;
        if (note != -1)
        {
            freq = freqs[notes[*current_note]];
            tone(freq, 8 | (2 << 10), 100, TONE_PULSE1);
        }
        

        ++(*current_note);
        *current_note %= 64;
    }
}
