#include "indexed_skiplist.h"

#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define FAILURE 1

/*
#define TRACE  
*/

uint64_t MAX_LEVELS = 10;
uint64_t MIN_KEY_LENGTH = 4;
uint64_t MIN_VALUE_LENGTH = 4;
uint64_t INDEX_LENGTH = 8;
uint64_t DATA_LENGTH;
void * NON_VALUE;
void * NON_POSITION;
uint64_t KEY_LENGTH;
uint64_t VALUE_LENGTH;

int UPDATE_WIDTHS_ON_WRITE = TRUE;

typedef struct snode {
    unsigned char * data;
    struct snode ** forward;
    int * width;
/*
    char * is_header;
*/
} snode;

typedef struct skiplist {
    int level;
    int size;
    struct snode * header;
} skiplist;

skiplist * list;

void initialize_skiplist(
            int max_levels,
            void * non_value, 
            void * non_position,
            size_t key_length, 
            size_t value_length){
    
    int i;
    
    #ifdef TRACE
        printf("initialize skiplist\n");
    #endif
    MAX_LEVELS = max_levels;
    NON_VALUE = non_value;
    NON_POSITION = non_position;
    if(key_length < MIN_KEY_LENGTH){
        KEY_LENGTH = MIN_KEY_LENGTH;
    } else {
        KEY_LENGTH = key_length;
    }
    if(value_length < MIN_VALUE_LENGTH){
        VALUE_LENGTH = MIN_VALUE_LENGTH;
    } else {
        VALUE_LENGTH = value_length;
    }
    DATA_LENGTH = KEY_LENGTH + VALUE_LENGTH + INDEX_LENGTH;
    list = (skiplist *)malloc(sizeof(struct skiplist));
    list->header = (snode *)malloc(sizeof(struct snode));
    list->header->data = (unsigned char *)calloc(DATA_LENGTH, sizeof(unsigned char));
    for(i=0; i<KEY_LENGTH; i++){
        list->header->data[i] = 0xff;
    }
    list->header->forward = (snode **)malloc(sizeof(snode*) * (MAX_LEVELS+1));
    for (i = 0; i <= MAX_LEVELS; i++) {
        list->header->forward[i] = list->header;
    }
    list->header->width = (int *)calloc(MAX_LEVELS+1, sizeof(int));
    list->level = 1;
    list->size = 0;
    
    initialize_test_levels();
}

int NUM_TEST_LEVELS, p;
int * test_level;

void initialize_test_levels(){
    
    NUM_TEST_LEVELS = 10;
    test_level = calloc(NUM_TEST_LEVELS, sizeof(int));
    test_level[0] = 1;
    test_level[1] = 2;
    test_level[2] = 1;
    test_level[3] = 3;
    test_level[4] = 1;
    test_level[5] = 2;
    test_level[6] = 3;
    test_level[7] = 1;
    test_level[8] = 2;
    test_level[9] = 3;
}

int choose_level(){
    
    int level;
    
    level = test_level[p++%NUM_TEST_LEVELS];
/*
    level = rand_level();
    level = 1;
*/
    
    #ifdef TRACE
        printf("skiplist choose level = %d\n", level);
    #endif
    return level;
}

int rand_level(){
    
    int level = 1;
    while (rand() < RAND_MAX/2 && level < MAX_LEVELS){
        level++;
    }
    return level;
}

void skiplist_node_free(snode *x){
    
    if (x) {
        free(x->forward);
        free(x);
    }
}

void view_key( unsigned char * data ){
    
    int g;
    
    for(g=0; g<KEY_LENGTH; g++){
        printf("%x ", data[g]);
    }
}

void view_data(unsigned char * k){
    
    int g;
    
    for(g=KEY_LENGTH; g<(KEY_LENGTH+VALUE_LENGTH); g++){
        printf("%x ", *(k+g));
    }
}

void skiplist_dump(){
    
    int g;
    snode * x;
    
    x = list->header;
    while (x && x->forward[1] != list->header) {
        printf("[");
        view_key(x->forward[1]->data);
        printf(" - ");
        view_data(x->forward[1]->data);
        printf("]->");
        x = x->forward[1];
    }
    printf("NIL\n");
}

