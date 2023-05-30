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


 /* This is a modification of the /openmp-examples/helloworld-offload-tests I've been working on to be run on the Jetson Nano
  * The following description is of the previous file. Here, all hero references have been changed.
  * 
  * This is a testbench for measuring the latency of memory in HERO. It is based on the pulp-mul project
  * from Gianluca Bellocchi, Unimore.
  * The program works like this: Allocation of the maximum available DRAM space partitioned in sections
  * according to the ARGUS (= log2(arguments)) constant. Initialization of the arrays to fill the DRAM with random
  * numbers.
  * Then according to the constants a loop over itarations is executed where in each iteration the size of the 
  * offloaded arrays grows linearly by INC_MOD and/or logarithmically by MUL_MOD. Both these constants are multiples
  * of the initial width of the arrays.
  * On each iteration code and data are offloaded and executed as per the constants IFTOFROM (include writeback to the DRAM)
  * OTHER TESTS|HEAVY_COMP:
  *                         00: arrayX functions: 1substitution per argument
  *                         01: arrayX_comp functions: #arguments*width additions and mod over the length of the arrays (currently commented out)
  *                         10: arrayX_no_comp functions: empty fucntion body, no calculations/substitutions
  *                         11: arrayX_all_comp functions: support only for ARGUS=1, computations (addition + mod) over multiple times the length up 
  *                             to the maximum array length. This was done to test over incresing size and arguments so that the computations are 
  *                             constant among variant array width and variations in argument number but was not continued to support more arguments 
  *                             beacause offloading of the arrays is done even without any calculations.
  *
  * Time measurements are taken at the end of main. Timestamps are taken across the whole program only from the host's side but only 1 time measurement is printed
  * currently (the offload+application+writeback time IFTOFROM, or just the offload !IFTOFROM).
  * Printing of the other measurements can be done by uncommenting the corresponding lines of code.
  *
  * For reference when setting the constants W,H,ITERATIONS,ARGUS,INC_MOD and MUL_MOD the following must apply:
  * MAX_W*MAX_H = 4096*4096*5*5 = W*H+ITERATIONS*W*H*INC_MOD = 419430400
  */

// #include <hero-target.h>  // BIGPULP_MEMCPY
#include <stdio.h>        // printf()
#include <time.h>

#define MAX_H 1
#define MAX_W 4096*5 * 4096*5

#define W 16//1093750//4096*5 * 4096*5 / 4
#define H 1
#define INC_MOD 0
#define MUL_MOD 2

#define HEAVY_COMP 0
#define OTHER_TESTS 1 //Other tests is only for 1 argument and heavy_comp 0: is constant operations on a few data 1: is const for all
#define VERBO 0
#define ARGUS 1
#define ITERATIONS 16//124//16+6+1
#define IFTOFROM 0
#define OTHER_MEASUREMENTS 0
/*---------------------------------------*/
/*       F U N C T I O N S               */
/*---------------------------------------*/

