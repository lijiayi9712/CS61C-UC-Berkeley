/*
 * Project 1-1: Computing a Displacement Map
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement)
            / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
}




/* Helper function to return the square euclidean distance between two values. */
unsigned int square_euclidean_distance(unsigned char a, unsigned char b, int width, int height) {
   /*
    * This is an optional helper function which you may find useful. It
    * currently has an incomplete example CUnit test to help with debugging.
    * This test is not graded and is merely for your convenience. See the spec
    * for details on how to use CUnit.
    */
    
    

    
    return 0;
}


unsigned int to_1D(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    return y*width + x;
}




void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
    int i, j;
    //printf("maximum_displacement is %d\n", maximum_displacement);
    for (j = 0; j < image_height; j++) {
        for (i = 0; i < image_width; i++) {
            //printf("index is %d\n", to_1D(i, j, image_width, image_height));
            //printf("This is left: %d\n", left[to_1D(i, j, image_width, image_height)]);
            depth_map[to_1D(i, j, image_width, image_height)] = 0;
        }
    }


    if (maximum_displacement == 0) {
        return;
    }
    if (image_width == 0 || image_height == 0) {
        return;
    }
    int leftx;//i % image_width;
    int lefty;//i / image_width;
    for (leftx = 0; leftx < image_width; leftx++) {
        for (lefty = 0; lefty < image_height; lefty++) {
            int i = to_1D(leftx, lefty, image_width, image_height);

            if (leftx<feature_width || lefty < feature_height || lefty+feature_height>=image_height ||leftx+feature_width >= image_width) {
                depth_map[i] = 0;
                continue;
            }
            
            //create green box of candidates
            int box_right, box_left, box_bottom, box_top;
            //int x_radius = maximum_displacement - feature_width;
            //int y_radius = maximum_displacement - feature_height;
            int x_radius = maximum_displacement;
            int y_radius = maximum_displacement;
            if (leftx + x_radius < image_width) { 
            //if (leftx + x_radius < image_width-feature_width) { //leftx + max_dis < image_width
                box_right = leftx+x_radius;
            } else {
                box_right = image_width-1;
                //box_right = image_width-feature_width -1;
            }
            if (leftx - x_radius >=0) {  //leftx - max_disp + feature_width >= 0
                box_left = leftx - x_radius; 
            } else {
                box_left = 0;
                //box_left = feature_width;
            }
            if (lefty + y_radius < image_height) {
            //if (lefty + y_radius < image_height-feature_height) {
                box_bottom = lefty + y_radius;
            } else {
                //box_bottom = image_height-feature_height-1;
                box_bottom = image_height-1;
            }
            if (lefty - y_radius >= 0) {
                box_top = lefty - y_radius;
            } else {
                box_top = 0;
                //box_top = feature_height;
            }

            depth_map[i] = 0;
            int y;
            int x;
            unsigned int min_distance = 99999;
            unsigned int min_norm = 99999;
            
            for (y = box_top; y<= box_bottom; y++) {
                for (x = box_left; x<= box_right; x++) {
                    
                    
                    if (x<feature_width || y < feature_height || y+feature_height>=image_height ||x+feature_width >= image_width) {
                        
                        continue;
                    }
                    
                    
                    int f_x;
                    int f_y;
                    unsigned int total_distance = 0;
                    
                    for (f_x = -1*feature_width; f_x <= feature_width; f_x++) {
                        for (f_y = -1*feature_height; f_y <= feature_height; f_y++) {
                            total_distance += pow((left[(leftx+f_x) + (lefty+f_y)*image_width] - right[(x+f_x) + (y+f_y)*image_width]), 2);
                        }
                    }
                    //unsigned int total_distance = square_euclidean_distance(current_feature, right_feature, image_width, image_height);
                
                    if (total_distance < min_distance) {
                        min_distance = total_distance;
                        int dx = (x-leftx);
                        int dy = (y-lefty);
                        int norm = normalized_displacement(dx, dy, maximum_displacement);
                        min_norm = norm;
                        
                    } else if (total_distance == min_distance) {
                        int dx = (x-leftx);
                        int dy = (y-lefty);
                        int norm = normalized_displacement(dx, dy, maximum_displacement);
                        if (norm < min_norm) {
                            min_norm = norm;
                        }
                    }
                }
            }
            
            depth_map[i] = min_norm;
            
        }
    }
}