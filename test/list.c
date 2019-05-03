#include "list.h"

#include <stdlib.h>
#include <stdio.h>

/****************************
 *** Elementary functions ***
 ****************************/
simple_flist* get_last(simple_flist* head)
{
    simple_flist* result;

    if(head == NULL)
        return NULL;

    for(result = head; result->tail != NULL; result = result->tail)
        ;

    return result;
}

int get_length(const simple_flist* head)
{
    const simple_flist* iter;
    int length = 0;

    for(iter = head; iter != NULL; iter = iter->tail)
        ++length;

    return length;
}

int get_value(const simple_flist* head, int position)
{
    const simple_flist* iter = head;
    int i;

    for(i = 0; i < position; ++i)
        iter = iter->tail;

    return iter->value;
}

/*******************************
 *** Elementary modification ***
 *******************************/
simple_flist* push_front(simple_flist* oldhead, int newvalue)
{
    simple_flist* newhead = (simple_flist*) malloc(sizeof(simple_flist));

    newhead->value = newvalue;
    newhead->tail = oldhead;

    return newhead;
}

simple_flist* pop_front(simple_flist* oldhead)
{
    simple_flist* newhead;

    if(oldhead == NULL)
        return NULL;

    newhead = oldhead->tail;
    free(oldhead);

    return newhead;
}

simple_flist* push_back(simple_flist* head, int newvalue)
{
    simple_flist* iter;

    if(head == NULL)
        return push_front(head, newvalue);

    iter = get_last(head);
    iter->tail = (simple_flist*) malloc(sizeof(simple_flist));
    iter->tail->value = newvalue;
    iter->tail->tail = NULL;

    return head;
}

simple_flist* pop_back(simple_flist* head)
{
    simple_flist* iter = head;

    if(head == NULL)
        return NULL;

    if(head->tail == NULL)
        return pop_front(head);

    while(iter->tail->tail != NULL)
        iter = iter->tail;

    free(iter->tail);
    iter->tail = NULL;

    return head;
}

simple_flist* copy(simple_flist* src)
{
    simple_flist newroot = {0, NULL};
    simple_flist *itr1 = src, *itr2 = &newroot;

    while(itr1 != NULL) {
        itr2->tail = (simple_flist*) malloc(sizeof(simple_flist));
        itr2->tail->value = itr1->value;
        itr2->tail->tail = NULL;

        itr1 = itr1->tail;
        itr2 = itr2->tail;
    }

    return newroot.tail;
}

void destroy(simple_flist* head)
{
    while(head != NULL)
        head = pop_front(head);
}

/*****************
 *** Filtering ***
 *****************/
simple_flist* filter(simple_flist* head, condition_t p)
{
    simple_flist* newhead = head;
    simple_flist* iter;

    while(newhead != NULL && !p(newhead->value))
        newhead = pop_front(newhead);

    if(newhead == NULL)
        return NULL;

    iter = newhead;

    while(iter->tail != NULL) {
        if(p(iter->tail->value))
            iter = iter->tail;
        else
            iter->tail = pop_front(iter->tail);
    }

    return newhead;
}

/*************************************
 *** Build-in comparison functions ***
 *************************************/
Ordering order_canonical(int x, int y)
{
    if(x==y)
        return EQ;
    if(x<y)
        return LT;
    if(x>y)
        return GT;
}

Ordering order_opposite(int x, int y)
{
    return order_canonical(y,x);
}

/*********************
 *** Build-in sort ***
 *********************/
simple_flist* bubble_sort(simple_flist* oldhead, ordering_t f)
{
    simple_flist dummy_head;
    simple_flist *iter, *tmp;
    int i, j, length = get_length(oldhead);

    if(oldhead == NULL || oldhead->tail == NULL)
        return oldhead;

    dummy_head.tail = oldhead;

    for(i = 0; i < length - 1; ++i) {
        iter = &dummy_head;

        for(j=0; j < length - i - 1; ++j) {
            switch(f(iter->tail->value, iter->tail->tail->value)) {
            case GT:
                tmp = iter->tail->tail;
                iter->tail->tail = tmp->tail;
                tmp->tail = iter->tail;
                iter->tail = tmp;
                break;
            }

            iter = iter->tail;
        }
    }

    return dummy_head.tail;
}

/************************
 *** Compute meridian ***
 ************************/
int get_median(simple_flist* head, ordering_t f, sort_algorithm_t sortalg)
{
    simple_flist* sorted;
    int length = get_length(head);
    int result;

    sorted = sortalg(copy(head), f);

    if(sorted == NULL) {
        result = 0;
    }
    else if(length % 2 == 0) {
        int m1 = get_value(sorted, (length/2)-1);
        int m2 = get_value(sorted, (length/2));

        result = (m1+m2)/2;
    }
    // The length is odd.
    else {
        result = get_value(sorted, length/2);
    }

    destroy(sorted);

    return result;
}
