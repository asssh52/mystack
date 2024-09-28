#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"

int main(){
    Stack_t stack = {CNR_PRT(0 DBG(,)) DBG("stack", __FILE__, __LINE__)};

    DBGStackCtor(&stack);

    DBGStackPush(&stack, 1);
    DBGStackPush(&stack, 1);
    StackDump(&stack, __FILE__, __LINE__);

    long long x = 0;

    for (int i = 0; i < 20; i++){
        DBGStackPush(&stack, 1);
    }
    for (int i = 0; i < 20; i++){
        DBGStackPop(&stack, &x);
    }




    StackDump(&stack, __FILE__, __LINE__);
    DBGStackPop(&stack, &x);
    StackDump(&stack, __FILE__, __LINE__);
    DBGStackDtor(&stack);

}
