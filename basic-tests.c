/*
 * HERO HelloWorld Example Application
 * EDITED: Testbench for memory behavior 
 * Copyright 2018 ETH Zurich, University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hero-target.h>  // BIGPULP_MEMCPY
#include <stdio.h>        // printf()
#include <time.h>

#define MAX_H 4096*5
#define MAX_W 4096*5

#define W 16
#define H 16
#define INC_MOD 4
#define VERBO 0
#define ARGUS 1

#define IFTOFROM 1
#define OTHER_MEASUREMENTS 0
/*---------------------------------------*/
/*       F U N C T I O N S               */
/*---------------------------------------*/
#pragma omp declare target 
void array1( __host uint32_t * __restrict__ x, int width, int height ) {
  #pragma omp parallel
  {
    x[1] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array2( __host uint32_t * __restrict__ x,__host uint32_t * __restrict__ y, int width, int height) {
  #pragma omp parallel
  {
    x[1] = y[width-1];
    y[2] = x[width-1];
  }
}
#pragma omp end declare target
/*--------------------------------------*/
#pragma omp declare target 
void array3( __host uint32_t * __restrict__ x,__host uint32_t * __restrict__ y,__host uint32_t * __restrict__ z, int width, int height) {
  #pragma omp parallel
  {
    x[1] = y[width-1];
    y[2] = z[width-1];
    z[3] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array4( __host uint32_t * __restrict__ u,__host uint32_t * __restrict__ x,__host uint32_t * __restrict__ y,__host uint32_t * __restrict__ z, int width, int height) {
  #pragma omp parallel
  {
    x[1] = y[width-1];
    y[2] = z[width-1];
    z[3] = u[width-1];
    u[4] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array5( __host uint32_t * __restrict__ w,__host uint32_t * __restrict__ u,__host uint32_t * __restrict__ x,__host uint32_t * __restrict__ y,__host uint32_t * __restrict__ z, int width, int height) {
  #pragma omp parallel
  {
    x[1] = y[width-1];
    y[2] = z[width-1];
    z[3] = u[width-1];
    u[4] = w[width-1];
    w[5] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void hellowerld( __host uint32_t * __restrict__ x, int width) {
  #pragma omp parallel
  {
    for (int i=0;i<width;i++){
      for (int j=0;j<width;j++){
        x[i*width+j] = 128;
      }
    }
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void helloworld(int x, int y, int z) {
  #pragma omp parallel
  {
    x += 128;
    y += 128;
    z += 128;
    printf("%d-%d-%d\n", x,y,z);
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void offl_const_var( __host uint32_t * __restrict__ x, int width) {
  #pragma omp parallel
  {
    x[1] = 15;
  }
}
#pragma omp end declare target
/*============================================*/
/*==============  =   MAIN   =  ==============*/
/*============================================*/

int main(int argc, char *argv[]) {

 /* Performance measurement. */
  struct timespec t_hmall_0, t_hmall_1;
  struct timespec t_warm_0, t_warm_1;
  struct timespec t_offl_0, t_offl_1;
  unsigned width          = W / ARGUS;
  unsigned height         = H;

  uint32_t* l3_in1;
  uint32_t* l3_in2;
  uint32_t* l3_in3;
  uint32_t* l3_in4;
  uint32_t* l3_in5;

  /* Allocation */
  clock_gettime(CLOCK_REALTIME, &t_hmall_0);
  if(ARGUS == 1)
    l3_in1      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t));
  else{
      l3_in1      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t)/ARGUS);
      l3_in2      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t)/ARGUS);
    if (ARGUS > 2)
      l3_in3      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t)/ARGUS);
    if (ARGUS > 3)
      l3_in4      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t)/ARGUS);
    if (ARGUS > 4)
      l3_in5      = (__host int32_t*)hero_l3malloc(MAX_H*MAX_W*sizeof(int32_t)/ARGUS);
  } 
  clock_gettime(CLOCK_REALTIME, &t_hmall_1);

  /* Init Array */
  if (ARGUS == 1){
    for(int i=0; i<MAX_H*MAX_W; i++)
      l3_in1[i] = rand() % 255; 
  } else{
    for (int i=0; i<MAX_H*MAX_W/ARGUS; i++){
      l3_in1[i] = rand() % 255;
      l3_in2[i] = rand() % 255;
    }
    if (ARGUS > 2){
      for (int i=0; i<MAX_H*MAX_W/ARGUS; i++)
        l3_in3[i] = rand() % 255;
    }
    if (ARGUS > 3){
      for (int i=0; i<MAX_H*MAX_W/ARGUS; i++)
        l3_in4[i] = rand() % 255;
    }
    if (ARGUS > 4){
      for (int i=0; i<MAX_H*MAX_W/ARGUS; i++)
        l3_in5[i] = rand() % 255;
    }      
  }

  /* Warm-up. */
  unsigned tmp_1 = 1;
  unsigned tmp_2 = 2;
  clock_gettime(CLOCK_REALTIME, &t_warm_0);
  #pragma omp target device(1) map(to: tmp_1) map(from: tmp_2)
  {
      tmp_2 = tmp_1;
  }
  tmp_1 = tmp_2;
  clock_gettime(CLOCK_REALTIME, &t_warm_1);

/*______________________________*/
/*  O F F L O A D     L O O P   */
/*______________________________*/
  for (int i=0; i<20;i++){
    // printf("ITERATION:%d\n",i);
    if(!IFTOFROM){
      /* OFFLOAD */
      if (ARGUS == 1){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(to: l3_in1[0:width*height]) 
          array1(l3_in1, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 2){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(to: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
          array2(l3_in1, l3_in2, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 3){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
          #pragma omp target device(BIGPULP_MEMCPY) map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height]) nowait
        array3(l3_in1, l3_in2, l3_in3, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 4){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
          #pragma omp target device(BIGPULP_MEMCPY) map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
        array4(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 5){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
          #pragma omp target device(BIGPULP_MEMCPY) map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],l3_in5[0:width*height]) nowait
        array5(l3_in1, l3_in2, l3_in3, l3_in4, l3_in5, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      }
    }else{
      /* OFFLOAD */
      if (ARGUS == 1){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(tofrom: l3_in1[0:width*height]) 
          array1(l3_in1, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 2){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
          array2(l3_in1, l3_in2, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 3){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height]) nowait
          array3(l3_in1, l3_in2, l3_in3, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 4){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
          array4(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      } else if(ARGUS == 5){
        clock_gettime(CLOCK_REALTIME, &t_offl_0);
        #pragma omp target device(BIGPULP_MEMCPY) map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],l3_in5[0:width*height]) nowait
          array5(l3_in1, l3_in2, l3_in3, l3_in4, l3_in5, width, height );
        clock_gettime(CLOCK_REALTIME, &t_offl_1);
      }
    }

    /*INCREMENT DATA*/
    width += (W/ARGUS)*INC_MOD;
    // printf("\n  - Offload to L3:\n");
    // printf("  -     - Execution time (ms):    %.3f ms\n", ((t_offl_1.tv_sec - t_offl_0.tv_sec) + (t_offl_1.tv_nsec - t_offl_0.tv_nsec)/1000000000.0)*1000.0 );
    if(OTHER_MEASUREMENTS){
         // Processing.
        // const unsigned long long  cycles_processing     = s.clk_0;
        // const double              processing_ms         = (double) (cycles_processing * 1000) / pulp_freq;
        // printf("\n%.3f ms\n",  processing_ms- ((t_offl_0.tv_sec) + (t_offl_0.tv_nsec)/1000000000.0)*1000.0);
        // cycles_processing     = s.clk_1;
        // processing_ms         = (double) (cycles_processing * 1000) / pulp_freq;
        // printf("%.3f ms\n",  ((t_offl_1.tv_sec) + (t_offl_1.tv_nsec)/1000000000.0)*1000.0 -processing_ms);
    }else
      printf("%.3f\n", ((t_offl_1.tv_sec - t_offl_0.tv_sec) + (t_offl_1.tv_nsec - t_offl_0.tv_nsec)/1000000000.0)*1000.0 );


    if (VERBO){
      printf("L3 Malloc: %.3f ms\n", ((t_hmall_1.tv_sec - t_hmall_0.tv_sec) + (t_hmall_1.tv_nsec - t_hmall_0.tv_nsec)/1000000000.0)*1000.0 );
      printf("Warmup: %.3f ms\n", ((t_warm_1.tv_sec - t_warm_0.tv_sec) + (t_warm_1.tv_nsec - t_warm_0.tv_nsec)/1000000000.0)*1000.0 );
    }
  }  

  return 0;
}
