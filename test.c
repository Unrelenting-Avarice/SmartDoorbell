#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/buttons.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/log.h"

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define MAX_TEXT_SIZE 400
#define MAX_FILE_NAME 100

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

#define NUM_ENTRIES 6
#define MAX_ENTRY_LENGTH 20



char entries[NUM_ENTRIES][MAX_ENTRY_LENGTH] = {"Clear", "Hello", "Chars", "Stars", "Flag", "Exit"};

// Makes sure to deinitialize everything before program close
void intHandler(int dummy) {
    log_info("Exiting...");

    // Resets the Screen
    display_clear(BLACK);
    display_draw_string(5, 5, "Goodbye!", &Font16, BLACK, WHITE);

    // Releases control of the display
    display_exit();
    exit(0);
}

// clearScreen
//
//
// Clears the screen to white.
void clearScreen() {
    // TODO: Put your code here
    display_clear(WHITE);
}

// drawHelloWorld
//
// Draws "Hello World!" on the display 10 times
// Make each line a different color
void drawHelloWorld() {
    // TODO: Put your code here
    display_draw_string(5, 5, "Hello World!", &Font12,
                       WHITE, BLACK);
}

// drawChars
//
// Draw chars of different values, sizes, colors, and locations onto the screen
void drawChars() {
    // TODO: Put your code here
}



// drawStars
//
// Draw at least 20 stars on the screen
// TIP: Make a function that uses display_draw_# functions to make the shape of the star,
// Then call that function at multiple locations
void drawStars() {
    // TODO: Put your code here
}

// drawFlag
//
// Use at least 5 other display_draw functions to make any flag you want.
// It must have at least 3 colors, and use 3 unique display_draw functions
void drawFlag() {
    // TODO: Put your code here
}

// draw_menu
//
// Draw the menu of the screen. It uses the entries array to create the menu, with the num_entries
// specifying how many entries are in the entries array. The selected parameter is the item in the
// menu that is selected and should be highlighted. Use BACKGROUND_COLOR, FONT_COLOR,
// SELECTED_BG_COLOR, and SELECTED_FONT_COLOR to help specify the colors of the background, font,
// select bar color, and selected text color.
void drawMenu(char entries[][MAX_ENTRY_LENGTH], int num_entries, int selected) {
    // TODO: Put your code here
}

int main(void) {

    log_info("Starting...");
    signal(SIGINT, intHandler);

    display_init();
    buttons_init();

    clearScreen();

    // Starting point for the list
    uint32_t startx = 5;
    uint32_t starty = 5;

    // Current coord
    uint32_t curx = 5;
    uint32_t cury = 5;

    // Amnt to move upand down
    uint32_t upDownFactor = 15;

    // Options list index
    int index_lst = 0;

    // Option Bar length and width

    int opBarLen = 100;

    void display_menu(uint32_t xcoord, uint32_t ycoord){
        clearScreen();

        
        // PRINT CURRENT RECTANGLE
        display_draw_rectangle(xcoord, ycoord, xcoord+opBarLen, ycoord+upDownFactor,
                            BLUE, true, 5);

        // PRINT MENU
        for (int i = 0; i < NUM_ENTRIES; i++){
            
            // PRINT CURRENT MENU OPTION W INVERTED COLORS
            if (index_lst == i){
                display_draw_string(startx, (starty + (upDownFactor * i)), entries[i], &Font12,
                    BLUE, WHITE);

            }

            // PRINT ALL OTHER OPTIONS
            else {
                display_draw_string(startx, (starty + (upDownFactor * i)), entries[i], &Font12,
                        WHITE, BLACK);
            }
        }

        
    

    }

    display_menu(curx, cury);

    while (true) {
        

        delay_ms(200);

        if (button_up() == 0) {
            // Do something upon detecting button press
            cury -= upDownFactor;
            index_lst -= 1;

            if (index_lst <= -1){
                index_lst = 5;
                cury = 5+(5*upDownFactor);
            }

            display_menu(curx, cury);

            

            while (button_up() == 0) {
                // Delay while the button is pressed to avoid repeated actions
                delay_ms(1);
            }
        }
        else if (button_down() == 0) {
            // Do something upon detecting button press
            cury += upDownFactor;
            index_lst += 1;

            if (index_lst > 5){
                index_lst = 0;
                curx = 5;
                cury = 5;
            }

            display_menu(curx, cury);

            while (button_down() == 0) {
                // Delay while the button is pressed to avoid repeated actions
                delay_ms(1);
            }
        }
        else if (button_left() == 0) {
            // Do something upon detecting button press

            while (button_left() == 0) {
                // Delay while the button is pressed to avoid repeated actions
                delay_ms(1);
            }
        }
        else if (button_right() == 0) {
            clearScreen();

            // Clear option
            if (index_lst == 0){
                delay_ms(1);
            }
            
            // Hello option
            else if (index_lst == 1){
                for (int i = 0; i <= 10; i++){
                    display_draw_string(startx, (starty + (10 * i)), "Hello World!", &Font8,
                        WHITE, ORANGE*(200*(i+1)));
                }
                
            }
            
            // Chars option
            else if (index_lst == 2){
                display_clear(WHITE);

                uint32_t randx = 0;
                uint32_t randy = 0;

                double colors[] = {BYU_NAVY,RED,YELLOW,BLACK,GREEN,ORANGE,PURPLE,ALICE_BLUE,BYU_NAVY,BYU_ROYAL};

                // Randomize x and y for placement using i

                for (int i = 1; i <= 11; i++){

                    randx = (10*i);
                    randy = (10*i);

                    char randomLetters[] = " !@#$^&*^(+!@#$^&*^(+";

                    display_draw_char(randx, randy, randomLetters[i], &Font12,
                        WHITE, colors[i]);

                }
        
            }

            // Stars option
            else if (index_lst == 3){
                display_clear(BLUE);

                uint32_t randx = 0;
                uint32_t randy = 0;
                uint32_t backColor = BLUE;
                uint32_t frontColor = YELLOW;

                // Randomize x and y for placement using i

                for (int i = 0; i <= 20; i++){

                    randx = (10*i);
                    randy = (10*i);

                    display_draw_string(randx, randy, "*", &Font12,
                        backColor, frontColor);

                    display_draw_string(randx+25, randy+25, "*", &Font12,
                        backColor, frontColor);

                }
            }

            // Flag option
            else if (index_lst == 4){

                display_clear(BLUE);

                display_draw_rectangle(0, 0, 50, 50, WHITE, true, 2);
                display_draw_rectangle(70, 0, 120, 50, WHITE, true, 2);
                display_draw_rectangle(0, 70, 50, 120, WHITE, true, 2);
                display_draw_rectangle(70, 70, 120, 120, WHITE, true, 2);

                display_draw_string(10, 10, "*", &Font16,
                       WHITE, RED);
            }

            // Exit option
            else if (index_lst == 5){

                display_clear(BLACK);
                display_draw_string(5, 5, "Goodbye!", &Font16, BLACK, WHITE);

                // Releases control of the display
                display_exit();
                exit(0);
            }

            //delay_ms(2000); // WAIT

            display_menu(curx, cury);
        }
    }

    
    return 0;
}
