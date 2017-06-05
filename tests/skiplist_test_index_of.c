/*
 * File:   skiplist_test_index_of.c
 * Author: benjamin
 *
 * Created on Dec 29, 2015, 12:33:30 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RUN_TEST  

/*
#define VIEW_PROGRESS  
*/
#define READ 0
#define WRITE 1
#define DELETE 2
#define FAILURE 0
#define SUCCESS 1

void test1() {
    
    FILE * fp;
    int64_t max_key, capacity, max_load, * key, g, h, num_anomalies, num_trials;
    int64_t type, * operation, * status, * result, num_operations, actual_index, * expected_index;
    int64_t index_trials, index_anomalies;
    int64_t * index_anomaly;
    unsigned char * k;
    unsigned char KEY[4];
    int64_t * val;
    
    printf("skiplist_test_index_of\n");
    fp = fopen("../misc_phd/input/operation_sequences/operation_sequence_with_feedback_and_indexes","r");
    if(fp==NULL){
        printf("fp is null!\n");
        exit(1);
    }
    fscanf(fp, "%d\n", &type);
    if(type == 0){
        printf("Artificial feedback detected\n");
    }
    fscanf(fp, "%d\n", &num_operations);
    fscanf(fp, "%ld\n", &max_key);
    fscanf(fp, "%ld\n", &capacity);
    fscanf(fp, "%ld\n", &max_load);
    printf("%d %ld %ld %ld\n", num_operations, max_key, capacity, max_load);
    operation = calloc(num_operations, sizeof(int));
    key = calloc(num_operations, sizeof(uint64_t));
    status = calloc(num_operations, sizeof(int));
    result = calloc(num_operations, sizeof(int));
    expected_index = calloc(num_operations, sizeof(int));
    index_anomaly = calloc(num_operations, sizeof(int));
    index_trials = index_anomalies = 0;
    if(operation==NULL)printf("operation is null\n");
    if(key==NULL)printf("key is null\n");
    if(status==NULL)printf("status is null\n");
    if(result==NULL)printf("result is null\n");
    for(g=0; g<num_operations; g++){
        fscanf(fp, "%d %d %d %d\n", &operation[g], &key[g], &status[g], &expected_index[g]);
    }
    fclose(fp);
    for(g=0; g<num_operations; g++){
        printf("%d %d %d %d %d\n", g, operation[g], key[g], status[g], expected_index[g]);
    }
    initialize_skiplist(
        max_levels, &NON_VALUE, &NON_POSITION, KEY_LENGTH, VALUE_LENGTH);
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

        k = (unsigned char *)&key[g];
        for(h=0; h<4; h++){
            KEY[h] = *(k+h);
        }
        
        switch(operation[g]){
            case READ:
                #ifdef VIEW_PROGRESS
                    printf("Read %d result %d\n", key[g], status[g]);
                #endif
                vp = (uint64_t *)skiplist_read(KEY);
                val = *vp;
                if(val == NON_VALUE){
                    result[g] = FAILURE;
                } else {
                    result[g] = SUCCESS;
                    actual_index = index_of(KEY);
                    index_trials++;
                    if(actual_index != expected_index[g]){
                        index_anomaly[index_anomalies] = g;
                        index_anomalies++;
                        printf("actual_index (%d) != expected_index (%d)  ANOMALY!!!\n", actual_index, expected_index[g]);
                    } else {
                        printf("actual_index (%d) == expected_index (%d)\n", actual_index, expected_index[g]);
                    }
                }
                break;
            case WRITE:
                #ifdef VIEW_PROGRESS
                    printf("Write %d result %d\n", key[g], status[g]);
                #endif
                vp = (uint64_t *)skiplist_read(KEY);
                val = *vp;
                if(val == NON_VALUE){
                    vp = (uint64_t *)skiplist_write(KEY);
                    if( *vp != NON_POSITION ){
                        *vp = key[g];
                        result[g] = SUCCESS;
                    } else {
                        result[g] = FAILURE;
                    }
                } else {
                    result[g] = FAILURE;
                }
                break;
            case DELETE:
                #ifdef VIEW_PROGRESS
                    printf("Delete %d result %d\n", key[g], status[g]);
                #endif
                vp = (uint64_t *)skiplist_read(KEY);
                val = *vp;
                if(val == NON_VALUE){
                    result[g] = FAILURE;
                } else {
                    if( skiplist_delete(KEY) == 0 ){
                        result[g] = SUCCESS;
                    } else {
                        result[g] = FAILURE;
                    }
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
*/
            printf("\n\ntest code\n");
            printf("After operation: %d %d %d %d - ", g, operation[g], key[g], status[g]);
            view_key(KEY);printf("\n");
            printf("skiplist_dump:\n");
            
/*
            skiplist_full_dump();
*/
            
            skiplist_premium_dump();
            
            printf("\n\n========\n\n");
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
    printf("trials:       %d  anomalies:       %d\n", num_trials, num_anomalies);
    printf("index trials: %d  index anomalies: %d\n", index_trials, index_anomalies);
    if(index_anomalies>0){
        printf("Index anomalies occurred at: ");
        for(g=0; g<index_anomalies; g++){
            printf("%d ", index_anomaly[g]);
        }
        printf("\n");
    }
}


int main(int argc, char** argv) {

    #ifdef RUN_TEST
        printf("%%TEST_STARTED%% (skiplist_test_index_of)\n");
        test1();
        printf("%%TEST_FINISHED%% (skiplist_test_index_of) \n");
    #endif
    return (EXIT_SUCCESS);
}

