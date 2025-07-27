#include "ppm.h"        //A tradition that this is my own file from the currect directory using "".
#include <stdlib.h>     //<> indicates a Standard Library of C.
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
//#include <sys/mman.h>


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
