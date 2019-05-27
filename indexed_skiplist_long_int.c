#include<stdint.h>
#include<stdlib.h>
#include<limits.h>
#include "indexed_skiplist.h"
#include "indexed_skiplist_long_int.h"

int MAX__LEVELS = 10;
size_t KEY__LENGTH = 8;
size_t VALUE__LENGTH = 8;

void skiplist_premium_dump_long_int(){
  skiplist_premium_dump();
}

void skiplist_full_dump_long_int(){
  skiplist_full_dump();
}

void initialize_skiplist_long_int(){
  initialize_skiplist(MAX__LEVELS, KEY__LENGTH, VALUE__LENGTH);
}

void skiplist_dump_long_int(){
  skiplist_dump();
}

int64_t * skiplist_read_long_int(int64_t * key){
  uint64_t g;
  unsigned char * k, byte_key[KEY__LENGTH], byte_value[VALUE__LENGTH];
  int64_t * vp;
  k = (unsigned char *) &(*key);
  for(g = 0; g < KEY__LENGTH; g++){
    byte_key[g] = *(k + g);
  }
  vp = (int64_t *) skiplist_read(byte_key);
  return vp;
}

int64_t * skiplist_write_long_int(int64_t * key, int64_t * value){
  uint64_t g;
  unsigned char * k, byte_key[KEY__LENGTH], byte_value[VALUE__LENGTH];
  int64_t * vp;
  k = (unsigned char *) &(*key);
  for(g = 0; g < KEY__LENGTH; g++){
    byte_key[g] = *(k + g);
  }
  vp = (int64_t *) skiplist_write(byte_key);
  if(vp != NULL){
    *vp = *value;
  }
  return vp;
}

int64_t * skiplist_delete_long_int(int64_t * key){
  uint64_t g;
  unsigned char * k, byte_key[KEY__LENGTH];
  int64_t * vp;
  k = (unsigned char *) &(*key);
  for(g = 0; g < KEY__LENGTH; g++){
    byte_key[g] = *(k + g);
  }
  vp = (int64_t *) skiplist_delete(byte_key);
  return vp;
}

int64_t index_of_long_int(int64_t * key){
  int64_t g;
  unsigned char * k, byte_key[KEY__LENGTH];
  k = (unsigned char *) &(*key);
  for(g = 0; g < KEY__LENGTH; g++){
    byte_key[g] = *(k + g);
  }
  return index_of(byte_key);
}

int64_t size_of_long_int(){
  return size_of();
}
