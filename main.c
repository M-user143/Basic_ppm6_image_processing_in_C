#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void display_menu() {
    printf("\nPPM Image Processor\n");
    printf("1. Invert Colors\n");
    printf("2. Convert to Grayscale\n");
    printf("3. Flip Horizontally\n");
    printf("4. Flip Vertically\n");
    printf("5. Rotate 90° Clockwise\n");
    printf("6. Apply Sepia Filter\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

void invert_colors(ppmimage *img) {
    for (unsigned int i = 0; i < img->rows; i++) {
        for (unsigned int j = 0; j < img->cols; j++) {
            img->pixels[i][j].r = img->maxval - img->pixels[i][j].r;
            img->pixels[i][j].g = img->maxval - img->pixels[i][j].g;
            img->pixels[i][j].b = img->maxval - img->pixels[i][j].b;
        }
    }
}

void grayscale(ppmimage *img) {
    for (unsigned int i = 0; i < img->rows; i++) {
        for (unsigned int j = 0; j < img->cols; j++) {
            uint8_t gray = (uint8_t)(
                0.299 * img->pixels[i][j].r +
                0.587 * img->pixels[i][j].g +
                0.114 * img->pixels[i][j].b
            );
            img->pixels[i][j].r = gray;
            img->pixels[i][j].g = gray;
            img->pixels[i][j].b = gray;
        }
    }
}

void flip_horizontal(ppmimage *img) {
    for (unsigned int i = 0; i < img->rows; i++) {
        for (unsigned int j = 0; j < img->cols/2; j++) {
            pixel temp = img->pixels[i][j];
            img->pixels[i][j] = img->pixels[i][img->cols - 1 - j];
            img->pixels[i][img->cols - 1 - j] = temp;
        }
    }
}

void flip_vertical(ppmimage *img) {
    for (unsigned int i = 0; i < img->rows/2; i++) {
        for (unsigned int j = 0; j < img->cols; j++) {
            pixel temp = img->pixels[i][j];
            img->pixels[i][j] = img->pixels[img->rows - 1 - i][j];
            img->pixels[img->rows - 1 - i][j] = temp;
        }
    }
}

void rotate_90_clockwise(ppmimage **img_ptr) {
    ppmimage *img = *img_ptr;
    ppmimage *rotated = malloc(sizeof(ppmimage));
    rotated->rows = img->cols;
    rotated->cols = img->rows;
    rotated->maxval = img->maxval;
    
    rotated->pixels = calloc(rotated->rows, sizeof(pixel *));
    for (unsigned int i = 0; i < rotated->rows; i++) {
        rotated->pixels[i] = calloc(rotated->cols, sizeof(pixel));
    }
    
    for (unsigned int i = 0; i < img->rows; i++) {
        for (unsigned int j = 0; j < img->cols; j++) {
            rotated->pixels[j][rotated->cols - 1 - i] = img->pixels[i][j];
        }
    }
    
    ppm_destroy(img);
    *img_ptr = rotated;
}

void sepia_filter(ppmimage *img) {
    for (unsigned int i = 0; i < img->rows; i++) {
        for (unsigned int j = 0; j < img->cols; j++) {
            uint8_t r = img->pixels[i][j].r;
            uint8_t g = img->pixels[i][j].g;
            uint8_t b = img->pixels[i][j].b;
            
            int new_r = (int)(0.393 * r + 0.769 * g + 0.189 * b);
            int new_g = (int)(0.349 * r + 0.686 * g + 0.168 * b);
            int new_b = (int)(0.272 * r + 0.534 * g + 0.131 * b);
            
            img->pixels[i][j].r = (new_r > 255) ? 255 : new_r;
            img->pixels[i][j].g = (new_g > 255) ? 255 : new_g;
            img->pixels[i][j].b = (new_b > 255) ? 255 : new_b;
        }
    }
}

int main() {
    char filename[256];
    printf("Enter path to PPM image: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return EXIT_FAILURE;
    }
    
    // Remove newline character
    filename[strcspn(filename, "\n")] = '\0';
    
    ppmimage *img = ppm_readimage(filename);
    if (!img) {
        fprintf(stderr, "Failed to load image: %s\n", filename);
        return EXIT_FAILURE;
    }
    
    printf("Loaded image: %ux%u (maxval=%u)\n", img->cols, img->rows, img->maxval);
    
    char output_path[256];
    printf("Enter output path: ");
    if (fgets(output_path, sizeof(output_path), stdin) == NULL) {
        fprintf(stderr, "Error reading output path\n");
        ppm_destroy(img);
        return EXIT_FAILURE;
    }
    output_path[strcspn(output_path, "\n")] = '\0';
    
    int choice;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1: invert_colors(img); break;
            case 2: grayscale(img); break;
            case 3: flip_horizontal(img); break;
            case 4: flip_vertical(img); break;
            case 5: rotate_90_clockwise(&img); break;
            case 6: sepia_filter(img); break;
            case 7: break;
            default: printf("Invalid choice. Try again.\n");
        }
        
        if (choice >= 1 && choice <= 6) {
            if (ppm_writeimage(output_path, img)) {
                printf("Operation successful. Saved to: %s\n", output_path);
            } else {
                fprintf(stderr, "Failed to save image\n");
            }
        }
    } while (choice != 7);
    
    ppm_destroy(img);
    printf("Exiting...\n");
    return EXIT_SUCCESS;
}