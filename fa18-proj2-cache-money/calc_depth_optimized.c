/*
 * Project 2: Performance Optimization
 */
#if !defined(_MSC_VER)
#include <pthread.h>
#endif
#include <omp.h>


#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "calc_depth_naive.h"
#include "calc_depth_optimized.h"
#include "utils.h"



/* Implements the displacement function */

void calc_depth_optimized(float *depth, float *left, float *right,
        int image_width, int image_height, int feature_width,
        int feature_height, int maximum_displacement) {
    // Naive implementation
    __m128 zeros = _mm_setzero_ps();
    int image_area = image_width*image_height;
    
    for (int i = 0; i < image_area /4 *4; i+=4) {
        _mm_storeu_ps(depth+i, zeros);
    }

    for (int i = image_area/4*4;i<image_area;i++) {
        depth[i] = 0.0;
    }

    
    #pragma omp parallel
    {
    int width_x = image_width - feature_width;
    int width_y = image_height - feature_height;
    int feature_size = 2*feature_height+1;
    int feature_area = feature_size * feature_size;
    float new_left[feature_area];
    float new_right[feature_area];
//    int left_x;
    int left_y;
//    int right_x;
    int right_y;
    float min_diff;
    int min_dy;
    int min_dx;
    int k;
    int w;
    int n;
    int m;
    int a;
    float squared_diff;
    float result[4];
    __m128 sum_a = _mm_setzero_ps();  //for left 
    __m128 sum_b = _mm_setzero_ps();  //for right
    __m128 temp = _mm_setzero_ps();
    
    int r;
    int s;
    int t;
    int u; 
    



   #pragma omp for schedule(dynamic)
    for (int y = feature_height; y < width_y; y++) {
        
        for (int x = feature_width; x < width_x; x++) {
     
            for (int box_y = -feature_height; box_y <= feature_height; box_y++) {
                //for (int box_x = -feature_width; box_x <= feature_width; box_x++) {
                left_y = y + box_y;
                memcpy(new_left+(box_y+feature_height)*feature_size, left+left_y * image_width+x-feature_width, (2*feature_width+1)*sizeof(float));
                            //left_x = x + box_x;
                            //
                            //new_left[box_x+feature_width+(box_y+feature_height)*feature_size] = left[left_y * image_width + left_x];
                    //}
            }
            min_diff = -1;
            min_dy = 0;
            min_dx = 0;
            k = feature_height-y;
            w = feature_width-x;
            n = image_height-y-feature_height;
            m = image_width-x-feature_width;
            r = ((-maximum_displacement+k)+abs(-maximum_displacement-k))/2;
            s = ((n+maximum_displacement+1)-abs(n - maximum_displacement-1))/2;
            t = ((-maximum_displacement+w)+abs(-maximum_displacement-w))/2;
            u = ((m+maximum_displacement+1)-abs(m - maximum_displacement-1))/2;
            

           
            for (int dy =r; dy < s; dy++) {
                for (int dx = t; dx < u; dx++) {
                    squared_diff = 0;

                    int c = x+dx;

                    int d = y+dy;
                    
                    for (int box_y = -feature_height; box_y <= feature_height; box_y++) {
                        //for (int box_x = -feature_width; box_x <= feature_width; box_x++) {
                        right_y = d + box_y;
                        memcpy(new_right+(box_y+feature_height)*feature_size, right+right_y * image_width+c-feature_width, (2*feature_width+1)*sizeof(float));
                        //right_x = c + box_x;
                        
                        //new_right[box_x+feature_width+(box_y+feature_height)*feature_size] = right[right_y * image_width + right_x];
                        //}
                    } 

                    __m128 sum1 = _mm_setzero_ps();

                    //#pragma omp parallel 
                
                    __m128 sum = _mm_setzero_ps();
                    // __m128 sum_a = _mm_setzero_ps();  //for left 
                    // __m128 sum_b = _mm_setzero_ps();  //for right
                    // __m128 temp = _mm_setzero_ps();

                    //int total_threads = omp_get_num_threads();
                    //int current = omp_get_thread_num();
                    //int chuck = (feature_area/16)/total_threads + 1;
                    
                    //for (int i = current*chuck*16; i < (current+1)*chuck*16; i+=16) {
                    for (int i = 0; i < (feature_area/16*16); i+=16) {
                        sum_a = _mm_loadu_ps(new_left+i);
                        sum_b = _mm_loadu_ps(new_right+i);
                        temp = _mm_sub_ps(sum_a, sum_b);
                        sum = _mm_add_ps(_mm_mul_ps(temp, temp), sum);

                        sum_a = _mm_loadu_ps(new_left+i+4);
                        sum_b = _mm_loadu_ps(new_right+i+4);
                        temp = _mm_sub_ps(sum_a, sum_b);
                        sum = _mm_add_ps(_mm_mul_ps(temp, temp), sum);

                        sum_a = _mm_loadu_ps(new_left+i+8);
                        sum_b = _mm_loadu_ps(new_right+i+8);
                        temp = _mm_sub_ps(sum_a, sum_b);
                        sum = _mm_add_ps(_mm_mul_ps(temp, temp), sum);

                        sum_a = _mm_loadu_ps(new_left+i+12);
                        sum_b = _mm_loadu_ps(new_right+i+12);
                        temp = _mm_sub_ps(sum_a, sum_b);
                        sum = _mm_add_ps(_mm_mul_ps(temp, temp), sum);
                    }
                    //chuck = (feature_area/4-feature_area/16*4)/total_threads + 1;
                    //#pragma omp for
                    //for (int i = feature_area/16*16+current*chuck*4; i < feature_area/16*16+(current+1)*chuck*4; i+=4) {
                    for (int i = feature_area/16*16; i < feature_area/4*4; i+=4) {
                        sum_a = _mm_loadu_ps(new_left+i);
                        sum_b = _mm_loadu_ps(new_right+i);
                        temp = _mm_sub_ps(sum_a, sum_b);
                        sum = _mm_add_ps(_mm_mul_ps(temp, temp), sum);
                    }


                
                    sum1 = _mm_add_ps(sum1,sum);

                    
                    _mm_storeu_ps(result, sum1);
                    for (int i = feature_area/4*4; i < feature_area; i++) {
                        a = new_left[i]-new_right[i];
                        result[3] += a*a;
                    }

                    squared_diff = result[0]+result[1]+result[2]+result[3];
                    if (min_diff == -1 || min_diff > squared_diff
                            || (min_diff == squared_diff
                                && sqrt(dx * dx + dy * dy) < sqrt(min_dx * min_dx + min_dy * min_dy))) {
                        min_diff = squared_diff;
                        min_dx = dx;
                        min_dy = dy;
                    }
                }
            }
            if (min_diff != -1) {
                if (maximum_displacement != 0) {
                    depth[y * image_width + x] = sqrt(min_dx * min_dx + min_dy * min_dy);
                }
            }
        }
        }
    
    }
}