#include <stdio.h>
#include <stdlib.h>
#include "mystack.hpp"
#include "colors.hpp"

size_t djb2hashFunc(void* input, size_t size){
    size_t hash = 0xeda;
    for (int i = 0; i < size; i++){
        hash = (hash * 31) ^ *((char*)input + i);
    }
    return hash;
}

void putHash(Stack_t* stk){
    stk->bufferHash = djb2hashFunc(stk->data CNR_PRT(- 1), stk->capacity CNR_PRT(+ 2));
    stk->stackHash = 0;
    stk->stackHash = djb2hashFunc(stk, 5 * 8 CNR_PRT(+ 4 * 8) DBG(+ 3 * 8));
}

stackExits StackCtor(Stack_t* stk DBG(, const char* fileName, int line)){
    if (!stk) return STK_NULL;

    if (!stk->capacity) stk->capacity = 16;
    stk->data = (StackElem_t*)calloc(stk->capacity CNR_PRT(+ 2), sizeof(StackElem_t)) CNR_PRT(+ 1);
    if (!stk->data) return MEM_FULL;

    CNR_PRT(
    *(stk->data - 1)             = 0xbadeda;
    *(stk->data + stk->capacity) = 0x900deda;

    stk->chicken_first  = 0xBADC0DE ;
    stk->chicken_second = 0x900DC0DE;
    )

    HASH_PRT(putHash(stk);)

    printf(BLU DBG("stack created\n") RESET);

    STK_CHECK(stk, fileName, line)

    return OK;
}

stackExits StackDtor(Stack_t* stk DBG(, const char* fileName, int line)){
    STK_CHECK(stk, fileName, line)

    free(stk->data CNR_PRT(- 1));

    stk->data = nullptr;

    printf(CYN "stack destroyed\n" RESET);

    //STK_CHECK(stk)?
    return OK;
}

stackExits StackPush(Stack_t* stk, StackElem_t item DBG(, const char* fileName, int line)){
    STK_CHECK(stk, fileName, line)
    *(stk->data + stk->size) = item;
    stk->size += 1;

    printf(GRN "item (%d) pushed\n" RESET, item);

    HASH_PRT(putHash(stk);)
    STK_CHECK(stk, fileName, line)
    return OK;
}

stackExits StackPop(Stack_t* stk, StackElem_t* item DBG(, const char* fileName, int line)){
    STK_CHECK(stk, fileName, line)
    *item = *(stk->data + stk->size - 1);
            *(stk->data + stk->size - 1) = 0;
    stk->size -= 1;

    printf(GRN "item (%d) popped\n" RESET, *item);

    HASH_PRT(putHash(stk);)
    STK_CHECK(stk, fileName, line)
    return OK;
}

stackExits StackDump(Stack_t* stk, const char* filename, int line){
    if (!stk){
        printf(RED "stack does not exist\n" RESET);
        return ERR;
    }
    printf(YEL "Stack_t[%p] born at %s:%d, name \"%s\"\n", stk, stk->filename, stk->line, stk->name);
    printf("dumb dump called from: %s:%d\n", filename, line);
    printf("{\n");

    CNR_PRT(
    printf("first chick:%p", stk->chicken_first);
    if (stk->chicken_first == 0xBADC0DE) printf(GRN " <OK>\n" YEL);
        else printf(RED " <NOT OK>\n" YEL);
    )

    printf("size:%lu\n",     stk->size);
    printf("capacity:%lu\n", stk->capacity);

    if (stk->data){
        CNR_PRT(
        printf("first hen:%p", *(stk->data - 1));
        if (*(stk->data - 1) == 0xbadeda) printf(GRN " <OK>\n" YEL);
            else printf(RED " <NOT OK>\n" YEL);
        )

        printf("data:%p\n",      stk->data);
        for (int i = 0; i < stk->capacity; i++){
            printf("<%lu>:%lu\n", i, *((char*)(stk->data) + i * sizeof(StackElem_t)));
        }

        CNR_PRT(
        printf("second hen:%p", *(stk->data + stk->capacity));
        if (*(stk->data + stk->capacity) == 0x900deda) printf(GRN " <OK>\n" YEL);
            else printf(RED " <NOT OK>\n" YEL);
        )
    }
    else{
        printf("data is empty\n");
    }

    CNR_PRT(
    printf("second chick:%p", stk->chicken_second);
    if (stk->chicken_second == 0x900DC0DE) printf(GRN " <OK>\n" YEL);
        else printf(RED " <NOT OK>\n" YEL);
    )

    HASH_PRT(
    printf("buffer hash:%p\n", stk->bufferHash);
    printf("stack hash: %p\n", stk->stackHash );
    )


    printf("}\n" RESET);
    return OK;
}

stackExits StackVerify(Stack_t* stk){
    if (!stk){
        return ERR;
    }
    if (!(stk->data)){
        return DATA_EMPTY;
    }
    if (stk->size > stk->capacity){
        return SIZE_OVERFLOW;
    }
    CNR_PRT(
    if (stk->chicken_first != 0xbadc0de || stk->chicken_second != 0x900dc0de){
        return CNR_STK_ERR;
    }
    if (*(stk->data - 1) != 0xbadeda || *(stk->data + stk->capacity) != 0x900deda){
        return CNR_BUF_ERR;
    }
    )

    return OK;
}