void skiplist_full_dump(){
    
    int g, h;
    snode * update[MAX_LEVELS + 1];
    snode * x;
    
    #ifdef TRACE
        printf("skiplist full dump, level = %d\n", list->level);
    #endif
    
    x = list->header;
    for (g = list->level; g >= 1; g--) {
        
        printf("%d: HEAD (%d)->",
                g, list->header->width[g]);
        
/*
        printf("%d: HEAD (%d),(%d)->", 
                g, list->header->width[g], list->header->is_header[g]);
*/
        
        while( x && x->forward[g] != list->header ){
            printf("[");
            view_key(x->forward[g]->data);
            printf(" - ");
            view_data(x->forward[g]->data);
            printf(" - (%d)", x->width[g]);
/*
            printf(",(%d)", x->is_header[g]);
*/
            printf("]->");
            x = x->forward[g];
        }
        printf("NIL\n");
        x = list->header;
    }
}

uint8_t equals(unsigned char * k1, unsigned char * k2){
    
    int g;
    
/*
    printf("equals\n");
    printf("first value: ");
    for(g=0; g<KEY_LENGTH; g++){
        printf("%x ", *(k1+g));
    }
    printf("\nsecond value: ");
    for(g=0; g<KEY_LENGTH; g++){
        printf("%x ", *(k2+g));
    }
    printf("\n");
*/
    
    for(g=0; g<KEY_LENGTH; g++){
        if( *(k1+g) != *(k2+g) ){
/*
            printf("equals returning FALSE\n");
*/
            return FALSE;
        }
    }
/*
    printf("equals returning TRUE\n");
*/
    return TRUE;
}

uint8_t less_than(unsigned char * k1, unsigned char * k2){
    
    int g;
    
/*
    #ifdef TRACE
        printf("less than\n");
        printf("first value: ");
        for(g=0; g<KEY_LENGTH; g++){
            printf("%x ", *(k1+g));
        }
        printf("\nsecond value: ");
        for(g=0; g<KEY_LENGTH; g++){
            printf("%x ", *(k2+g));
        }
        printf("\n");
    #endif
*/
    
    for(g=KEY_LENGTH-1; g>=0; g--){
        if( *(k1+g) < *(k2+g) ){
/*
            printf("position %d is less than: returning TRUE\n", g);
            printf("less than returning TRUE\n");
*/
            return TRUE;
        } else {
            if( *(k1+g) > *(k2+g) ){
/*
                printf("position %d is greater than: returning FALSE\n", g);
                printf("less than returning FALSE\n");
*/
                return FALSE;
            } 
/*
            else {
                printf("position %d is equal skipping this position\n", g);
            }
*/
        }
    }
/*
    printf("less than returning FALSE\n");
*/
    return FALSE;
}

snode * find_prior_insertion_point(unsigned char * key){
    
    snode * x, * p;
    
    if( ! less_than(list->header->forward[list->level]->data, key) ){
        return list->header;
    }
    x = list->header->forward[list->level];
    p = x;
    while( less_than(x->forward[list->level]->data, key) ){
        p = x;
        x = x->forward[list->level];
    }
    return p;
}

snode * find_node(unsigned char * key){
    
    int i;
    snode * x;
    
    x = list->header;
    for(i=list->level; i>=1; i--){
        while( less_than(x->forward[i]->data, key) ){
            x = x->forward[i];
        }
    }
    if( equals(x->forward[1]->data, key) ){
        return x->forward[1];
    } else {
        return NULL;
    }
    return NULL;
}

void * skiplist_read(unsigned char * key){
    
    snode * x;
    int i;
    
    #ifdef TRACE
        printf("skiplist read, level = %d\n", list->level);
    #endif

    x = find_node(key);
    if( x != NULL ){
        #ifdef TRACE
            printf("skiplist read found value\n");
        #endif
        return x->data + KEY_LENGTH;
    } else {
        #ifdef TRACE
            printf("skiplist read didn't find value\n");
        #endif
        return NON_VALUE;
    }

    /*
    x = list->header;
    for (i = list->level; i >= 1; i--){
        
        #ifdef TRACE
            printf("skiplist searching list at level %d\n", i);
        #endif
        
        while( less_than(x->forward[i]->data, key) ){
            
            #ifdef TRACE
                printf("moving forward->");
            #endif
            
            x = x->forward[i];
            
        }
        
        #ifdef TRACE
            printf("\n");
        #endif
        
    }
    
    #ifdef TRACE
        printf("skiplist read about to compare key to position... ");
    #endif
    if( equals(x->forward[1]->data, key) ){
        #ifdef TRACE
            printf("skiplist read found value\n");
        #endif
        return x->forward[1]->data + KEY_LENGTH;
    } else {
        #ifdef TRACE
            printf("skiplist read didn't find value\n");
        #endif
        return NON_VALUE;
    }
     */
}

