#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"

int main(){
    Stack_t stack = {CNR_PRT(0, DBG("stack", __FILE__, __LINE__))};
    //Stack_t stack = {DBG("stack", __FILE__, __LINE__)};

    StackCtor(&stack    DBG(, __FILE__, __LINE__));
    StackDump(&stack, __FILE__, __LINE__);
    StackPush(&stack, 1 DBG(, __FILE__, __LINE__));
    StackDump(&stack, __FILE__, __LINE__);
    long long x = 0;
    StackPop(&stack, &x DBG(, __FILE__, __LINE__));
    //printf("x:%d\n", x);
    StackDump(&stack, __FILE__, __LINE__);
    StackDtor(&stack    DBG(, __FILE__, __LINE__));
    StackDump(&stack, __FILE__, __LINE__);
}
