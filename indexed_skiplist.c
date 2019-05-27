#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "indexed_skiplist.h"

#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define FAILURE 1

uint64_t MAX_LEVELS = 25;
uint64_t MIN_KEY_LENGTH = 4;
uint64_t MIN_VALUE_LENGTH = 4;
uint64_t INDEX_LENGTH = 8;
uint64_t DATA_LENGTH;
void * NON_VALUE;
void * NON_POSITION;
uint64_t KEY_LENGTH;
uint64_t VALUE_LENGTH;

int UPDATE_WIDTHS_ON_WRITE = TRUE;
int UPDATE_WIDTHS_ON_DELETE = TRUE;

typedef struct snode {
  unsigned char * data;
  struct snode ** forward;
  int * width;
} snode;

typedef struct skiplist {
  int level;
  int size;
  struct snode * header;
} skiplist;

skiplist * list;

void initialize_test_levels();

void initialize_skiplist(int max_levels, size_t key_length, size_t value_length){
  int i;
  #ifdef TRACE
  printf("initialize skiplist\n");
  #endif
  MAX_LEVELS = max_levels;
  if (key_length < MIN_KEY_LENGTH) {
    KEY_LENGTH = MIN_KEY_LENGTH;
  } else {
    KEY_LENGTH = key_length;
  }
  if (value_length < MIN_VALUE_LENGTH) {
    VALUE_LENGTH = MIN_VALUE_LENGTH;
  } else {
    VALUE_LENGTH = value_length;
  }
  DATA_LENGTH = KEY_LENGTH + VALUE_LENGTH + INDEX_LENGTH;
  list = (skiplist *) malloc(sizeof (struct skiplist));
  list->header = (snode *) malloc(sizeof (struct snode));
  list->header->data = (unsigned char *) calloc(DATA_LENGTH, sizeof (unsigned char));
  for (i = 0; i < KEY_LENGTH; i++) {
    list->header->data[i] = 0xff;
  }
  list->header->forward = (snode **) malloc(sizeof (snode*) * (MAX_LEVELS + 1));
  list->header->width = (int *) calloc(MAX_LEVELS + 1, sizeof (int));
  for (i = 0; i <= MAX_LEVELS; i++) {
    list->header->forward[i] = list->header;
    list->header->width[i] = 1;
  }
  list->level = 1;
  list->size = 0;

  initialize_test_levels();
}

int NUM_TEST_LEVELS, p;
int * test_level;

void initialize_test_levels() {
  NUM_TEST_LEVELS = 20;
  test_level = calloc(NUM_TEST_LEVELS, sizeof (int));
  test_level[0] = 1;
  test_level[1] = 2;
  test_level[2] = 1;
  test_level[3] = 3;
  test_level[4] = 2;
  test_level[5] = 2;
  test_level[6] = 1;
  test_level[7] = 2;
  test_level[8] = 3;
  test_level[9] = 1;
  test_level[10] = 1;
  test_level[11] = 4;
  test_level[12] = 2;
  test_level[13] = 2;
  test_level[14] = 2;
  test_level[15] = 1;
  test_level[16] = 2;
  test_level[17] = 4;
  test_level[18] = 1;
  test_level[19] = 3;
}

int choose_level() {
  int level;
  level = rand_level();
  /*
  level = test_level[p++%NUM_TEST_LEVELS];
  level = 1;
   */
  #ifdef TRACE
  printf("skiplist choose level = %d\n", level);
  #endif
  return level;
}

int rand_level() {
  int level = 1;
  while(rand() < RAND_MAX / 2 && level < MAX_LEVELS){
    level++;
  }
  return level;
}

void skiplist_node_free(snode *x){
  if(x){
    free(x->forward);
    free(x);
  }
}

void view_key(unsigned char * data){
  for(int g = 0; g < KEY_LENGTH; g++){
    printf("%x ", data[g]);
  }
}

void view_data(unsigned char * k){
  for(int g = KEY_LENGTH; g < (KEY_LENGTH + VALUE_LENGTH); g++){
    printf("%x ", *(k + g));
  }
}

void view_snode(snode * node, int g){
  printf("{[%ld](%ld)[%ld]=", (long int) node, (long int) node->width[g], (long int) node->forward[g]);
  view_key(node->data);
  printf("}");
}

void skiplist_dump(){
  snode * x = list->header;
  while(x && x->forward[1] != list->header){
    printf("[");
    view_key(x->forward[1]->data);
    printf(" - ");
    view_data(x->forward[1]->data);
    printf("]->");
    x = x->forward[1];
  }
  printf("NIL\n");
}

