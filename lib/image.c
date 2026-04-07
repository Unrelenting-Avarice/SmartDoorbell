#include <stdlib.h>
#include <string.h>

#include "image.h"

uint8_t create_bmp(Bitmap *dst, uint8_t *src) {
    int offset = 0;
    dst->img = NULL;

    // Copy in the bitmap file header
    memcpy(dst->file_header, src, BMP_FILE_HEADER_SIZE);
    offset += BMP_FILE_HEADER_SIZE;

    // Get filesize
    dst->file_size = (dst->file_header[5] << 8 * 3) | (dst->file_header[4] << 8 * 2) |
                     (dst->file_header[3] << 8 * 1) | (dst->file_header[2]);

    // Get start location of pixel data
    dst->pxl_data_offset = (dst->file_header[13] << 8 * 3) | (dst->file_header[12] << 8 * 2) |
                           (dst->file_header[11] << 8 * 1) | (dst->file_header[10]);

    // // Get DIB header data
    uint8_t dib_header_size = dst->pxl_data_offset - BMP_FILE_HEADER_SIZE;
    dst->dib_header = (uint8_t *)malloc((dib_header_size) * sizeof(uint8_t));
    memcpy(dst->dib_header, src + offset, dib_header_size);
    offset += dib_header_size;

    // Get image width
    dst->img_width = (dst->dib_header[7] << 8 * 3) | (dst->dib_header[6] << 8 * 2) |
                     (dst->dib_header[5] << 8 * 1) | (dst->dib_header[4]);

    // Get image height
    dst->img_height = (dst->dib_header[11] << 8 * 3) | (dst->dib_header[10] << 8 * 2) |
                      (dst->dib_header[9] << 8 * 1) | (dst->dib_header[8]);

    if (dst->img_height < 0) {
        dst->img_height *= -1;
    }

    // Get image data
    dst->pxl_data_size = dst->img_width * dst->img_height * 3;
    dst->pxl_data = malloc(sizeof(uint8_t) * dst->pxl_data_size);
    memcpy(dst->pxl_data, src + offset, dst->pxl_data_size);

    // Create copy of image data for reset
    dst->pxl_data_cpy = malloc(sizeof(uint8_t) * dst->pxl_data_size);
    memcpy(dst->pxl_data_cpy, dst->pxl_data, dst->pxl_data_size);

    return LOAD_SUCCESS;
}

void destroy_bmp(Bitmap *bmp) {
    free(bmp->dib_header);
    free(bmp->pxl_data);
    free(bmp->pxl_data_cpy);
}

void reset_pixel_data(Bitmap *bmp) {
    bmp->pxl_data = memcpy(bmp->pxl_data, bmp->pxl_data_cpy, bmp->pxl_data_size);
}

uint8_t *get_pxl_data(Bitmap *bmp) { return bmp->pxl_data; }

uint8_t *get_original_pxl_data(Bitmap *bmp) { return bmp->pxl_data_cpy; }

void remove_color_channel(Color color, Bitmap *bmp) {
    for (int x = 0 ; x < bmp->pxl_data_size; x += 3 ){

        uint16_t blue = 0;
        uint16_t green = 0;
        uint16_t red = 0;

        if (color == BLUE_CHANNEL){

            bmp->pxl_data[x] = blue;
            
        }

        else if (color == GREEN_CHANNEL){
            
            bmp->pxl_data[x+1] = green;
            
        }

        else if (color == RED_CHANNEL){

            bmp->pxl_data[x+2] = red;
        }


    }


}

void grayscale(Bitmap *bmp) {
    for (int x = 0 ; x < bmp->pxl_data_size; x += 3 ){

        uint16_t blue = ((bmp->pxl_data[x]) * .114);
        uint16_t green = ((bmp->pxl_data[x+1]) * .587);
        uint16_t red = ((bmp->pxl_data[x+2]) * .299);blue = blue | (blue + bmp->img_width);

        int Y = 0.299 * red + 0.587 * green + 0.114 * blue;


        bmp->pxl_data[x] = Y;
        bmp->pxl_data[x+1] = Y;
        bmp->pxl_data[x+2] = Y;


    }
}

void or_filter(Bitmap *bmp) {
    int row = bmp->img_width * 3;

    for (int x = 0 ; x < bmp->pxl_data_size; x += 3 ){

        uint16_t blue = (bmp->pxl_data_cpy[x]);
        uint16_t green = (bmp->pxl_data_cpy[x+1]);
        uint16_t red = (bmp->pxl_data_cpy[x+2]);

        // if the x pixel is in the top row
        if (x < (row)){
            blue |= (bmp->pxl_data_cpy[x+row]);
            green |= (bmp->pxl_data_cpy[x+row+1]);
            red |= (bmp->pxl_data_cpy[x+row+2]);
        }

        // if the x pixel is in the bottom row
        else if (x > bmp->pxl_data_size - row){
            blue |= (bmp->pxl_data_cpy[x-row]);
            green |= (bmp->pxl_data_cpy[x-row+1]);
            red |= (bmp->pxl_data_cpy[x-row+2]);
        }

        else{
            blue |= (bmp->pxl_data_cpy[x-row]) | (bmp->pxl_data_cpy[x+row]);
            green |= (bmp->pxl_data_cpy[x-row+1]) | (bmp->pxl_data_cpy[x+row+1]);
            red |= (bmp->pxl_data_cpy[x-row+2]) | (bmp->pxl_data_cpy[x+row+2]);
        }

        bmp->pxl_data[x] = blue;
        bmp->pxl_data[x+1] = green;
        bmp->pxl_data[x+2] = red;

    }
}