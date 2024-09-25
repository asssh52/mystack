#pragma once

typedef int StackElem_t;

typedef struct Stack_t{
    StackElem_t* data;
    size_t size;
    size_t capacity;
} Stack_t;

int StackCtor   (Stack_t* stk);
int StackDtor   (Stack_t* stk);
int StackPush   (Stack_t* stk, StackElem_t item);
int StackPop    (Stack_t* stk, StackElem_t* item);
int StackDump   (Stack_t* stk);
