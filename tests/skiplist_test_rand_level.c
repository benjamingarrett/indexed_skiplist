/*
 * File:   skiplist_test_rand_level.c
 * Author: benjamin
 *
 * Created on Nov 9, 2015, 12:07:26 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
#define RUN_TEST  
*/

void test1() {
    
    int g, h, level, count[10];
    
    printf("skiplist_test_rand_level\n");
    for(g=0; g<10; g++){
        count[g] = 0;
    }
    for(g=0; g<200000; g++){
        level = rand_level();
        count[level]++;
/*
        printf("%d: %d\n", g, level);
*/
    }
/*
    for(g=0; g<10; g++){
        printf("%d: %d\n", g, count[g]);
    }
*/
    for(g=1; g<10; g++){
        for(h=1; h<g; h++){
            printf("checking if %d > %d...", h, g);
            if( count[h] < count[g] ){
                printf("failure!\n");
            } else {
                printf("OK\n");
            }
        }
    }
}

int main(int argc, char** argv) {

    #ifdef RUN_TEST
        printf("%%TEST_STARTED%% (skiplist_test_rand_level)\n");
        test1();
        printf("%%TEST_FINISHED%% (skiplist_test_rand_level) \n");
    #endif
    return (EXIT_SUCCESS);
}