void skiplist_premium_dump(){
  snode * x;
  for(int g = list->level; g >= 1; g--){
    x = list->header;
    printf("%d: ", g);
    view_snode(x, g);
    x = x->forward[g];
    printf("->");
    view_snode(x, g);
    while(x && x->forward[g] != list->header){
      x = x->forward[g];
      printf("->");
      view_snode(x, g);
    }
    x = x->forward[g];
    printf("->");
    view_snode(x, g);
    printf("\n");
  }
}

void skiplist_full_dump(){
  int g, h;
  snode * update[MAX_LEVELS + 1];
  snode * x;
  #ifdef TRACE
  printf("skiplist full dump, list->level = %d\n", list->level);
  #endif
  x = list->header;
  for(g = list->level; g >= 1; g--){
    printf("%d: HEAD [%d](%d)->", g, list->header, list->header->width[g]);
    while(x && x->forward[g] != list->header){
      printf("[");
      view_key(x->forward[g]->data);
      printf(" - ");
      view_data(x->forward[g]->data);
      printf(" - [%d](%d)", x, x->width[g]);
      printf("]->");
      x = x->forward[g];
    }
    printf("NIL\n");
    x = list->header;
  }
}

uint8_t equals(unsigned char * k1, unsigned char * k2) {
  //    printf("equals %x %x %x %x %x %x %x %x --- %x %x %x %x %x %x %x %x\n", 
  //            *(k1+0), *(k1+1), *(k1+2), *(k1+3), *(k1+4), *(k1+5), *(k1+6), *(k1+7), 
  //            *(k2+0), *(k2+1), *(k2+2), *(k2+3), *(k2+4), *(k2+5), *(k2+6), *(k2+7) );
  for(int g = 0; g < KEY_LENGTH; g++){
    if(*(k1 + g) != *(k2 + g)){
      return FALSE;
    }
  }
  return TRUE;
}

uint8_t less_than(unsigned char * k1, unsigned char * k2){
  for(int g = KEY_LENGTH - 1; g >= 0; g--){
    if(*(k1 + g) < *(k2 + g)){
      return TRUE;
    } else {
      if(*(k1 + g) > *(k2 + g)){
        return FALSE;
      }
    }
  }
  return FALSE;
}

int is_max_key(unsigned char * key){ 
  for(int g = 0; g < KEY_LENGTH; g++){
    if(*(key + g) != 0xff){
      return FALSE;
    }
  }
  return TRUE;
}

snode * find_prior_insertion_point(unsigned char * key){
  printf("find prior insertion point: ");
  view_key(key);
  printf("\n");
  printf("compare(header): ");
  view_key(list->header->forward[list->level]->data);
  printf(" and ");
  view_key(key);
  printf("\n");
  if(!less_than(list->header->forward[list->level]->data, key)){
    printf("found prior insertion point(header): ");
    view_snode(list->header, 1);
    printf("\n");
    return list->header;
  }
  snode * x = list->header->forward[list->level];
  printf("compare(initially): ");
  view_key(x->forward[list->level]->data);
  printf(" and ");
  view_key(key);
  printf("\n");
  while(less_than(x->forward[list->level]->data, key)){
    x = x->forward[list->level];
    printf("compare: ");
    view_key(x->forward[list->level]->data);
    printf(" and ");
    view_key(key);
    printf("\n");
  }
  printf("found prior insertion point: ");
  view_snode(x, 1);
  printf("\n");
  return x;
}

snode * find_node(unsigned char * key){
  snode * x = list->header;
  for(int i = list->level; i >= 1; i--){
    while(less_than(x->forward[i]->data, key)){
      x = x->forward[i];
    }
  }
  if(equals(x->forward[1]->data, key)){
    return x->forward[1];
  } else {
    return NULL;
  }
  return NULL;
}

void * skiplist_read(unsigned char * key){
  #ifdef TRACE
  printf("skiplist read, level = %d\n", list->level);
  #endif
  if(is_max_key(key)){
    return NULL;
  }
  snode * x = find_node(key);
  if(x != NULL){
    #ifdef TRACE
    printf("skiplist read found value\n");
    #endif
    return x->data + KEY_LENGTH;
  } else {
    #ifdef TRACE
    printf("skiplist read didn't find value\n");
    #endif
    return NULL/*NON_VALUE*/;
  }
}

