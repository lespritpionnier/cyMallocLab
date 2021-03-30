#include <stdio.h>
#include <stdlib.h>
#include "aboutDataStructure.h"

int initMemory (int nBytes);
int freeMemory();
void* myalloc(int nBytes);
int myfree(void* p);
void* myrealloc(int nBytes);

//Tests show that void* is a kind of char*, so we move the position by unit 'byte'
void *myMemoryPool;
ListBlock freeLists[FREE_LIST_NUMBER];
ListBlock userList;

int main() {
    initMemory(10);

    freeMemory();
    return 0;
}


int initMemory(int nBytes){
    int local = INIT_MEMORY_SIZE(nBytes);
    if((myMemoryPool=(int*)malloc(INIT_MEMORY_SIZE(nBytes)))==NULL){
        fprintf(stderr,"Erreur dans la allocation de la memoire initiale");

        Header initHeader = myMemoryPool ;
        Footer initFooter = myMemoryPool + local - UNIT_SIZE;
        myMemoryPool += UNIT_SIZE;
        *initHeader = 0;
        *initFooter = 0;
        myMemoryPool = addHeader(myMemoryPool,(local-2*UNIT_SIZE),STATE_FREE);

        for (int i=0;i<FREE_LIST_NUMBER;i++){
            freeLists[i]=initListBlock();
        }
        insertFreeList(freeLists,myMemoryPool);

        userList = initListBlock();

        return 1;
    }
    else{
        printf("Memoire allueée avec succées %d \n",local);
    }

    short int t = 16;
    printf("Memoire allueée avec succées %d \n",t==local);

    return 0;
}


int freeMemory(){
    free(myMemoryPool);
}


void* myalloc(int nBytes);
int myfree(void* p);
void* myrealloc(int nBytes);












