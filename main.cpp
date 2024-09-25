#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"

int main(){
    Stack_t stack= {};
    StackCtor(&stack);
    StackDump(&stack);
    StackPush(&stack, 1);
    StackDump(&stack);
    int x = 0;
    StackPop(&stack, &x);
    //printf("x:%d\n", x);

    StackDtor(&stack);
    StackDump(&stack);
}
