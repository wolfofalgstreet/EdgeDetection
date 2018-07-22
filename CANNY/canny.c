// C reated by Isaias Perez
// Canny Filter - Edge Detector
// ----------------------
// Program will generate 3 output images in .pgm format
// The smoothened magnitude gradient, peaks images, and final output image showing true edges.
// To run progrm on terminal:
// gcc main.c
// ./a.out sourceImage.pgm 1 .25    // Sigma and percent respectively

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// CONSTANTS //
#define  PICSIZE 256
#define  MAXMASK 100
#define  MAX_VAL 255

// MATRICES //
int    pic[PICSIZE][PICSIZE];
double Gx[PICSIZE][PICSIZE];
double Gy[PICSIZE][PICSIZE];
int    peaks[PICSIZE][PICSIZE];
int    final[PICSIZE][PICSIZE];
double maskX[MAXMASK][MAXMASK]; // Should masks be the same size as the pic?
double maskY[MAXMASK][MAXMASK];
double conv[PICSIZE][PICSIZE];

// FUNCTION SIGNATURES //
void initImgFiles();
void processHeader();
void outFile(int arr[PICSIZE][PICSIZE], double ar[PICSIZE][PICSIZE], FILE* outFile, char* name);

// GLOBALS //
FILE *magGradient,*sourceFile, *ridgePeaks, *finalImage, *fopen();
void parseHeader();
void outPeaks();

int DX[] = {-1, 0, 1};
int DY[] = {-1, 0, 1};

int main(int argc, const char * argv[]) {

    if (argc == 4) {
        // IDE TESTING //
        //char* fileName = "/Users/Isaias/Desktop/FALL 2017/ROBOT VISION/ASSIGNMENTS/#1/B_Canny/Canny/Canny/garb34.pgm";
        // TERMINAL TESTING //
        const char* fileName = argv[1];

        int     i,j,p,q,mr,centx,centy, direction, HI, LO, cutOff, areaOfTops;
        double  maskvalX, maskvalY,sumX, sumY,sig,maxival, percent;

        // Open Source PGM File
        sourceFile = fopen(fileName,"rb");

        // Process header of source image
        processHeader();

        // Prep Output Images
        initImgFiles();

        // Read Sigma from terminal
        sig = atof(argv[2]);

        // Read percent
        percent = atof(argv[3]);

        // Magnitude ratio and variable mask size setup
        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        // Read file into pic[][]
        for (i=0;i<256;i++) {
            for (j=0;j<256;j++) {
                pic[i][j]  =  getc(sourceFile);
            }
        }

        // Computing 1st derivative of guassian
        for (p=-mr;p<=mr;p++) {
            for (q=-mr;q<=mr;q++) {
                maskvalX = -1 * (q*exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                maskvalY = -1 * (p *exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                maskX[p+centy][q+centx] = maskvalX;
                maskY[p+centy][q+centx] = maskvalY;
            }
        }

        // Convolution Gx = A*Gx and Gy = A*Gy
        for (i=mr;i<=255-mr;i++) {
            for (j=mr;j<=255-mr;j++) {
                sumX = 0; sumY = 0;
                for (p=-mr;p<=mr;p++) {
                    for (q=-mr;q<=mr;q++) {
                        sumX += pic[i+p][j+q] * maskX[p+centy][q+centx];
                        sumY += pic[i+p][j+q] * maskY[p+centy][q+centx];

                    }
                }
                Gx[i][j] = sumX;
                Gy[i][j] = sumY;
            }
        }



    return 0;
}

// READ HEADER OF FILE //
void processHeader() {
    char string[15];
    for (int x = 0; x < 3; x++) {
        fgets(string, 15, sourceFile);
    }
}


// INITIATES OUTPUT IMG FILES //
void initImgFiles() {
    magGradient = fopen("gradMag.pgm", "wb");
    ridgePeaks = fopen("peaks.pgm", "wb");
}

void outPeaks() {
    fprintf(ridgePeaks,"P5\n256 256\n255\n");
    for (int i=0;i<256;i++) {
        for (int j=0;j<256;j++) {
            fprintf(ridgePeaks, "%c",(char)((int)peaks[i][j]));
        }
    }
}

// outfile
