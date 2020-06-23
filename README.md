This builds on https://github.com/axmandm/sgdk-background-image, to add a title screen to our program.

This is the largest change yet, and a number of new functions are added, to simplify repetitive actions throughout.

A new function is added centeredText, to calculate the correct position horizontally for messages on screen.

```
/*Draws text in the center of the screen, along with the vertical line position*/
void centeredText(char s[], int v_pos){
	VDP_drawText(s, 20 - strlen(s)/2 ,v_pos);
}
```

This is used in conjunction with:

```
/*Define the text messages used in game, and a variable for vertical positioning*/
int v_pos;
char msg_start[12] = "Press Start\0";
char msg_author[16] = "By Damian, 2020\0";
char msg_game_over[11] = "Game Over!\0";
```

Called as:

`centeredText(msg_start, 21);`

When a message is to be displayed on screen.

Within int main(), the loop is changed, to detect if the game is running, and the title has been displayed.

Declare some new variables at the top of main.c

```
/*Game running variable - used throughout to 'switch' based on if true*/
bool game_running = FALSE;
bool title_shown = FALSE;
```

Alter int main():

```
while (game_running == FALSE)
{
  TITLE CODE
}
```
The same method is used to call the title image as in https://github.com/axmandm/sgdk-background-image.

Some code is added to clear the screen, which in future could be used to fade/wipe the screen between background images instead.

```
/*Clear the screen*/
VDP_clearPlane(BG_A, 1);
VDP_clearPlane(BG_B, 1);
VDP_clearPlane(WINDOW, 1);
```

Alter handleJoyEvent to act differently in game, and at the title screen

```
if (game_running == FALSE)
{
  TITLE BUTTON HANDLING
}
```

The main loop is critical to making this all work - breaking out the different sections of the program into voids broke a number of things, including XGM audio.

Occasionally, when there is a button press that triggers an audio stop, i.e. SND_stopPlay_XGM(); the audio will 'rush ahead' to catch up with the action taken. This can be got around by *pausing* the music instead.

`XGM_pausePlay	();`
