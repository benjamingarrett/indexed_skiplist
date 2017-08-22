/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   indexed_skiplist_long_int.h
 * Author: benjamin
 *
 * Created on March 2, 2017, 1:17 AM
 */

#ifndef INDEXED_SKIPLIST_LONG_INT_H
#define INDEXED_SKIPLIST_LONG_INT_H

#ifdef __cplusplus
extern "C" {
#endif

    #include<stdint.h>
    #include<stdlib.h>
    #include<limits.h>
    #include "indexed_skiplist.h"
    void initialize_skiplist_long_int();
    int64_t * skiplist_read_long_int(int64_t * key);
    int64_t * skiplist_write_long_int(int64_t * key, int64_t * value);
    int64_t * skiplist_delete_long_int(int64_t * key);
    int64_t index_of_long_int(int64_t * key);
    int64_t size_of_long_int();
    void skiplist_premium_dump_long_int();
    void skiplist_full_dump_long_int();
    void skiplist_dump_long_int();
    

#ifdef __cplusplus
}
#endif

#endif /* INDEXED_SKIPLIST_LONG_INT_H */

