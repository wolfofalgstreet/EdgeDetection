// ----------------------
// Created by Isaias Perez
// Sobel Filter
// ----------------------
// Program will generate 5 output images in .pgm format
// The smoothened magnitude gradient, Gx, Gy, and two images that exceed a low and high threshold.
// To run progrm on terminal:
// gcc main.c
// ./a.out sourceImage.pgm 50 110

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
double ival[256][256], maxival;
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
int lowThreshold, highThreshold;
FILE *source, *fopen(), *gradientX, *gradientY, *gradientMag, *gradientLow, *gradientHigh;


void initImgFiles();

int main(int argc, char * argv[]) {
   
    
    return 0;
}

// READ HEADER OF FILE //
void processHeader() {
    char string[10];
    for (int x = 0; x < 3; x++) {
        fgets(string, 10, source);
        printf("%s", string);
    }
}

// INITIATES OUTPUT IMG FILES //
void initImgFiles() {
    gradientX = fopen("Gx.pgm", "wb");
    gradientY = fopen("Gy.pgm", "wb");
    gradientMag = fopen("gradMag.pgm", "wb");
    gradientLow = fopen("gradLow.pgm", "wb");
    gradientHigh = fopen("gradientHigh.pgm", "wb");
    
}










