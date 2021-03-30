//
// Created by L'esprit pionnier on 30/03/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include "aboutDataStructure.h"


void *addHeader(void *pd, int size, int state){
    void *after = pd + UNIT_SIZE;
    *(int*) pd = (size-2*UNIT_SIZE)*state;
    return after;
}

void addFooter(void *pd, int size, int state){
    *(int*) (pd + (size-1*UNIT_SIZE)*state) = size*state - 2*UNIT_SIZE;
}


int *GET_FOOTER_ADR(void *pd){
    return (int *) (pd + GET_SIZE_REAL(abs(GET_HEADER_VALUE(pd))));
}

int GET_NEXT_BLOCK_INFO(void *pd){
    return *(int *) (GET_FOOTER_ADR(pd) + UNIT_SIZE);
}


char *getBlockDataPointer(Block block){
    return block->data;
}
int getBlockSize (Block block){
    return abs( GET_HEADER_VALUE(getBlockDataPointer(block)) );
}


//Check if the previous block is occupied
int checkPrevBlock(Block block){
    return GET_PREV_BLOCK_INFO(getBlockDataPointer(block)) <= 0;
}

//Check if the next block is occupied
int checkNextBlock(Block block){
    return GET_NEXT_BLOCK_INFO(getBlockDataPointer(block)) <= 0;
}

Block initDataBlock(void *pd){
    Block new = (Block) malloc(sizeof (Block));
    new->data = GET_HEADER_ADR(pd);
    new->data = pd;
    new->footer = GET_FOOTER_ADR(pd);
}


ListBlock initListBlock(void *pd){
    ListBlock new = (ListBlock) malloc(sizeof(struct listBlock));
    new->size = GET_HEADER_VALUE(pd);
    new->block = initDataBlock(pd);
    new->next = null;
    new->last = null;
    return new;
}

void insertOrderList(ListBlock head, void *pd){
    if (head==null){
        head=initListBlock(pd);
    } else if ((head->next==null)||(head->size<GET_HEADER_VALUE(pd))){
        if (head->size<GET_HEADER_VALUE(pd)){
            ListBlock new = initListBlock(pd);
            new->last = head;
            head->next = new;
        } else{
            ListBlock change =  initListBlock(head->block->data);
            head->block = initDataBlock(pd);
            head->size = GET_HEADER_VALUE(pd);
            head->next = change;
        }
    }else {
        ListBlock current = head;
        int size = GET_HEADER_VALUE(pd);
        while (current->size<size && current->next!=null) {
            current=current->next;
        }
        ListBlock new = initListBlock(pd);
        current->last->next = new;
        new->last = current->last;
        current->last = new;
        new->next = current;
    }
}

void insertFreeList(ListBlock freeLists[], void *pd){
    int size = GET_HEADER_VALUE(pd);
    if(size>0){
        int position=-5;
        while (size!=0){
            size=size/2;
        }
        if (position<0){
            insertOrderList(freeLists[0],pd);
        } else if (position>=FREE_LIST_NUMBER){
            insertOrderList(freeLists[FREE_LIST_NUMBER-1],pd);
        }else{
            insertOrderList(freeLists[position],pd);
        }
    } else{
        fprintf(stderr,"Erreur, inser une liste occupé");
    }
}

void insertUserList(ListBlock head, ListBlock list){

}