#pragma omp declare target 
void array1_no_comp(  long int * __restrict__ x, int width, int height ) {
  #pragma omp parallel
  {
    // x[0] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array2_no_comp(  long int * __restrict__ x, long int * __restrict__ y, int width, int height) {
  #pragma omp parallel
  {
    // x[0] = x[width-1];
    // y[0] = y[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array4_no_comp(  long int * __restrict__ x, long int * __restrict__ y, long int * __restrict__ z,  long int * __restrict__ u, int width, int height) {
  #pragma omp parallel
  {
    // x[0] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array8_no_comp(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
                   long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
                  int width, int height) {
  #pragma omp parallel
  {
  }
}
#pragma omp end declare target
/*--------------------------------------*/
#pragma omp declare target 
void array16_no_comp(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
                    long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
                    long int * __restrict__ a9, long int * __restrict__ a10, long int * __restrict__ a11, long int * __restrict__ a12,\
                    long int * __restrict__ a13, long int * __restrict__ a14, long int * __restrict__ a15, long int * __restrict__ a16,\
                  int width, int height) {
  #pragma omp parallel
  {
  }
}
#pragma omp end declare target
/*========================================*/
#pragma omp declare target 
void array1_const_all_comp(  long int * __restrict__ x, int width, int height ) { //NOT TO BE USED 
  #pragma omp parallel
  {
    int thresh = W*H+ITERATIONS*W*H*INC_MOD;
    // printf("%dwidth: %d, thresh: %d, %d\n",0,width, thresh,0);
    for (int i=0; i<thresh; i++){
        x[i%width] += x[i%width];
        x[i%width] = x[i%width] % 255;
    }
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array1_const_few_comp(  long int * __restrict__ x, int width, int height ) { //NOT TO BE USED 
  #pragma omp parallel
  {
    int thresh = W*H+ITERATIONS*W*H*INC_MOD;
    // printf("%dwidth: %d, thresh: %d, %d\n",0,width, thresh,0);
    for (int i=0; i<thresh; i++){
        x[0] += x[width-1];
        x[0] = x[width-1] % 255;
    }
  }
}
#pragma omp end declare target
/*---------------------------------------*/

#pragma omp declare target 
void array1_comp(  long int * __restrict__ x, int width, int height ) {
  #pragma omp parallel
  {
    // for (int i=0; i<width; i++){
    //     x[i] += x[i]%255;
    // }
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array2_comp(  long int * __restrict__ x, long int * __restrict__ y, int width, int height) {
  #pragma omp parallel
  {
    // for (int i=0; i<width; i++){
    //   x[i] += x[i]%255;
    //   y[i] += y[i]%255;
    // }
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array4_comp(  long int * __restrict__ x, long int * __restrict__ y, long int * __restrict__ z,  long int * __restrict__ u, int width, int height) {
  #pragma omp parallel
  {
  //  for (int i=0; i<width; i++){
  //     x[i] += x[i]%255;
  //     y[i] += y[i]%255;
  //     z[i] += z[i]%255;
  //     u[i] += u[i]%255;
  //  }
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array8_comp(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
                   long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
                  int width, int height) {
  #pragma omp parallel
  {
    // for (int i=0; i<width; i++){
    //     a1[i] += a1[i]%255;
    //     a2[i] += a2[i]%255;
    //     a3[i] += a3[i]%255;
    //     a4[i] += a4[i]%255;
    //     a5[i] += a5[i]%255;
    //     a6[i] += a6[i]%255;
    //     a7[i] += a7[i]%255;
    //     a8[i] += a8[i]%255;
    // }
  }
}
#pragma omp end declare target
/*--------------------------------------*/
#pragma omp declare target 
void array16_comp(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
                    long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
                    long int * __restrict__ a9, long int * __restrict__ a10, long int * __restrict__ a11, long int * __restrict__ a12,\
                    long int * __restrict__ a13, long int * __restrict__ a14, long int * __restrict__ a15, long int * __restrict__ a16,\
                  int width, int height) {
  #pragma omp parallel
  {
    // for (int i=0; i<width; i++){
    //   a1[i] += a1[i]%255;
    //   a2[i] += a2[i]%255;
    //   a3[i] += a3[i]%255;
    //   a4[i] += a4[i]%255;
    //   a5[i] += a5[i]%255;
    //   a6[i] += a6[i]%255;
    //   a7[i] += a7[i]%255;
    //   a8[i] += a8[i]%255;
    //   a9[i] += a9[i]%255;
    //   a10[i] += a10[i]%255;
    //   a11[i] += a11[i]%255;
    //   a12[i] += a12[i]%255;
    //   a13[i] += a13[i]%255;
    //   a14[i] += a14[i]%255;
    //   a15[i] += a15[i]%255;
    //   a16[i] += a16[i]%255;
    // }
  }
}
#pragma omp end declare target
/*--------------------------------------*/
/*---------------------------------------*/
#pragma omp declare target 
void array1(  long int * __restrict__ x, int width, int height ) {
  #pragma omp parallel
  {
    x[0] = x[width-1];
    x[1] = x[width-2];
    x[2] = x[width-3];
    x[3] = x[width-4];
    x[4] = x[width-5];
    x[5] = x[width-6];
    x[6] = x[width-7];
    x[7] = x[width-8];
    x[8] = x[width-9];
    x[9] = x[width-10];
    x[10] = x[width-11];
    x[11] = x[width-12];
    x[12] = x[width-13];
    x[13] = x[width-14];
    x[14] = x[width-15];
    x[15] = x[width-16];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array2(  long int * __restrict__ x, long int * __restrict__ y, int width, int height) {
  #pragma omp parallel
  {
    x[0] = y[width-1];
    x[1] = y[width-2];
    x[2] = y[width-3];
    x[3] = y[width-4];
    x[4] = y[width-5];
    x[5] = y[width-6];
    x[6] = y[width-7];
    x[7] = y[width-8];
    y[0] = x[width-1];
    y[1] = x[width-2];
    y[2] = x[width-3];
    y[3] = x[width-4];
    y[4] = x[width-5];
    y[5] = x[width-6];
    y[6] = x[width-7];
    y[7] = x[width-8];
  }
}
#pragma omp end declare target
/*--------------------------------------*/
#pragma omp declare target 
void array3(  long int * __restrict__ x, long int * __restrict__ y, long int * __restrict__ z, int width, int height) {
  #pragma omp parallel
  {
    x[0] = y[width-1];
    y[0] = z[width-1];
    z[0] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array4(  long int * __restrict__ x, long int * __restrict__ y, long int * __restrict__ z,  long int * __restrict__ u, int width, int height) {
  #pragma omp parallel
  {
    x[0] = y[width-1];
    x[1] = y[width-2];
    x[2] = y[width-3];
    x[3] = y[width-4];
    y[0] = z[width-1];
    y[1] = z[width-2];
    y[2] = z[width-3];
    y[3] = z[width-4];
    z[0] = u[width-1];
    z[1] = u[width-2];
    z[2] = u[width-3];
    z[3] = u[width-4];
    u[0] = x[width-1];
    u[1] = x[width-2];
    u[2] = x[width-3];
    u[3] = x[width-4];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
#pragma omp declare target 
void array5(  long int * __restrict__ x, long int * __restrict__ y, long int * __restrict__ z, long int * __restrict__ u, long int * __restrict__ w, int width, int height) {
  #pragma omp parallel
  {
    x[0] = y[width-1];
    y[0] = z[width-1];
    z[0] = u[width-1];
    u[0] = w[width-1];
    w[0] = x[width-1];
  }
}
#pragma omp end declare target
/*---------------------------------------*/
/*---------------------------------------*/
#pragma omp declare target 
void array8(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
              long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
             int width, int height) {
  #pragma omp parallel
  {
    a1[0] = a2[width-1];
    a2[0] = a3[width-1];
    a3[0] = a4[width-1];
    a4[0] = a5[width-1];
    a5[0] = a6[width-1];
    a6[0] = a7[width-1];
    a7[0] = a8[width-1];
    a8[0] = a1[width-1];
    a1[1] = a2[width-2];
    a2[1] = a3[width-2];
    a3[1] = a4[width-2];
    a4[1] = a5[width-2];
    a5[1] = a6[width-2];
    a6[1] = a7[width-2];
    a7[1] = a8[width-2];
    a8[1] = a1[width-2];
  }
}
#pragma omp end declare target
/*--------------------------------------*/
#pragma omp declare target 
void array16(  long int * __restrict__ a1, long int * __restrict__ a2, long int * __restrict__ a3, long int * __restrict__ a4,\
                    long int * __restrict__ a5, long int * __restrict__ a6, long int * __restrict__ a7, long int * __restrict__ a8,\
                    long int * __restrict__ a9, long int * __restrict__ a10, long int * __restrict__ a11, long int * __restrict__ a12,\
                    long int * __restrict__ a13, long int * __restrict__ a14, long int * __restrict__ a15, long int * __restrict__ a16,\
                  int width, int height) {
  #pragma omp parallel
  {
    a1[0] = a2[width-1];
    a2[0] = a3[width-1];
    a3[0] = a4[width-1];
    a4[0] = a5[width-1];
    a5[0] = a6[width-1];
    a6[0] = a7[width-1];
    a7[0] = a8[width-1];
    a8[0] = a9[width-1];
    a9[0] = a10[width-1];
    a10[0] = a11[width-1];
    a11[0] = a12[width-1];
    a12[0] = a13[width-1];
    a13[0] = a14[width-1];
    a14[0] = a15[width-1];
    a15[0] = a16[width-1];
    a16[0] = a1[width-1];
  }
}
#pragma omp end declare target
/*======================================================*/
#pragma omp declare target 
void hellowerld(  long int * __restrict__ x, int width) {
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
void offl_const_var(  long int * __restrict__ x, int width) {
  #pragma omp parallel
  {
    x[0] = 15;
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
  unsigned div = 1;
  for (int i=1; i<ARGUS; i++)
    div *= 2;
  unsigned width          = W / div;
  unsigned height         = H;
  // printf("ARGUS: %d", div);
  long int* l3_in1;
  long int* l3_in2;
  long int* l3_in3;
  long int* l3_in4;
  long int* l3_in5;
  long int* l3_in6;
  long int* l3_in7;
  long int* l3_in8;
  long int* l3_in9;
  long int* l3_in10;
  long int* l3_in11;
  long int* l3_in12;
  long int* l3_in13;
  long int* l3_in14;
  long int* l3_in15;
  long int* l3_in16;
  /* Allocation */
  clock_gettime(CLOCK_REALTIME, &t_hmall_0);
  if(ARGUS == 1)
    l3_in1      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int));
  else{
      l3_in1      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in2      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
    if (ARGUS > 2)
      l3_in3      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in4      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
    if (ARGUS > 3)
      l3_in5      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in6      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in7      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in8      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
    if (ARGUS > 4)
      l3_in9      = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in10     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in11     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in12     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in13     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in14     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in15     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
      l3_in16     = ( long int*)malloc(MAX_H*MAX_W*sizeof(long int)/ARGUS);
  } 
  clock_gettime(CLOCK_REALTIME, &t_hmall_1);

  /* Init Array */
  if (ARGUS == 1){
    for(int i=0; i<MAX_H*MAX_W; i++)
      l3_in1[i] = rand() % 255; 
  } else{
    for (int i=0; i<MAX_H*MAX_W/div; i++){
      l3_in1[i] = rand() % 255;
      l3_in2[i] = rand() % 255;
    }
    if (ARGUS > 2){
      for (int i=0; i<MAX_H*MAX_W/div; i++){
        l3_in3[i] = rand() % 255;
        l3_in4[i] = rand() % 255;
      }
    }
    if (ARGUS > 3){
      for (int i=0; i<MAX_H*MAX_W/div; i++){
        l3_in5[i] = rand() % 255;
        l3_in6[i] = rand() % 255;
        l3_in7[i] = rand() % 255;
        l3_in8[i] = rand() % 255;
      }
    }
    if (ARGUS > 4){
      for (int i=0; i<MAX_H*MAX_W/div; i++){
        l3_in9[i] = rand() % 255;
        l3_in10[i] = rand() % 255;
        l3_in11[i] = rand() % 255;
        l3_in12[i] = rand() % 255;
        l3_in13[i] = rand() % 255;
        l3_in14[i] = rand() % 255;
        l3_in15[i] = rand() % 255;
        l3_in16[i] = rand() % 255;
      }
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
  for (int i=0; i<ITERATIONS;i++){
    // printf("ITERATION:%d\n",i);
    if (!OTHER_TESTS){
      if(HEAVY_COMP==1){
        if(!IFTOFROM){
          /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height])
              array1_comp(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2_comp(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4_comp(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }
        }else{ //TOFROM
          /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height])
              array1_comp(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2_comp(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4_comp(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }
        }
      }else if(HEAVY_COMP==0){
        if(!IFTOFROM){
          /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height])
              array1(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }
        }else{
                /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height])
              array1(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }
        }
      }
    }else{
      if(HEAVY_COMP){
        if(!IFTOFROM){
          if (ARGUS == 1){
              clock_gettime(CLOCK_REALTIME, &t_offl_0);
              #pragma omp target  map(tofrom: l3_in1[0:width*height])
                array1_const_all_comp(l3_in1, width, height );
              clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }else{
            printf("\nMultiple Arguments NOT SUPPORTED!\n");
            return 0;
          } 
        }else{
          if (ARGUS == 1){
              clock_gettime(CLOCK_REALTIME, &t_offl_0);
              #pragma omp target  map(to: l3_in1[0:width*height])
                array1_const_all_comp(l3_in1, width, height );
              clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }else{
            printf("\nMultiple Arguments NOT SUPPORTED!\n");
            return 0;
          } 
        }
      }else{ //OTHER TESTS : !HEAVY_COMP   AKA NO COMP
        if(IFTOFROM){
                /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height])
              array1_no_comp(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2_no_comp(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4_no_comp(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8_no_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(tofrom: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16_no_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          }
        }else{
                           /* OFFLOAD */
          if (ARGUS == 1){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height])
              array1_no_comp(l3_in1, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 2){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height]) nowait
              array2_no_comp(l3_in1, l3_in2, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 3){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height]) nowait
              array4_no_comp(l3_in1, l3_in2, l3_in3, l3_in4, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 4){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height]) nowait
              array8_no_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } else if(ARGUS == 5){
            clock_gettime(CLOCK_REALTIME, &t_offl_0);
            #pragma omp target  map(to: l3_in1[0:width*height], l3_in2[0:width*height], l3_in3[0:width*height], l3_in4[0:width*height],\
                                                              l3_in5[0:width*height], l3_in6[0:width*height], l3_in7[0:width*height], l3_in8[0:width*height],\
                                                              l3_in9[0:width*height], l3_in10[0:width*height], l3_in11[0:width*height], l3_in12[0:width*height],\
                                                              l3_in13[0:width*height], l3_in14[0:width*height], l3_in15[0:width*height], l3_in16[0:width*height]) nowait
              array16_no_comp(l3_in1, l3_in2, l3_in3, l3_in4,l3_in5, l3_in6, l3_in7, l3_in8,l3_in9, l3_in10, l3_in11, l3_in12,l3_in13, l3_in14, l3_in15, l3_in16, width, height );
            clock_gettime(CLOCK_REALTIME, &t_offl_1);
          } 
        }
      }
    } 
    /* Evaluation of Results */
    if (VERBO && IFTOFROM){
      if (ARGUS == 1)
        printf("%d\n",l3_in1[0] == l3_in1[width-1]);
      if (ARGUS == 2){
        printf("%d\n",l3_in1[0] == l3_in2[width-1]);
        printf("%d\n",l3_in2[0] == l3_in1[width-1]);
      }
      else if (ARGUS == 3){
        printf("%d\n",l3_in1[0] == l3_in2[width-1]);
        printf("%d\n",l3_in2[0] == l3_in3[width-1]);
        printf("%d\n",l3_in3[0] == l3_in4[width-1]);
      }
      else if (ARGUS == 4){
        printf("%d\n",l3_in1[0] == l3_in2[width-1]);
        printf("%d\n",l3_in2[0] == l3_in3[width-1]);
        printf("%d\n",l3_in3[0] == l3_in4[width-1]);
        printf("%d\n",l3_in4[0] == l3_in5[width-1]);
      }
      else if (ARGUS == 5){
        printf("%d\n",l3_in1[0] == l3_in2[width-1]);
        printf("%d\n",l3_in2[0] == l3_in3[width-1]);
        printf("%d\n",l3_in3[0] == l3_in4[width-1]);
        printf("%d\n",l3_in4[0] == l3_in5[width-1]);
        printf("%d\n",l3_in5[0] == l3_in6[width-1]);
      }

     
      for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
          printf("%d ",l3_in1[i*width+j]);
        }
        printf("\n");
      }
      printf("\n");

      if (ARGUS > 1){    
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in2[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
      }
      if (ARGUS > 2){    
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in3[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in4[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
      }
      if (ARGUS > 3){
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in5[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in6[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in7[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in8[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
      }
      if (ARGUS > 4){
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in9[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in10[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in11[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in12[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in13[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in14[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in15[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
        for (int i=0; i<height; i++){
          for (int j=0; j<width; j++){
            printf("%d ",l3_in16[i*width+j]);
          }
          printf("\n");
        }
        printf("\n"); 
      }
    }
    
    /*INCREMENT DATA*/
    width += (W/div)*INC_MOD;
    width *= MUL_MOD;
    

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
