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
    *(int*) (pd + (size*state-1*UNIT_SIZE)) = size*state - 2*UNIT_SIZE;
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


ListBlock initListBlock(Block block){
    ListBlock new = (ListBlock) malloc(sizeof(struct listBlock));
    new->size = GET_HEADER_VALUE(block->data);
    new->block = block;
    new->next = NULL;
    new->last = NULL;
    return new;
}

void deleteList(ListBlock this){
    if (this->last!=NULL)this->last->next=this->next;
    if (this->next!=NULL)this->next->last=this->last;
    free(this);
}

void insertOrderFreeList(ListBlock head, void *pd){
    if (head==NULL){
        head=initListBlock(initDataBlock(pd));
    } else if ((head->next==NULL)||(head->size<GET_HEADER_VALUE(pd))){
        if (head->size<GET_HEADER_VALUE(pd)){
            ListBlock new = initListBlock(initDataBlock(pd));
            new->last = head;
            head->next = new;
        } else{
            ListBlock change =  initListBlock(head->block);
            head->block = initDataBlock(pd);
            head->size = GET_HEADER_VALUE(pd);
            head->next = change;
        }
    }else {
        ListBlock current = head;
        int size = GET_HEADER_VALUE(pd);
        while (current->size<size && current->next!=NULL) {
            current=current->next;
        }
        ListBlock new = initListBlock(initDataBlock(pd));
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
            insertOrderFreeList(freeLists[0], pd);
        } else if (position>=FREE_LIST_NUMBER){
            insertOrderFreeList(freeLists[FREE_LIST_NUMBER - 1], pd);
        }else{
            insertOrderFreeList(freeLists[position], pd);
        }
    } else{
        fprintf(stderr,"Erreur, inser une liste occupé");
    }
}

ListBlock firstFitFreeList(ListBlock freeList, int size){
    ListBlock current = freeList;
    while (current->size<size){
        current=current->next;
    }
    ListBlock new = initListBlock(current->block);
    new->size=size;
    deleteList(current);
    return new;
}

ListBlock findFreeList(ListBlock freeLists[], int nBytes){
    int size = nBytes;
    if(size>0){
        int position=-5;
        while (size!=0){
            size=size/2;
        }
        if (position<0){
            insertOrderFreeList(freeLists[0], pd);
        } else if (position>=FREE_LIST_NUMBER){
            insertOrderFreeList(freeLists[FREE_LIST_NUMBER - 1], pd);
        }else{
            insertOrderFreeList(freeLists[position], pd);
        }
    } else{
        fprintf(stderr,"Erreur, inser une liste occupé");
    }
}

void insertUserList(ListBlock head, ListBlock list){
    if (head==NULL){
        head=list;
    }
    ListBlock position = head;
    while (position->next!=NULL){
        position=position->next;
    }
    position->next=position;
    list->last=position;
}

void freeUserList(ListBlock userLists, void *p, ListBlock freeLists[]) {
    ListBlock position = userLists;
    while (position->block->data!=p&&position!=NULL){
        position=position->next;
    }
    if (position==NULL){
        fprintf(stderr,"Element n'exist pas!");
    } else{
        freeBusyList(freeLists,position);
    }
}










