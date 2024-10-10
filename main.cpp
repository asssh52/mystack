#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"
#include "colors.hpp"

int main(){
    Stack_t stack = {};

    StackElem_t x = 0;

    StackCtor(&stack);
    StackPush(&stack, 1);
    StackDump(&stack);
    StackDtor(&stack);

}
