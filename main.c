#include <stdio.h>
#include <stdlib.h> // memory allocation, abs(x)

#include "header.h"

// self-referential structure
 struct rgb{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

typedef struct rgb rgb; // synonym for struct rgb

// prototypes
void downsize_function(rgb *downsized, rgb *A, rgb *B, rgb *C, rgb *D);

int main() {
    FILE *inputPtr;

    if ((inputPtr = fopen("itu.bmp", "rb")) == NULL) {
        printf("The \"........bmp\" file could not be opened\n");
    } else {
        BITMAPFILEHEADER fileheader_input;
        fread(&fileheader_input, sizeof(BITMAPFILEHEADER), 1, inputPtr);
        BITMAPINFOHEADER infoheader_input;
        fread(&infoheader_input, sizeof(BITMAPINFOHEADER), 1, inputPtr);

        if (fileheader_input.bfType != 0x4d42 || fileheader_input.bfOffBits != 54 ||
            infoheader_input.biSize != 40 || infoheader_input.biBitCount != 24 || infoheader_input.biCompression != 0) {
            fclose(inputPtr);
            printf("Unsupported file format.\n");
        } else {
            FILE *outputPtr;

            if ((outputPtr = fopen("itu_downscaled.bmp", "wb")) == NULL) {
                printf("The \"......_downscaled.bmp\" file could not be created\n");
            } else {
                BITMAPFILEHEADER fileheader_output = fileheader_input;
                BITMAPINFOHEADER infoheader_output = infoheader_input;

                infoheader_output.biWidth = infoheader_input.biWidth / 2;
                infoheader_output.biHeight = infoheader_input.biHeight / 2;
                fileheader_output.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) +
                                           3 * abs(infoheader_output.biWidth) * abs(infoheader_output.biHeight);

                fwrite(&(fileheader_output), sizeof(BITMAPFILEHEADER), 1, outputPtr);
                fwrite(&(infoheader_output), sizeof(BITMAPINFOHEADER), 1, outputPtr);

                rgb *line_a = calloc(infoheader_input.biWidth, sizeof(rgb));
                rgb *line_b = calloc(infoheader_input.biWidth, sizeof(rgb));
                rgb *downsized_line = calloc(infoheader_output.biWidth, sizeof(rgb));

                for (int i = 0; i < infoheader_input.biHeight; i += 2) {
                    //it first reads the pixel data after the header file
                    fread(line_a, sizeof(rgb), infoheader_input.biWidth, inputPtr);
                    //this reads the next line
                    fread(line_b, sizeof(rgb), infoheader_input.biWidth, inputPtr);
                    for (int j = 0; j < infoheader_output.biWidth; ++j) {
                        //averaging
                        downsize_function(&(downsized_line[j]), &(line_a[j * 2]), &(line_a[(j * 2) + 1]),
                                          &(line_b[j * 2]),
                                          &(line_b[(j * 2) + 1]));
                        //writes the averaging result to the original pixel
                        fwrite(&(downsized_line[j]), sizeof(rgb), 1, outputPtr);
                    }
                }
                free(line_a);
                free(line_b);
                free(downsized_line);
                fclose(inputPtr);
                fclose(outputPtr);
            }
        }
    }
}
//it is the function that takes 4 adjacent pixels and average them
void downsize_function(rgb *downsized, rgb *A, rgb *B, rgb *C, rgb *D)
{
    downsized->red = (A->red + B->red + C->red + D->red) / 4;
    downsized->green = (A->green + B->green + C->green + D->green) / 4;
    downsized->blue = (A->blue + B->blue + C->blue + D->blue) / 4;
}