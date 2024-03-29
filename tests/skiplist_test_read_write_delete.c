/*
 * File:   skiplist_test_read_write_delete.c
 * Author: benjamin
 *
 * Created on Nov 9, 2015, 12:08:30 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define RUN_TEST  


#define VIEW_PROGRESS  
#define READ 0
#define WRITE 1
#define DELETE 2
#define FAILURE 0
#define SUCCESS 1

uint64_t NON_VALUE = -1;
uint64_t NON_POSITION = -1;
uint64_t KEY_LENGTH = 4;
uint64_t VALUE_LENGTH = 8;
int max_levels = 10;

void test1() {
    
    FILE * fp;
    uint64_t max_key, capacity, max_load, * key, g, h, num_anomalies, num_trials;
    int type, * operation, * status, * result, num_operations, deletions_enabled;
    unsigned char * k;
    unsigned char KEY[4];
    uint64_t * vp, val;
    
    printf("skiplist_test_read_write_delete\n");
//    fp = fopen("../misc_phd/input/operation_sequences/feedback/deletions/100/operation_sequence_with_feedback-100-20-32-12","r");
    fp = fopen("../misc_phd/input/operation_sequences/feedback/deletions/100000/operation_sequence_with_feedback-100000-80000-131072-22587","r");
    if(fp==NULL){
        printf("fp is null!\n");
        exit(1);
    }
    fscanf(fp, "%d\n", &type);
    if(type == 0){
        printf("Artificial feedback detected\n");
    }
    fscanf(fp, "%d\n", &num_operations);
    fscanf(fp, "%d\n", &max_key);
    fscanf(fp, "%d\n", &capacity);
    fscanf(fp, "%d\n", &max_load);
    fscanf(fp, "%d\n", &deletions_enabled);
    printf("%d %d %d %d\n", num_operations, max_key, capacity, max_load);
    operation = calloc(num_operations, sizeof(int));
    key = calloc(num_operations, sizeof(uint64_t));
    status = calloc(num_operations, sizeof(int));
    result = calloc(num_operations, sizeof(int));
    if(operation==NULL)printf("operation is null\n");
    if(key==NULL)printf("key is null\n");
    if(status==NULL)printf("status is null\n");
    if(result==NULL)printf("result is null\n");
    for(g=0; g<num_operations; g++){
        fscanf(fp, "%d %d %d\n", &operation[g], &key[g], &status[g]);
    }
    fclose(fp);
    for(g=0; g<num_operations; g++){
        printf("%d %d %d %d\n", g, operation[g], key[g], status[g]);
    }
    initialize_skiplist_long_int();
    #ifdef VIEW_PROGRESS
        printf("Starting trials\n");
    #endif
    
/*
    num_operations = 24;
*/
    for(g=0; g<num_operations; g++){
        
        #ifdef VIEW_PROGRESS
            printf("Performing operation: %d %d %d %d\n", g, operation[g], key[g], status[g]);
        #endif

//        k = (unsigned char *)&key[g];
//        for(h=0; h<4; h++){
//            KEY[h] = *(k+h);
//        }
        
        switch(operation[g]){
            case READ:
                val = skiplist_read_long_int(&key[g]);
                if(val == NON_VALUE){
                    result[g] = FAILURE;
                } else {
                    result[g] = SUCCESS;
                }
                break;
            case WRITE:
                val = skiplist_write_long_int(&key[g], &key[g]);
                if(val == NON_VALUE){
                    result[g] = FAILURE;
                } else {
                    result[g] = SUCCESS;
                }
                break;
            case DELETE:
                val = skiplist_delete_long_int(&key[g]);
                if(val == NON_VALUE){
                    result[g] = FAILURE;
                } else {
                    result[g] = SUCCESS;
                }
                break;
            default:
                printf("bad operation: %d %d\n", g, operation[g]);
                exit(1);
                break;
        }
        
        #ifdef VIEW_PROGRESS
            if(status[g] != result[g]){
                printf("Anomaly found after operation %d\n", g);
            }
/*
            else {
                printf("Operation %d successful\n", g);
            }
            printf("skiplist_dump: ");
            skiplist_full_dump();
*/
        #endif
    }
        
    num_anomalies = num_trials = 0;
    for(g=0; g<num_operations; g++){
        printf("%d %d %d %d %d\n", g, operation[g], key[g], status[g], result[g]);
        if(status[g]!=result[g]){
            num_anomalies++;
        }
        num_trials++;
    }
    printf("trials: %d  anomalies: %d\n", num_trials, num_anomalies);
    
}


int main(int argc, char** argv) {

    #ifdef RUN_TEST
        printf("%%TEST_STARTED%% (skiplist_test_read_write_delete)\n");
        test1();
        printf("%%TEST_FINISHED%% (skiplist_test_read_write_delete) \n");
    #endif
    return (EXIT_SUCCESS);
}
