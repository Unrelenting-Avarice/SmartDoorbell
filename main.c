#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/buttons.h"
#include "lib/camera.h"
#include "lib/client.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/image.h"
#include "lib/log.h"

#include <pthread.h>
#include <unistd.h>

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define NUM_ENTRIES 6
#define MAX_TEXT_SIZE 400
#define MAX_FILE_NAME 100

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

// GLOBAL VARIABLES

int SendingStatus = 0;

// Makes sure to deinitialize everything before program close
void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit();
    exit(0);
}

/*
 * Takes in a folder, reads the contents of the folder, filtering out any files that do not end with
 * .log or .bmp. This function should check to make sure the folder exists. It fills in the entries
 * array with all of the entries in the folder, up to 8 (MAX_ENTRIES). The function returns the
 * number of entries it put into the entries array.
 */
void getFileExtension(char extension[], char fileName[])
{
    uint32_t i = 0;
    uint32_t dotIndex = 0;

    while (fileName[i] != '\0')
    {
        if (fileName[i] == '.')
        {
            dotIndex = i;
            break;
        }
        i++;
    }

    i = dotIndex + 1;
    uint32_t j = 0;

    while (fileName[i] != '\0')
    {
        extension[j] = fileName[i];
        i++;
        j++;
    }

    extension[j] = '\0';
}

int get_entries(char *folder, char entries[][MAX_FILE_NAME]) {
    
    DIR *dp = opendir(folder);

    if (dp == NULL) {

        printf("FOLDER DOES NOT EXIST");
        return -1;
    }
  
    struct dirent *entry;    // Current folder object
    int count = 0;


    while ((entry = readdir(dp)) != NULL) {

        char extension[10];
        getFileExtension(extension, entry->d_name);

        if(strcmp(extension, "bmp") == 0 || strcmp(extension, "log") == 0){
            strcpy(entries[count], entry->d_name);
            count++;
        }

        if (count >= MAX_ENTRIES){
            break;
        }
        

        // if strcmp(entry, extension, )
    }

  
    closedir(dp);

    return count;

}

// Starting point for the list
uint32_t startx = 5;
uint32_t starty = 5;

// Amnt to move upand down
uint32_t upDownFactor = 15;

// Option Bar length and width

int opBarLen = 100;