void * skiplist_write(unsigned char * key){
  int i, g, level, temp_width;
  snode * update[MAX_LEVELS + 1];
  snode * w, *x, * y, * z;
  #ifdef TRACE
  printf("skiplist write\n\n");
  #endif
  if(is_max_key(key)){
    return NULL;
  }
  if(find_node(key) != NULL){
    return NULL;
  }
  w = list->header;
  for(i = list->level; i >= 1; i--){
    while(less_than(w->forward[i]->data, key)){
      w = w->forward[i];
    }
    update[i] = w;
  }
  w = w->forward[1];
  if(equals(key, w->data)){
    return w->data + KEY_LENGTH;
  } else {
    level = choose_level();
    if(level > list->level){
      for(i = list->level + 1; i <= level; i++){
        update[i] = list->header;
      }
      list->level = level;
    }
    w = (snode *) malloc(sizeof (snode));
    if(w == NULL){
      return NULL;
    }
    w->data = (unsigned char *) calloc(DATA_LENGTH, sizeof (unsigned char));
    for(g = 0; g < KEY_LENGTH; g++){
      *(w->data + g) = *(key + g);
    }
    w->forward = (snode**) malloc(sizeof (snode*) * (level + 1));
    w->width = (int *) calloc(level + 1, sizeof (int));
    for(i = 1; i <= level; i++){
      w->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = w;
    }
    if(UPDATE_WIDTHS_ON_WRITE){
      w = find_node(key);
      w->width[1] = 1;
      for(i = 2; i <= level; i++){
        y = w;
        w->width[i] = y->width[i - 1];
        while(less_than(y->forward[i - 1]->data, w->forward[i]->data)){
          y = y->forward[i - 1];
          w->width[i] += y->width[i - 1];
        }
      }
      update[1]->width[1] = 1;
      for(i = 2; i <= level; i++){
        x = find_node(update[i]->data);
        y = x;
        temp_width = y->width[i - 1];
        while(less_than(y->forward[i - 1]->data, x->forward[i]->data)){
          y = y->forward[i - 1];
          temp_width += y->width[i - 1];
        }
        x->width[i] = temp_width;
      }
      if(level < list->level){
        x = list->header;
        for(i = list->level; i > level; i--){
          while(less_than(x->forward[i]->data, key)){
            x = x->forward[i];
          }
          x->width[i]++;
        }
      }
    }
    list->size++;
    return w->data + KEY_LENGTH;
  }
}

void * skiplist_delete(unsigned char * key){
  int i, tmp_cnt;
  snode * update[MAX_LEVELS + 1];
  snode * x, * y;
  #ifdef TRACE
  printf("skiplist_delete\n");
  skiplist_premium_dump();
  #endif
  if(is_max_key(key)){
    return NULL/*FAILURE*/;
  }
  x = list->header;
  for(i = list->level; i >= 1; i--){
    while(less_than(x->forward[i]->data, key)){
      x = x->forward[i];
    }
    update[i] = x;
  }
  x = x->forward[1];
  if(equals(x->data, key)){
    for(i = 1; i <= list->level; i++){
      if(update[i]->forward[i] != x){
        if(UPDATE_WIDTHS_ON_DELETE){
          update[i]->width[i]--;
        }
      } else {
        if (UPDATE_WIDTHS_ON_DELETE){
          update[i]->width[i] = update[i]->width[i] + x->width[i] - 1;
        }
        update[i]->forward[i] = x->forward[i];
      }
    }
    skiplist_node_free(x);
    while (list->level > 1 && list->header->forward[list->level] == list->header){
      list->level--;
    }
    list->size--;
    list->size = (list->size < 0) ? 0 : list->size;
    return list->header->forward[1]->data + KEY_LENGTH;
  }
  return NULL/*FAILURE*/;
}

int64_t size_of(){
  return list->size;
}

int64_t index_of(unsigned char * key){
  snode *x;
  int i, index;
  if(is_max_key(key)){
    //        printf("index_of is max key\n");
    return -1;
  }
  x = list->header;
  index = 0;
  for(i = list->level; i >= 1; i--){
    while(less_than(x->forward[i]->data, key)){
      index += x->width[i];
      x = x->forward[i];
    }
  }
  if(equals(x->forward[1]->data, key)){
    //        printf("index_of found the key\n");
    return index;
  } else {
    //        printf("index_of did not find key\n");
    return -1;
  }
}
