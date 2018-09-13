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
        //char* fileName = "filename.pgm";
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

        // Biderectional gradient magnitude computation
        // Tracks maxVal to use on avg val calculation
        maxival = 0;
        for (i=mr;i<256-mr;i++) {
            for (j=mr;j<256-mr;j++) {
                conv[i][j] = sqrt((double)((Gx[i][j]*Gx[i][j]) + (Gy[i][j]*Gy[i][j])));
                if (conv[i][j] > maxival) maxival = conv[i][j];

            }
        }

        // Smoothing magnitude gradient, write it to file
        fprintf(magGradient,"P5\n256 256\n255\n");
        for (i=0;i<256;i++) {
            for (j=0;j<256;j++) {
                conv[i][j] = (conv[i][j] / maxival) * MAX_VAL;
                fprintf(magGradient, "%c",(char)((int)conv[i][j]));
            }
        }

        // Prep Peaks
        for (int i=0;i<256;i++) {
            for (int j=0;j<256;j++) {
                peaks[i][j] = 0;
            }
        }

        // Non-Maxima Supression
        // RidgePeaks
        for (i=mr;i<256-mr;i++) {
            for (j=mr;j<256-mr;j++) {

                // Probably shouldn't have zeros in denominator
                if (Gx[i][j] == 0)  Gx[i][j] = 0.000001;

                direction = Gy[i][j]/Gx[i][j];

                // MaxTest on horizontal pixels
                if ((direction > -.4142) && (direction <= .4142)) {
                    if ((conv[i][j] > conv[i][j-1]) && (conv[i][j] > conv[i][j+1])) {
                        peaks[i][j] = MAX_VAL;
                    }

                    // MaxTest on SW and NE pixels
                } else if ((direction > .4142) && (direction <= 2.4142)) {
                    if ((conv[i][j] > conv[i-1][j-1]) && (conv[i][j] > conv[i+1][j+1])) {
                        peaks[i][j] = MAX_VAL;
                    }

                    // MaxTest on SE and NW pixels
                } else if ((direction > -2.4142) && (direction <= -.4142)) {
                    if ((conv[i][j] > conv[i+1][j-1]) && (conv[i][j] > conv[i-1][j+1])) {
                        peaks[i][j] = MAX_VAL;
                    }

                    // Maxtest on vertical pixels
                } else {
                    if ((conv[i][j] > conv[i-1][j]) && (conv[i][j] > conv[i+1][j])) {
                        peaks[i][j] = MAX_VAL;
                    }

                }
            }
        }

        // Generating peak image
        outPeaks();

        // Building magnitude histogram
        int histogram[PICSIZE*PICSIZE];
        for (i = 0; i < PICSIZE*PICSIZE; i++) {
            histogram[i] = 0;
        }
        for (i = 0; i < PICSIZE; i++) {
            for (j = 0; j < PICSIZE; j++) {
                int h = (int)conv[i][j];
                histogram[h] += 1;
            }
        }

        // Auto generate HI and LO
        cutOff = percent*PICSIZE*PICSIZE;
        areaOfTops = 0;
        for (i = PICSIZE*PICSIZE - 1; i > 0; i--) {
            areaOfTops += histogram[i];
            if (areaOfTops > cutOff) {
                HI = i;
                LO = HI*.35;
                break;
            }
        }

        printf("HI: %d  LO: %d\n", HI, LO);

        // Double thresholding
        for (i = 0; i < 256; i++) {
            for (j = 0; j < 256; j++) {

                // Handle magnitudes above HI and below LO
                if (peaks[i][j] == MAX_VAL) {
                    if (conv[i][j] > HI) {
                        peaks[i][j] = 0;
                        final[i][j] = MAX_VAL;
                    } else if (conv[i][j] < LO) {
                        peaks[i][j] = 0; final[i][j] = 0;
                    }
                }
            }
        }

        // Handle magnitudes in between HI and LO
        int moreToDo = 1;
        while (moreToDo) {
            moreToDo = 0;
            for (i = 0; i < 256; i++) {
                for (j = 0; j < 256; j++) {
                    if (peaks[i][j]) {
                        for (int x = 0; x < 3; x++) {
                            for (int y = 0; y < 3; y++) {
                                if (final[i + DX[x]][j + DY[y]]) {
                                    peaks[i][j] = 0; final[i][j] = MAX_VAL;
                                    moreToDo = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Write final image
        outFile(final, NULL, finalImage, "final.pgm");

    } else {
        printf("Need more arguments!\n");
    }



    return 0;
}

// *--------------------------------------------------------* //
// PGM file has a header that must be ommited for convolution //
// *--------------------------------------------------------* //
void processHeader() {
    char string[15];
    for (int x = 0; x < 3; x++) {
        fgets(string, 15, sourceFile);
    }
}


// *--------------------------------------------------------* //
// Initiates files to be written to                           //
// *--------------------------------------------------------* //
void initImgFiles() {
    magGradient = fopen("gradMag.pgm", "wb");
    ridgePeaks = fopen("peaks.pgm", "wb");
}


// *--------------------------------------------------------* //
// Output all peaks discovered after convolution              //
// *--------------------------------------------------------* //
void outPeaks() {
    fprintf(ridgePeaks,"P5\n256 256\n255\n");
    for (int i=0;i<256;i++) {
        for (int j=0;j<256;j++) {
            fprintf(ridgePeaks, "%c",(char)((int)peaks[i][j]));
        }
    }
}


void outFile(int arr[PICSIZE][PICSIZE], double ar[PICSIZE][PICSIZE], FILE* outFile, char* name) {
    outFile = fopen(name, "wb");
    fprintf(outFile,"P5\n256 256\n255\n");
    for (int i=0;i<256;i++) {
        for (int j=0;j<256;j++) {
            if (arr != NULL) {

                fprintf(outFile, "%c",(char)((int)arr[i][j]));
            } else {
                fprintf(outFile, "%c",(char)((int)ar[i][j]));
            }
        }
    }
}
