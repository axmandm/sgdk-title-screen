#include <genesis.h>
#include <string.h>
#include "sound.h"
#include "font.h"
#include "gfx.h"

/*The variable for the SFX used must be defined*/
#define GAME_OVER   65

/*Timer variables*/
int timer = 3;
char label_timer[6] = "TIMER\0";
char str_timer[3] = "0";

/*String handling*/
int sign(int x) {
    return (x > 0) - (x < 0);
}

/*Update the timer on screen*/
void updateTimer(){
  /*Convert the int to a char*/
	sprintf(str_timer,"%d",timer);

  /*Remove the text on screen*/
	VDP_clearText(1,2,3);

  /*Draw the timer at 1,2*/
	VDP_drawText(str_timer,1,2);
}

/*Define the text messages used in game, and a variable for vertical position*/
int v_pos;
char msg_start[12] = "Press Start\0";
char msg_author[16] = "By Damian, 2020\0";
char msg_game_over[11] = "Game Over!\0";

/*Game running variable - used throughout to 'switch' based on if true*/
bool game_running = FALSE;
bool title_shown = FALSE;

/*Joypad handling function*/
void handleJoyEvent(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if (game_running == TRUE)
        {
          if (changed & ~state & BUTTON_START)
          {
              timer = 3;
              title_shown = FALSE;
              game_running = FALSE;

              SYS_reset();
          }
          if (changed & ~state & BUTTON_A)
          {
              timer = timer + 3;
              if (SND_isPlaying_XGM())
                    SND_stopPlay_XGM();
                else
                    SND_startPlay_XGM(title_music);
          }
        }

        if (game_running == FALSE)
        {
          if (changed & ~state & BUTTON_START)
          {
              XGM_pausePlay	();
              game_running = TRUE;
          }
          if (changed & ~state & BUTTON_A)
          {
            if (SND_isPlaying_XGM())
                  SND_stopPlay_XGM();
              else
                  SND_startPlay_XGM(title_music);
          }
        }
    }
}

/*Draws text in the center of the screen, along with the vertical line position*/
void centeredText(char s[], int v_pos){
	VDP_drawText(s, 20 - strlen(s)/2 ,v_pos);
}

int main()
{

  /*Main Loop*/
  while(TRUE)
  {

    JOY_setEventHandler(handleJoyEvent);

    while (game_running == FALSE)
    {

      if (title_shown == FALSE)
      {
        /*Clear the screen*/
        VDP_clearPlane(BG_A, 1);
        VDP_clearPlane(BG_B, 1);
        VDP_clearPlane(WINDOW, 1);

        /*Draw the background image*/
        VDP_drawImageEx(BG_B, &title, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 1, DMA);

        /*Write title screen text*/
        centeredText(msg_start, 21);
        centeredText(msg_author, 24);

        title_shown = TRUE;
      }
        if (!SND_isPlaying_XGM())
        XGM_startPlay(title_music);

    }

    if (game_running == TRUE)
    {

      /*Clear the screen*/
      VDP_clearPlane(BG_A, 1);
      VDP_clearPlane(BG_B, 1);
      VDP_clearPlane(WINDOW, 1);

      /*Load the custom font, and set the pallete to the colours of the font*/
      VDP_loadFont(custom_font.tileset, DMA);
      VDP_setPalette(PAL0, custom_font.palette->data);

      /*Draw the background image*/
      VDP_drawImageEx(BG_B, &background, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, 1, DMA);

      /*Start playing the music*/
      XGM_startPlay(game_music);

      /*Initialize SFX - this refers to the sound files defined in sound.h and sound.res*/
      SND_setPCM_XGM(GAME_OVER, gameover, sizeof(gameover));

      while(timer >= 1)
      {

          /*Display the timer label at 1,1*/
          VDP_drawText(label_timer, 1, 1);

          /*Call the timer display function*/
          updateTimer();

          /*Decrement the timer*/
          timer--;

          /*Wait for 1s*/
          waitMs (1000);

      }

      if(timer >=0)
      {

        /*Remove the scoreboard*/
        /*Clear the screen*/
        VDP_clearPlane(BG_A, 1);
        VDP_clearPlane(BG_B, 1);
        VDP_clearPlane(WINDOW, 1);

        //VDP_clearTextArea(1,1,10,10);

        /*Stop the music*/
        XGM_stopPlay();

        /*Game Over Text*/
        centeredText(msg_game_over, 11);
        XGM_startPlayPCM(GAME_OVER, 1, SOUND_PCM_CH2);
        waitMs(3000);
        XGM_startPlay(game_over);

      }

    }

    return 0;

  }

  /*Wait vsync - always last - if you don't wait for vsync it flickers*/
  VDP_waitVSync();

  return 0;

}
