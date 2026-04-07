#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "camera.h"
#include "log.h"

void camera_capture_data(uint8_t *buf, size_t bufsize) {
    char tmp_file_name[16] = "ecen-224-XXXXXX";
    char *command = "rpicam-still -n --immediate -e bmp --width 128 --height 128 -o ";

    // Create the temporary file
    int fp = mkstemp(tmp_file_name);

    // Create whole command
    ssize_t size = snprintf(NULL, 0, "%s%s", command, tmp_file_name);
    char *full_command = malloc(size + 1);
    snprintf(full_command, size + 1, "%s%s", command, tmp_file_name);

    // Run the command
    system(full_command);

    // Copy the data out of the file
    read(fp, buf, bufsize);

    // Clean up
    close(fp);
    remove(tmp_file_name);
    free(full_command);
}

void camera_save_to_file(uint8_t *buf, size_t bufsize, char *filename) {
    /*
    YOUR CODE HERE
    */
    uint8_t *my_new_buf = malloc(sizeof(uint8_t) * IMG_SIZE);

    // Whenever we are done with a malloc'd buffer in C, don't forget to call the following line:
    free(my_new_buf);

}

/*
 * Takes image data *with* the BMP header and saves it to a file.
 *
 * uint8_t * buf: a buffer where the image data of the photo taken is stored
 * size_t bufsize: integer that holds the size of the buffer. The size of the photo
 *                 being taken is found as a #DEFINEd variable in "camera.h"
 * char * filename: name of the file that is being saved
 */
