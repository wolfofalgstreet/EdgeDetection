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

void outGradTrshImg(double value, int start);
void outDirectionalGrad();
void processHeader();
void initImgFiles();

int main(int argc, char * argv[]) {
    int i,j,p,q,mr,sum1,sum2;
    
    if (argc == 4) {

        // Read terminal parameters
        for (int x = 1; x < 4; x++) {
            switch (x) {
                case 1: // Source file
                    source = fopen(argv[x], "rb");
                    break;
                case 2: // Low threshold
                    lowThreshold = atoi(argv[x]);
                    break;
                case 3: // High threshold
                    highThreshold = atoi(argv[x]);
                    break;
                default:
                    break;
            }
        }
    
        
        initImgFiles();
        processHeader();
        
        // Loading pic
        for (i=0;i<256;i++) {
            for (j=0;j<256;j++) {
                pic[i][j]  =  getc (source);
                pic[i][j]  &= 0377;
            }
        }
        
        // Convolution calculation
        // Uses 4 ones
        mr = 1;
        for (i=mr;i<256-mr;i++) {
            for (j=mr;j<256-mr;j++) {
                sum1 = 0;
                sum2 = 0;
                for (p=-mr;p<=mr;p++) {
                    for (q=-mr;q<=mr;q++) {
                        sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                        sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                    }
                }
                outpicx[i][j] = sum1;
                outpicy[i][j] = sum2;
            }
        }
        
        // Output GX and GY Gradients
        outDirectionalGrad();
    
        // Biderectional gradient magnitude
        maxival = 0;
        for (i=mr;i<256-mr;i++) {
            for (j=mr;j<256-mr;j++) {
                ival[i][j] = sqrt((double)((outpicx[i][j]*outpicx[i][j]) + (outpicy[i][j]*outpicy[i][j])));
                if (ival[i][j] > maxival) maxival = ival[i][j];
            }
        }
        
        // Smoothing
        int start = 1;
        for (i=0;i<256;i++) {
            for (j=0;j<256;j++) {
                ival[i][j] = (ival[i][j] / maxival) * 255;
                outGradTrshImg(ival[i][j], start);
                start = 0;
            }
        }

    // Fail safe
    } else {
        printf("You need more arguments!\n");
        printf("arg[0]: executable arg[1]: source.pgm arg[2]: LowThreshold arg[3]: HighTreshold\n");
    }
    
    return 0;
}

// READ HEADER OF FILE //
void processHeader() {
    char string[10];
    for (int x = 0; x < 3; x++) {
        fgets(string, 10, source);
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

// WRITE IMAGES THAT EXCEED A LIBERAL LOW AND RESTRICTED HIGH THRESHOLDS //
void outGradTrshImg(double value, int start) {
    if (start) {
        fprintf(gradientLow,"P5\n256 256\n255\n");
        fprintf(gradientHigh,"P5\n256 256\n255\n");
        fprintf(gradientMag,"P5\n256 256\n255\n");
    }
    double white = 255; double black = 0;
    
    // Write magnitude image
    fprintf(gradientMag,"%c",(char)((int)value));
    
    // Write B&W img exceeding low threshold
    if ((int)value >= lowThreshold) {
        fprintf(gradientLow, "%c", (char)((int)white));
    } else {
        fprintf(gradientLow, "%c", (char)((int)black));
    }
    
    // Write B&W img exceeding high treshold
    if ((int)value >= highThreshold) {
        fprintf(gradientHigh, "%c", (char)((int)white));
    } else {
        fprintf(gradientHigh, "%c", (char)((int)black));
    }
}

// WRITE DIRECTIONAL GRADIENTS //
void outDirectionalGrad() {
    fprintf(gradientX,"P5\n256 256\n255\n");
    fprintf(gradientY,"P5\n256 256\n255\n");
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256; y++) {
            fprintf(gradientX,"%c",(char)sqrt((double)(outpicx[x][y]*outpicx[x][y])));
            fprintf(gradientY,"%c",(char)sqrt((double)(outpicy[x][y]*outpicy[x][y])));
        }
    }
}