// List of options, max num entries, index_list num
void draw_menu(char entries[][MAX_FILE_NAME], int num_entries, int selected) {

    display_clear(WHITE);


    // PRINT MENU
    for (int i = 0; i < num_entries; i++){

        
        // PRINT CURRENT MENU OPTION W INVERTED COLORS [ 0,0 ] [20,5]
        if (selected == i){

            display_draw_rectangle(startx , starty+(upDownFactor*i), startx+(opBarLen), starty+(upDownFactor)+(upDownFactor*i),
                    BLUE, true, 5);
            
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

    
void draw_loading_bar() {

    /* SendingStatus 
    0 - Not Sending / Complete (show nothing)
    1 - Sending (Yellow bar, 'sending')
    2 - Complete (green bar, 'complete') */

    int star_x, star_y, end_x, end_y;
    star_x = 1;
    star_y = 110;
    end_x = 28+(opBarLen);
    end_y = 125;

    if (SendingStatus == 1){
        
        display_draw_rectangle(star_x, star_y, end_x, end_y, YELLOW, true, 5); //Display Loading Screen
        display_draw_string(star_x, star_y, "Loading . . .", &Font12, YELLOW, BLACK);
    }

    if (SendingStatus == 2){ // After delay, if status is 'SUCCESS', give complete msg
        display_draw_rectangle(star_x, star_y, end_x, end_y, GREEN, true, 5);
        display_draw_string(star_x, star_y, "Complete!", &Font12, GREEN, BLACK);
    }

    if (SendingStatus == 0){
        display_draw_rectangle(1, 110, 28+(opBarLen), 125, WHITE, true, 5); // Delete msg
    }
        

}

/*
 * Displays an image or a log file. This function detects the type of file that should be draw. If
 * it is a bmp file, then it calls display_draw_image. If it is a log file, it opens the file, reads
 * 100 characters (MAX_TEXT_SIZE), and displays the text using display_draw_string. Combine folder
 * and file_name to get the complete file path.
 */
void draw_file(char *folder, char *file_name) {
    
    display_clear(WHITE);
    

    char path[30];
    sprintf(path, "%s%s" , folder, file_name);

    char extension[10];
    getFileExtension(extension, file_name);

    if (strcmp(extension, "bmp") == 0){
        display_draw_image(path);
    }

    else if (strcmp(extension, "log") == 0){
        char readFromFile[MAX_TEXT_SIZE] = "";

        FILE *fp;    // File pointer for interacting with file.
        fp = fopen(path, "r");
        fread(readFromFile, 1, MAX_TEXT_SIZE, fp);
        fclose(fp);    // Closes the file that was attached to the fp pointer

        display_draw_string(5, 5, readFromFile, &Font12, WHITE, BLACK);

    }
        
}

/*
void send_image(uint8_t *new_img_buf){
    struct thread_args *data = (struct thread_args *)arg;
    tot_count, entries, new_img_buf
*/

void *send_image(void *new_img_buf){
    
    SendingStatus = 1; // Set loading GlobalVar to 'loading'

    // SEND IMAGE TO WEBSITE -- HW ID + new_img_buf variable
    log_info("img string: %c", new_img_buf);
    log_info("img size: %d", IMG_SIZE);

    // LAB 11 - IMG to WEBSITE
    
    Config sinfo;
    sinfo.port = "2240";
    sinfo.host = "ecen224.byu.edu";
    sinfo.payload = new_img_buf;
    sinfo.payload_size = IMG_SIZE;
    sinfo.hw_id = "006FDF9FB";

    int socket = client_connect(&sinfo);
    client_send_image(socket, &sinfo);
    client_receive_response(socket);
    client_close(socket);
    
    SendingStatus = 2; // Set loading GlobalVar flag 'Complete'
    delay_ms(2000);
    SendingStatus = 0;

    free(new_img_buf);
    return NULL;

}

void drawWelcomeMenu (){
    display_draw_image("viewer/224.bmp");
    display_draw_rectangle(0, 10, 120, 30, BLACK, true, 5);
    display_draw_string(10, 10, "Welcome", &Font20,BLACK, WHITE);
}

int main(void) {
        
    signal(SIGINT, intHandler);

    log_info("Starting...");

    // Use this to fill in with entries from the directory
    //char entries[MAX_ENTRIES][MAX_FILE_NAME];

    //char entries[NUM_ENTRIES][MAX_FILE_NAME] = {"Clear", "Hello", "Chars", "Stars", "Flag", "Exit"};


    // TODO: Initialize the hardware
    display_init();
    buttons_init();

    // RUN MAIN LOOP OF PROGRAM

    while(true){

        // Captive host for starting menu

        drawWelcomeMenu();

        char password[20] = "UUDDLRLR";
        char str[20] = "";
        

        bool captive_screen = true;

        while(captive_screen){
            // Draw welcome message
            delay_ms(100); // WAIT

            if (button_up() == 0) {
                strcat(str, "U");

                while (button_up() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }

            if (button_right() == 0) {
                strcat(str, "R");

                while (button_right() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }

            if (button_down() == 0) {
                while (button_down() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
                strcat(str, "D");
            }

            if (button_left() == 0) {
                strcat(str, "L");

                while (button_left() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }

            if (button_center() == 0) {

                // Create new picture and store image
                uint8_t *new_img_buf = malloc(sizeof(uint8_t) * IMG_SIZE);
                camera_capture_data(new_img_buf, IMG_SIZE);

                // Create new file to store image
                FILE *fp = fopen("viewer/doorbell.bmp", "wb");
                fwrite(new_img_buf, sizeof(uint8_t), IMG_SIZE, fp);
                fclose(fp);

                    //Draw image
                    //Bitmap bmp;
                    //create_bmp(&bmp, new_img_buf);
                    //display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                
                    // while (true){
                    //     reset_pixel_data(&bmp);
                    //     if (button_up() == 0) {
                    //         remove_color_channel(GREEN_CHANNEL, &bmp);
                    

                    //         while (button_up() == 0) {
                    //             // Delay while the button is pressed to avoid repeated actions
                    //             delay_ms(1);
                    //         }
                    //         display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    //         delay_ms(1000);
                    //     }

                    //     else if (button_right() == 0) {
                    //         remove_color_channel(RED_CHANNEL, &bmp);
                        

                    //         while (button_right() == 0) {
                    //             // Delay while the button is pressed to avoid repeated actions
                    //             delay_ms(1);
                    //         }
                    //         display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    //     }

                    //     else if (button_down() == 0) {
                    //         or_filter(&bmp);
                        

                    //         while (button_down() == 0) {
                    //                 // Delay while the button is pressed to avoid repeated actions
                    //                 delay_ms(1);
                    //         }
                    //         display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    //     }

                    //     else if (button_left() == 0) {
                    //         remove_color_channel(BLUE_CHANNEL, &bmp);
                        

                    //         while (button_left() == 0) {
                    //             // Delay while the button is pressed to avoid repeated actions
                    //             delay_ms(1);
                    //         }
                    //         display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    //     }

                        
                pthread_t send_to_server_thread;
                pthread_create(&send_to_server_thread, NULL, &send_image, (void *) new_img_buf);

                // Clear the screen and display taking picture message for 3 seconds
                display_draw_string(37, 40, "Ding", &Font20,
                        WHITE, BLACK);
                display_draw_string(24, 60, "Dong!", &Font24,
                WHITE, BLACK);
                delay_ms(2000);

                drawWelcomeMenu();

                }

            if (strlen(str) == strlen(password)) {
                // Auto get password
                log_info("%s", str);
                if (strcmp(str, password) == 0){
                    str[0] = '\0';
                    captive_screen = false;
                }
                str[0] = '\0';
            }

        }

        // Get directory contents using get_entries function
        char entries[MAX_ENTRIES][MAX_FILE_NAME];
        int tot_count = get_entries(VIEWER_FOLDER,entries);

        draw_menu(entries, tot_count, 0);

        int selected = 0;
        bool secret_menu = true;

        while (secret_menu) {
            
            // Check if loading flag is init.
            draw_loading_bar();

            delay_ms(100); // WAIT

            if (button_up() == 0) {
                // Do something upon detecting button press
                selected -= 1;

                if (selected <= -1){
                    selected = NUM_ENTRIES - 1;
                }

                draw_menu(entries, NUM_ENTRIES, selected);


                while (button_up() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }
            
            else if (button_down() == 0) {
                // Do something upon detecting button press
                selected += 1;

                if (selected > 5){
                    selected = 0;
                }

                draw_menu(entries, NUM_ENTRIES, selected);

                while (button_down() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }

            else if (button_left() == 0) {
                // Kill program > blue screen
                secret_menu = false;

                while (button_left() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }
            }

            else if (button_right() == 0) {
                draw_file(VIEWER_FOLDER, entries[selected]);
                

                while (button_right() == 0) {
                    // Delay while the button is pressed to avoid repeated actions
                    delay_ms(1);
                }

                delay_ms(3000);

                draw_menu(entries, NUM_ENTRIES, selected);
            }

            else if (button_center() == 0) {
                
                // Clear the screen and display taking picture message for 3 seconds
                display_clear(WHITE);
                display_draw_string(5, 40, "Picture", &Font20,
                        WHITE, BLACK);
                display_draw_string(5, 60, "Time!", &Font24,
                WHITE, BLACK);
                delay_ms(1000);
                display_clear(WHITE);

                // Create new picture and store image
                uint8_t *new_img_buf = malloc(sizeof(uint8_t) * IMG_SIZE);
                camera_capture_data(new_img_buf, IMG_SIZE);

                // Create new file to store image
                FILE *fp = fopen("viewer/doorbell.bmp", "wb");
                fwrite(new_img_buf, sizeof(uint8_t), IMG_SIZE, fp);
                fclose(fp);

                //Draw image
                Bitmap bmp;
                create_bmp(&bmp, new_img_buf);
                display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                
                while (true){
                    reset_pixel_data(&bmp);
                    if (button_up() == 0) {
                        remove_color_channel(GREEN_CHANNEL, &bmp);
                

                        while (button_up() == 0) {
                            // Delay while the button is pressed to avoid repeated actions
                            delay_ms(1);
                        }
                        display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                        delay_ms(1000);
                    }

                    else if (button_right() == 0) {
                        remove_color_channel(RED_CHANNEL, &bmp);
                    

                        while (button_right() == 0) {
                            // Delay while the button is pressed to avoid repeated actions
                            delay_ms(1);
                        }
                        display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    }

                    else if (button_down() == 0) {
                        or_filter(&bmp);
                    

                        while (button_down() == 0) {
                                // Delay while the button is pressed to avoid repeated actions
                                delay_ms(1);
                        }
                        display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    }

                    else if (button_left() == 0) {
                        remove_color_channel(BLUE_CHANNEL, &bmp);
                    

                        while (button_left() == 0) {
                            // Delay while the button is pressed to avoid repeated actions
                            delay_ms(1);
                        }
                        display_draw_image_data(bmp.pxl_data, bmp.img_width, bmp.img_height);
                    }


                    else if (button_center() == 0) {
                        
                        pthread_t send_to_server_thread;
                        pthread_create(&send_to_server_thread, NULL, &send_image, (void *) new_img_buf);

                        break;

                        while (button_center() == 0) {
                            // Delay while the button is pressed to avoid repeated actions
                            delay_ms(1);
                        }
                    }
                    //free(new_img_buf);
                }
                draw_menu(entries, NUM_ENTRIES, selected);
            }
        }
    }

    display_clear(BLUE);
    return 0;
}
