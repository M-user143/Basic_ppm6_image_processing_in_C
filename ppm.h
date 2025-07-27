#ifndef PPM_H
#define PPM_H

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint16_t r,g,b;                 //We are using 16 bits as max 
} pixel;


typedef struct {                    //A struct that tells about the size of the ppm image.
    unsigned int rows;
    unsigned int cols;
    unsigned int maxval;

    pixel **pixels;                 //Just making a pixel struct double pointer.

} ppmimage;



ppmimage *ppm_readimage(const char *filename);  //The prototype of the function that will read a ppm image fron file and store it in the pointer of ppmimage in Heap.
void ppm_destroy(ppmimage *image);              //A prototype of function to destroy the memory allocated by the ppm image loaded.
bool ppm_writeimage(const char *filename, ppmimage *image);  //Prototype of function that will write changes/edits to the ppm image file after processing.

#endif