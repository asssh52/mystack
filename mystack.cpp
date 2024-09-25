#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"
#include "colors.hpp"

int StackCtor(Stack_t* stk){
    if (!stk->capacity) stk->capacity = 256;
    stk->data = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));


    printf(BLU "stack created\n" RESET);
    return 0;
}

int StackDtor(Stack_t* stk){
    //stack_verify
    free(stk->data);

    stk->data = nullptr;

    printf(CYN "stack destroyed\n" RESET);
    return 0;
}

int StackPush(Stack_t* stk, StackElem_t item){
    *(stk->data + stk->size) = item;
    stk->size += 1;

    printf(GRN "item (%d) pushed\n", item, RESET);
    return 0;
}

int StackPop(Stack_t* stk, StackElem_t* item){
    *item = *(stk->data + stk->size);
    stk->size -= 1;

    printf(GRN "item (%d) popped\n", *item, RESET);
    return 0;
}

int StackDump(Stack_t* stk){
    if (!stk || !stk->data){
        printf(RED "stack empty\n" RESET);
        return 1;
    }

    printf(YEL "\n\ndump start\n" RESET);

    printf("size:%d\n", stk->size);

    printf("capacity:%d\n", stk->capacity);

    printf("elems:\n");
    for (int i = 0; i < stk->size; i++){
        printf("num<%d>:%d ", i, *(stk->data + i * sizeof(StackElem_t)));
    }
    printf("\n");

    printf(YEL "stack dumped\n\n\n" RESET);
    return 0;
}

int StackVerify(Stack_t* stk){
    if (!stk) printf(RED "stk == null" RESET);
    if (!stk->data) printf(RED "stk == null" RESET);

    return 0;
}

void StackAssertFunc(Stack_t* stk, const char* filename, int line){}
