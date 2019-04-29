#ifndef TSUKIMADE_TEST_LIST_H
#define TSUKIMADE_TEST_LIST_H

// Simple forward list.
struct simple_flist_ {
    int value;
    struct simple_flist_* tail;
};

typedef struct simple_flist_ simple_flist;


// Elementary functions on lists.
simple_flist* get_last(simple_flist* head);
int get_length(const simple_flist* head);
int get_value(const simple_flist* head, int position);

// Elementary modification on lists.
simple_flist* push_front(simple_flist* oldhead, int newvalue);
simple_flist* pop_front(simple_flist* oldhead);
simple_flist* push_back(simple_flist* head, int newvalue);
simple_flist* pop_back(simple_flist* head);

simple_flist* copy(simple_flist* src);
void destroy(simple_flist* head);


// Filtering.
typedef int (*condition_t)(int);

simple_flist* filter(simple_flist* head, condition_t p);


// Sorting.
typedef enum {
    EQ,
    LT,
    GT
} Comparison;

typedef Comparison (*compare_func_t)(int, int);

Comparison compare_canonical(int x, int y);
Comparison compare_opposite(int x, int y);

typedef simple_flist* (*sort_algorithm_t)(simple_flist*, compare_func_t);

simple_flist* bubble_sort(simple_flist* oldhead, compare_func_t f);


// Compute meridian
int get_median(simple_flist* head, compare_func_t f, sort_algorithm_t sortalg);

#endif