void * skiplist_write(unsigned char * key){
    
    int i, g, level;
    snode * update[MAX_LEVELS+1];
    snode * x, * y, * z;
    
    #ifdef TRACE
        printf("skiplist write\n\n");
    #endif

    x = list->header;
    for (i = list->level; i >= 1; i--){
        while( less_than(x->forward[i]->data, key) ){
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[1];
    if ( equals(key, x->data) ) {
        return x->data + KEY_LENGTH;
    } else {
        #ifdef TRACE
            printf("item not found\n");
        #endif
        level = choose_level();
        if (level > list->level) {
            for (i = list->level+1; i <= level; i++) {
                update[i] = list->header;
            }
            list->level = level;
        }
        #ifdef TRACE
            printf("done making update array:\n");
            for(i=level; i>=1; i--){
                printf("%d: ", i);view_key(update[i]->data);printf("-->");view_key(update[i]->forward[i]->data);printf("-->");view_key(update[i]->forward[i]->forward[i]->data);printf("\n");
            }
            printf("----\n");
        #endif
        x = (snode *)malloc(sizeof(snode));
        if( x == NULL ){
            return NON_POSITION;
        }
        x->data = (unsigned char *)calloc(DATA_LENGTH, sizeof(unsigned char));
        for(g=0; g<KEY_LENGTH; g++){
            *(x->data+g) = *(key+g);
        }
        x->forward = (snode**)malloc( sizeof(snode*) * (level + 1) );
        x->width = (int *)calloc(level + 1, sizeof(int));
        for(i=1; i<=level; i++){
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
        x->width[1] = 1;
        
        #ifdef TRACE
            printf("Just pointed update to new data\n");
            for(i=level; i>=1; i--){
                printf("%d: ", i);view_key(update[i]->data);printf("-->");view_key(update[i]->forward[i]->data);printf("-->");view_key(update[i]->forward[i]->forward[i]->data);printf("\n");
            }
            printf("----\n");
            printf("skiplist_dump: ");
            skiplist_full_dump();
            printf("\n\n");
        #endif
        
        if(UPDATE_WIDTHS_ON_WRITE){
        
            #ifdef TRACE
                printf("skiplist write: affecting new widths, level = %d\n", level);
            #endif
        
            for(i=2; i<=level; i++){
                
                #ifdef TRACE
                    printf("skiplist_write: affecting new widths on level %d\n", i);
                #endif
                
                
                    
                    
                y = x;
                x->width[i] = y->width[i-1];
                #ifdef TRACE
                    printf("initializing new width to %d of node: ", y->width[i-1]);view_key(y->data);printf("\n");/*printf("(%d)\n", y->is_header[i-1]);*/
                    printf("first comparison\n");
                    if( less_than( x->forward[i]->data, y->forward[i-1]->data ) ){
                        view_key(x->forward[i]->data);printf(" < ");view_key(y->forward[i-1]->data);printf("\n");
                    } else {
                        view_key(x->forward[i]->data);printf(" >= ");view_key(y->forward[i-1]->data);printf("\n");
                    }
                #endif
                while( less_than( x->forward[i]->data, y->forward[i-1]->data ) ){
                    
                    #ifdef TRACE
                        view_key(x->forward[i]->data);printf(" < ");view_key(y->forward[i-1]->data);printf("\n");
                    #endif
                    y = y->forward[i-1];
                    x->width[i] += y->width[i-1];
                    #ifdef TRACE
                        printf("Adding %d to new width of node: ");view_key(y->data);printf("\n");
                    #endif
                }
            }

                
            #ifdef TRACE
                printf("skiplist_dump: ");
                skiplist_full_dump();
                printf("\n\n");
            #endif
                
                
            #ifdef TRACE
                printf("skiplist write: affecting old widths, level = %d\n", level);
            #endif
                
            update[1]->width[1] = 1;
                
            for(i=2; i<=level; i++){
                
                #ifdef TRACE
                    printf("skiplist_write: affecting old widths on level %d\n", i);
                #endif
                
                y = update[i-1];
                z = update[i];

                #ifdef TRACE
                    printf("update array and successors\n");
                    for(g=level; g>=1; g--){
                        printf("%d: ", g);view_key(update[g]->data);printf("-->");view_key(update[g]->forward[g]->data);printf("-->");view_key(update[g]->forward[g]->forward[g]->data);printf("\n");
                    }
                    printf("----\n");
                    printf("update[%d]->data = ", i);view_key(z->data);printf("\n"); 
                    printf("update[%d]->data = ", i-1);view_key(y->data);printf("\n");
                #endif
                
                
                z = list->header;
                y = list->header;
                                
                int temp_width = y->width[i-1];
                
                #ifdef TRACE
                    printf("initializing temp width to %d of node: ", y->width[i-1]);view_key(y->data);printf("\n");/*printf("(%d)\n", y->is_header[i-1]);*/
                    if( less_than(y->forward[i-1]->data, z->forward[i]->data) ){
                        view_key(y->forward[i-1]->data);printf(" < ");view_key(z->forward[i]->data);printf("\n");
                    } else {
                        view_key(y->forward[i-1]->data);printf(" >= ");view_key(z->forward[i]->data);printf("\n");
                    }
                #endif
                while( less_than(y->forward[i-1]->data, z->forward[i]->data) ){
                    #ifdef TRACE
                        view_key(y->forward[i-1]->data);printf(" < ");view_key(z->forward[i]->data);printf("\n");
                    #endif
                    y = y->forward[i-1];
                    temp_width += y->width[i-1];
                    #ifdef TRACE
                        printf("Adding %d to temp width of node: ", y->width[i-1]);view_key(y->data);printf("\n");/*printf("(%d)\n", y->is_header[i-1]);*/
                    #endif
                }
                z->width[i] = temp_width;
            }

        }

        return x->data + KEY_LENGTH;
    }
}

int8_t skiplist_delete(unsigned char * key){
    
    int i;
    snode *update[MAX_LEVELS + 1];
    snode *x;
    
    int tmp_cnt;
    
    #ifdef TRACE
        printf("skiplist_delete\n");
        skiplist_full_dump();
    #endif
    
    
    
    x = list->header;
    
    #ifdef TRACE
        printf("first value is: ");
        view_data(x->data);
        printf("\n");
        printf("next value is: ");
        if(x->forward){
            view_data(x->forward[list->level]->data);
            printf("\n");
        } else {
            printf("NIL\n");
        }
    #endif
    
    for (i = list->level; i >= 1; i--) {
        
        #ifdef TRACE
            printf("finding key at level %d\n", i);
            tmp_cnt = 0;
        #endif
        
        while( less_than(x->forward[i]->data, key) ){
            
            #ifdef TRACE
                printf("moving forward at level %d, because data is: ", i);
                view_data(x->forward[i]->data);
                printf("\n");
                tmp_cnt++;
            #endif
            
            x = x->forward[i];
        }
        
        #ifdef TRACE
            printf("found key at level %d after %d iterations\n", i, tmp_cnt);
        #endif

        update[i] = x;
        
        #ifdef TRACE
            printf("update[%d] is now: ", i);
            view_data(update[i]->data);
            printf("\n");
        #endif
        
    }
    
    x = x->forward[1];
    
    if ( equals(x->data, key) ) {
        
        #ifdef TRACE
            printf("skiplist delete found key, level = %d\n", list->level);
        #endif
        
        for (i = 1; i <= list->level; i++) {
            
            #ifdef TRACE
                printf("skiplist delete loop %d: ", i);
            #endif
            
            if (update[i]->forward[i] != x){
                
                #ifdef TRACE
                    printf("this isn't the intended node. Break!\n");
                #endif
                break;
            }
            
            #ifdef TRACE
                if( ! x->forward[i] ){
                    printf("skiplist delete x->forward doesn't exist\n");
                } else {
                    printf("skiplist delete x->forward has value: ");
                    view_data( x->forward[i]->data );
                    printf("\n");
                }
            #endif
            
            update[i]->forward[i] = x->forward[i];
            
        }
        
        skiplist_node_free(x);
        
        while (list->level > 1 && list->header->forward[list->level] == list->header){
            list->level--;
            
            #ifdef TRACE
                printf("skiplist delete decrementing level to %d\n", list->level);
            #endif
        }
        return SUCCESS;
    }
    return FAILURE;
}

int64_t index_of(unsigned char * key){
    
    snode *x;
    int i, index;
    
    #ifdef TRACE
        printf("skiplist index of, level = %d\n", list->level);
        printf("1...\n");
    #endif

    x = list->header;
    index = 0;
    for (i = list->level; i >= 1; i--){
        
        #ifdef TRACE
            printf("skiplist searching list at level %d\n", i);
        #endif
        
        while( less_than(x->forward[i]->data, key) ){
            
            #ifdef TRACE
                printf("moving forward->");
            #endif
            
            index += x->width[i];
            x = x->forward[i];
            
        }
        
        #ifdef TRACE
            printf("\n");
        #endif
        
    }
    
    #ifdef TRACE
        printf("skiplist read about to compare key to position... ");
    #endif
    if( equals(x->forward[1]->data, key) ){
        #ifdef TRACE
            printf("skiplist read found value at index %d\n", index);
        #endif
        return index;
    } else {
        #ifdef TRACE
            printf("skiplist read didn't find value\n");
        #endif
        return -1;
    }
}