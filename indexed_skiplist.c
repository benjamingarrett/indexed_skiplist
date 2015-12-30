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

typedef struct snode {
    unsigned char * data;
    struct snode ** forward;
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
    list->level = 1;
    list->size = 0;
}

int choose_level(){
    return rand_level();
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

void skiplist_dump(){
    
    int g;
    snode * x;
    
    x = list->header;
    while (x && x->forward[1] != list->header) {
        printf("[");
        for(g=0; g<KEY_LENGTH; g++){
            printf("%x ", x->forward[1]->data[g]);
        }
        printf(" - ");
        for(g=KEY_LENGTH; g<(KEY_LENGTH+VALUE_LENGTH); g++){
            printf("%x ", x->forward[1]->data[g]);
        }
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
        printf("%d:", g);
        while( x && x->forward[g] != list->header ){
            printf("[");
            for(h=0; h<KEY_LENGTH; h++){
                printf("%x ", x->forward[g]->data[h]);
            }
            printf(" - ");
            for(h=KEY_LENGTH; h<(KEY_LENGTH+VALUE_LENGTH); h++){
                printf("%x ", x->forward[g]->data[h]);
            }
            printf("]->");
            x = x->forward[g];
        }
        printf("NIL\n");
        x = list->header;
    }
}

void view_data(unsigned char * k){
    
    int g;
    
    for(g=0; g<DATA_LENGTH; g++){
        printf("%x ", *(k+g));
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
    
    for(g=KEY_LENGTH-1; g>=0; g--){
        if( *(k1+g) < *(k2+g) ){
/*
            printf("position %d is less than: returning TRUE\n", g);
*/
/*
            printf("less than returning TRUE\n");
*/
            return TRUE;
        } else {
            if( *(k1+g) > *(k2+g) ){
/*
                printf("position %d is greater than: returning FALSE\n", g);
*/
/*
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

void * skiplist_read(unsigned char * key){
    
    snode *x;
    int i;
    
    #ifdef TRACE
        printf("skiplist read, level = %d\n", list->level);
        printf("1...\n");
    #endif

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
}




void * skiplist_write(unsigned char * key){
    
    int i, g, level;
    snode * update[MAX_LEVELS+1];
    snode * x;
    
    #ifdef TRACE
        printf("skiplist write\n");
    #endif

    x = list->header;
    
    /* this points each slot of array to the key in the list for that given level */
    /* that is greater than or equal to the key being inserted */
    for (i = list->level; i >= 1; i--){
        while( less_than(x->forward[i]->data, key) ){
            x = x->forward[i];
        }
        update[i] = x;
    }
    
    /* why? Because we know this node isn't less than the key */
    /* it is either equal to the key of greater than, hence the following conditional */
    x = x->forward[1];
    
    /* if the item is already in the list, treat it like a failure */
    if ( equals(key, x->data) ) {
        
        return x->data + KEY_LENGTH;
/*
        return NON_POSITION;
*/
        
    } else {
        
        /* if the item isn't in the list, first choose a random level up to which */
        /* the new item will be pointed to in the list */
        level = choose_level();
        
        #ifdef TRACE
            printf("skiplist write, rand level = %d\n", level);
        #endif
        
        /* list->level seems to indicate the highest level of any node in the entire skiplist */
        if (level > list->level) {
            
            /* if the current randomly chosen level is greater than the list level, then */
            /* point the local array to the header for all levels? */
            for (i = list->level+1; i <= level; i++) {
                update[i] = list->header;
            }
            
            /* update the level of the entire list if it is greater than the previous one */
            /* no matter by how much */
            list->level = level;
            
        }
        
        /* allocate memory for a new node and set the value and key to this node */
        x = (snode *)malloc(sizeof(snode));
        if( x == NULL ){
            return NON_POSITION;
        }
        x->data = (unsigned char *)calloc(DATA_LENGTH, sizeof(unsigned char));
        for(g=0; g<KEY_LENGTH; g++){
            *(x->data+g) = *(key+g);
        }
        
        /* get memory for several pointers */
        
        int s = sizeof(snode*) * (level + 1);
/*
        printf("getting memory for x->forward: %d\n", s);
*/
        
        snode ** y = (snode**)malloc( s );
        x->forward = y;
/*
        y = malloc( s );
*/
        
        /* for all levels from 1 to the chosen level, point the successor of the new item */
        /* to the successor of the temp array for that level */
        /* and point the successor of the temp item to the new item */
        for (i = 1; i <= level; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
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

uint64_t index_of(unsigned char * key){
    
    return 880;
}