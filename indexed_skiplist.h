/* 
 * File:   indexed_skiplist.h
 * Author: benjamin
 *
 * Created on November 9, 2015, 12:04 AM
 */

#ifndef INDEXED_SKIPLIST_H
#define	INDEXED_SKIPLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include<stdint.h>
    #include<stdio.h>
    #include<stdlib.h>
    #include<limits.h>
    void initialize_skiplist(
            int max_levels,
       /*     void * non_value, 
            void * non_position,  */
            size_t key_length, 
            size_t value_length);
    
    /* exposed for testing */
    int rand_level();
    uint8_t less_than(unsigned char * k1, unsigned char * k2);
    void skiplist_dump();
    void skiplist_full_dump();
    void skiplist_premium_dump();
    void view_key(unsigned char * key);
    
    void * skiplist_read(unsigned char * key);
    void * skiplist_write(unsigned char * key);
    void * skiplist_delete(unsigned char * key);
    int64_t index_of(unsigned char * key);
    int64_t size_of();

#ifdef	__cplusplus
}
#endif

#endif	/* INDEXED_SKIPLIST_H */

