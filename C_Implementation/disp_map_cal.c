#include <math.h>
#include <algorithm>
#include<cmath>
#include<stdio.h>
#include<stdlib.h>
#include "disp_map_cal.h"

double* calculate_disp_map(int* imgL, int* imgR, int img_r, int img_c, int win_r, int win_c, int step_sz, int numDisp){

    /* This function takes input the image array pointers, the rows in image, columns in image, the window size,
    the step size and the disparity range (neighbouring range for calculating disparity). It then performs the 
    disparity calculation and returns a 1D array pointer pointing to the disparity map array.*/

    double R_img[img_r][img_c];
    double L_img[img_r][img_c];
    int counter = 0;

    for(int i=0; i<img_r; i++){
        for(int j=0; j<img_c; j++){
            // reading back the image arrays from the pointers
            R_img[i][j] = *(imgR+counter);
            L_img[i][j] = *(imgL+counter);
            counter++;
        }
    }

    double disp_map[img_r][img_c]; // double 2D array to store disparity map values for corresponding pixels
    double ssd[2];
    double sum1=0;

    for(int i=0; i<img_r; i++){
        for(int j=0; j<img_c-win_c; j++){
            // ssd[0] stores INFINITY such that it can compare with sum1 and store the minimum value of ssd in ssd[1]
            ssd[0] = INFINITY;
            // ssd[1] stores the pixel difference corresponding to minimum ssd value
            ssd[1] = 0;
            sum1 = 0;

            //this for loop makes the window slide horizontally over  the'Left' image
            for(int m=max(0, j-numDisp); m<min(img_c-win_c, j+numDisp); m+step_sz){
                for(int k=0; k<win_r; k++){
                    for(int l=0; l<win_c; l++){
                        sum1 += pow((R_img[i+k][j+l] - L_img[i+k][j+m+l]), 2); // calculates the squared difference of pixel value for each pixel
                    }
                }

                //here it stores the minimum ssd value and pixel difference corresponding to it
                if(sum1<ssd[0]){
                    ssd[0] = abs(sum1);
                    ssd[1] = abs(m-j);
                }
            }

            disp_map[i][j] = ssd[1];
        }
    }

    //storing the 2D disp_map array in 1D temp array to generate a 1D pointer and return from the function.
    double temp_dispmap[img_r * img_c];
    counter = 0;
    for(int i=0; i<img_r; i++){
        for(int j=0; j<img_c-win_c; j++){
            temp_dispmap[counter++] = disp_map[i][j];
        }
    }

    return temp_dispmap;
}