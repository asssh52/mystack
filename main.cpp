#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"
#include "colors.hpp"

int main(){
    Stack_t stack = {};

    StackElem_t x = 0;

    StackCtor(&stack);

    for (uint64_t i = 0; i < 300; i++){
        StackPush(&stack, i);
    }

    for (uint64_t i = 0; i < 302; i++){
        StackPop(&stack, &x);
    }

    StackDtor(&stack);
    StackDtor(&stack);
    StackDtor(&stack);
    StackCtor(&stack);
    StackCtor(&stack);
    StackCtor(&stack);
}
