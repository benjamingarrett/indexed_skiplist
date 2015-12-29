/*
 * File:   skiplist_test_key_comparisons.c
 * Author: benjamin
 *
 * Created on Nov 9, 2015, 12:06:04 AM
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RUN_TEST  

uint64_t NON_VALUE = -1;
uint64_t NON_POSITION = -1;
uint64_t KEY_LENGTH = 4;
uint64_t VALUE_LENGTH = 8;

void test1() {
    
    int g, num_trials, num_successes;
    unsigned char * k1, * k2;
    
    printf("skiplist_test_key_comparisons\n");
    initialize_skiplist(10, &NON_VALUE, &NON_POSITION, KEY_LENGTH, VALUE_LENGTH);
    num_trials = num_successes = 0;
    k1 = (unsigned char *)calloc(KEY_LENGTH, sizeof(unsigned char));
    k2 = (unsigned char *)calloc(KEY_LENGTH, sizeof(unsigned char));
    
    
    if( less_than(k1,k2) ){
        printf("%d: failure!\n", num_trials);
    } else {
        num_successes++;
        printf("%d: success!\n", num_trials);
    }
    num_trials++;
    
    
    *k2 = 1;
    if( less_than(k1,k2) ){
        num_successes++;
        printf("%d: success!\n", num_trials);
    } else {
        printf("%d: failure!\n", num_trials);
    }
    num_trials++;
    
    
    *k2 = 0;
    *(k2+1) = 1;
    if( less_than(k1,k2) ){
        num_successes++;
        printf("%d: success!\n", num_trials);
    } else {
        printf("%d: failure!\n", num_trials);
    }
    num_trials++;
    
    *(k2+1) = 0;
    *(k2+2) = 1;
    if( less_than(k1,k2) ){
        num_successes++;
        printf("%d: success!\n", num_trials);
    } else {
        printf("%d: failure!\n", num_trials);
    }
    num_trials++;
    
    *(k2+2) = 0;
    *(k2+3) = 1;
    if( less_than(k1,k2) ){
        num_successes++;
        printf("%d: success!\n", num_trials);
    } else {
        printf("%d: failure!\n", num_trials);
    }
    num_trials++;
    
    *(k1+3) = 1;
    *(k1+2) = 1;
    *(k1+1) = 1;
    *k1 = 1;
    *(k2+3) = 1;
    *(k2+2) = 1;
    *(k2+1) = 1;
    *k2 = 1;
    if( less_than(k1,k2) ){
        printf("%d: failure!\n", num_trials);
    } else {
        num_successes++;
        printf("%d: success!\n", num_trials);
    }
    num_trials++;
    
    *(k2+3) = 0;
    *(k1+2) = 0;
    if( less_than(k1,k2) ){
        printf("%d: failure!\n", num_trials);
    } else {
        num_successes++;
        printf("%d: success!\n", num_trials);
    }
    num_trials++;
    
    *(k1+3) = 0;
    *(k1+2) = 1;
    *(k1+1) = 0;
    *k1     = 1;
    *(k2+3) = 1;
    *(k2+2) = 0;
    *(k2+1) = 1;
    *k2     = 0;
    if( less_than(k1,k2) ){
        num_successes++;
        printf("%d: success!\n", num_trials);
    } else {
        printf("%d: failure!\n", num_trials);
    }
    num_trials++;
    
    *(k1+3) = 1;
    *(k1+2) = 0;
    *(k1+1) = 1;
    *k1     = 0;
    *(k2+3) = 0;
    *(k2+2) = 1;
    *(k2+1) = 0;
    *k2     = 1;
    if( less_than(k1,k2) ){
        printf("%d: failure!\n", num_trials);
    } else {
        num_successes++;
        printf("%d: success!\n", num_trials);
    }
    num_trials++;
    
    printf("trials: %d  successes: %d\n", num_trials, num_successes);
    
    
}

int main(int argc, char** argv) {
    
    #ifdef RUN_TEST
        printf("%%TEST_STARTED%% test1 (skiplist_test_key_comparisons)\n");
        test1();
        printf("%%TEST_FINISHED%% time=0 test1 (skiplist_test_key_comparisons) \n");
    #endif
    return (EXIT_SUCCESS);
}