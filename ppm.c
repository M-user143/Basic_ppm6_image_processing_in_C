#include "ppm.h"        //A tradition that this is my own file from the currect directory using "".
#include <stdlib.h>     //<> indicates a Standard Library of C.
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
//#include <sys/mman.h>

/*
ppmimage *ppm_readimage(const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file){
        perror("Failed to open file"); //Error handling if file cannot be opened.
        return NULL;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);           // Move the file pointer to the end of the file
    size_t filesize = ftell(file);      // Get the file size

    //Get file Descriptor.
    int file_descriptor = fileno(file); // Get the file descriptor from the file pointer because we need it for mmap.
    if (file_descriptor < 0) {
        perror("Failed to get file descriptor");
        fclose(file);
        return NULL;
    }

    // Go back to the start of the file
    rewind(file);   //The rewind function in C++ resets the position of the file pointer associated with the given FILE stream (_File) to the beginning of the file. 
                    //It also clears any error indicators and the end-of-file indicator for the stream.


    ppmimage *result = malloc(sizeof(ppmimage)); // Allocate memory for the ppmimage struct

    // Read the header
    char header[3];     // Buffer to store the header of 2 characters + the Null terminator
    fscanf(file, "%2s", header); // Read the first two characters (P6 for binary PPM)
    if (strcmp(header, "P6") != 0) { // Check if the header is P6      //strcmp compares two strings character by character and returns 0 if they are equal or the difference betweem the first Non_same ASCII.
        fprintf(stderr, "Unsupported PPM format: %s\n", header);
        free(result);   // Free the allocated memory for ppmimage
        fclose(file);   // Close the file
        return NULL;
    }

    // Read the image dimensions and max color value
    fscanf(file, "%u %u %u", &result->cols, &result-> rows, &result->maxval);   // Read the columns, rows, and max color value and %u is format specifier for unsigned int.
    

    off_t offset = ftell(file); // Get the current position in the file after reading the header

    uint8_t *raw_data = malloc(filesize - offset); // Allocate memory for the raw pixel data

    if (!raw_data) {
        perror("Failed to allocate memory for raw data");
        free(result);   // Free the allocated memory for ppmimage
        fclose(file);   // Close the file
        return NULL;
    }

    // how many bytes per color channel.
    size_t bytes_per_pixel = 3; // P6 format uses 3 bytes per pixel (RGB)
    size_t num_pixels = (filesize - offset) / bytes_per_pixel; // Calculate the number of pixels

    uint8_t *pixel_data = raw_data + offset + 1; // Pointer to the raw pixel data
    
    result->pixels = calloc(result->rows , sizeof(pixel *)); // Allocate memory for the pixel rows.The calloc function in C++ allocates memory for an array of _NumOfElements elements, each of size _SizeOfElements, and initializes all bytes in the allocated memory to zero. It returns a pointer to the allocated memory or nullptr if the allocation fails.

    for (unsigned int i = 0; i < result->rows; i++) {
        result->pixels[i] = calloc(result->cols , sizeof(pixel)); // Allocate memory for each row of pixels
        for (int x=0; x < result->cols; x++) {
            uint8_t *pixel_start = pixel_data + (i * result->cols + x) * bytes_per_pixel; // Calculate the starting address of the pixel

            result->pixels[i][x].r = pixel_start[0]; // Assign the red channel
            result->pixels[i][x].g = pixel_start[1]; // Assign the green channel
            result->pixels[i][x].b = pixel_start[2]; // Assign the blue channel 


        }

    }

    return result; // Return the pointer to the ppmimage struct containing the image data


}


void ppm_destroy(ppmimage *image){
    for (unsigned int i = 0; i < image->rows; i++) {
        free(image->pixels[i]); // Free each row of pixels
    }

    free(image); // Free the image struct itself
}

bool ppm_writeimage(const char *filename, ppmimage *image){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return false;
    }

    // Write the PPM header
    fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);      // Dont write the binary data using fprintf, use fputc instead for binary data.

    // Write the pixel data
    for (unsigned int i = 0; i < image->rows; i++) {
        for (unsigned int j = 0; j < image->cols; j++) {
            fputc(image->pixels[i][j].r, file); // Write red channel
            fputc(image->pixels[i][j].g, file); // Write green channel
            fputc(image->pixels[i][j].b, file); // Write blue channel
        }
    }

    fclose(file);
    return true; // Return true if the write operation was successful
}
*/

ppmimage *ppm_readimage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Read header
    char magic[3];
    if (fscanf(file, "%2s", magic) != 1 || strcmp(magic, "P6") != 0) {
        fprintf(stderr, "Unsupported PPM format: %s\n", magic);
        fclose(file);
        return NULL;
    }

    // Skip comments and whitespace
    int c;
    while ((c = fgetc(file))) {
        if (c == '#') {
            while ((c = fgetc(file)) != '\n' && c != EOF);
        } else if (!isspace(c)) {
            ungetc(c, file);
            break;
        }
    }

    ppmimage *result = malloc(sizeof(ppmimage));
    if (!result) {
        perror("Failed to allocate image struct");
        fclose(file);
        return NULL;
    }

    // Read dimensions and maxval
    if (fscanf(file, "%u %u %u", &result->cols, &result->rows, &result->maxval) != 3) {
        fprintf(stderr, "Invalid PPM header\n");
        fclose(file);
        free(result);
        return NULL;
    }
    
    // Skip to binary data
    while (fgetc(file) != '\n' && !feof(file));

    // Allocate pixel array
    result->pixels = calloc(result->rows, sizeof(pixel *));
    if (!result->pixels) {
        perror("Failed to allocate pixel rows");
        fclose(file);
        free(result);
        return NULL;
    }

    for (unsigned int i = 0; i < result->rows; i++) {
        result->pixels[i] = calloc(result->cols, sizeof(pixel));
        if (!result->pixels[i]) {
            perror("Failed to allocate pixel row");
            for (unsigned int j = 0; j < i; j++) free(result->pixels[j]);
            free(result->pixels);
            free(result);
            fclose(file);
            return NULL;
        }
    }

    // Read pixel data
    for (unsigned int i = 0; i < result->rows; i++) {
        for (unsigned int j = 0; j < result->cols; j++) {
            pixel *p = &result->pixels[i][j];
            if (fread(&p->r, 1, 1, file) != 1 ||
                fread(&p->g, 1, 1, file) != 1 ||
                fread(&p->b, 1, 1, file) != 1) {
                fprintf(stderr, "Unexpected end of pixel data\n");
                ppm_destroy(result);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return result;
}

void ppm_destroy(ppmimage *image) {
    if (!image) return;
    
    if (image->pixels) {
        for (unsigned int i = 0; i < image->rows; i++) {
            free(image->pixels[i]);
        }
        free(image->pixels);
    }
    free(image);
}

bool ppm_writeimage(const char *filename, ppmimage *image) {
    if (!image || !image->pixels) return false;
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return false;
    }

    // Write header
    fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);

    // Write pixel data
    for (unsigned int i = 0; i < image->rows; i++) {
        for (unsigned int j = 0; j < image->cols; j++) {
            pixel *p = &image->pixels[i][j];
            if (fwrite(&p->r, 1, 1, file) != 1 ||
                fwrite(&p->g, 1, 1, file) != 1 ||
                fwrite(&p->b, 1, 1, file) != 1) {
                perror("Failed to write pixel data");
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return true;
}
