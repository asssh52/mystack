#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"

int main(){
    Stack_t stack = {CNR_PRT(0 DBG(,)) DBG("stack", __FILE__, __LINE__)};

    DBGStackCtor(&stack);

    StackDump(&stack, __FILE__, __LINE__);

    char x = 0;

    for (int i = 0; i < 100; i++){
        DBGStackPush(&stack, '1');
    }
    StackDump(&stack, __FILE__, __LINE__);
    for (int i = 0; i < 100; i++){
        DBGStackPop(&stack, &x);
    }



    DBGStackPop(&stack, &x);
    DBGStackPop(&stack, &x);
    StackDump(&stack, __FILE__, __LINE__);
    DBGStackDtor(&stack);

}
