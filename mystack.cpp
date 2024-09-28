#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "mystack.hpp"
#include "colors.hpp"

uint64_t djb2hashFunc(void* input, size_t size){
    uint64_t hash = 0xeda;
    for (int i = 0; i < size; i++){
        hash = (hash * 31) ^ *((char*)input + i);
    }
    return hash;
}

void PutHash(Stack_t* stk){
    stk->bufferHash = djb2hashFunc((char*)stk->data CNR_PRT(- 1 * sizeof(canary_t)), stk->capacity * sizeof(StackElem_t) CNR_PRT(+ 2 * sizeof(canary_t)));
    stk->stackHash = 0;
    stk->stackHash = djb2hashFunc(stk, 5 * sizeof(char*) CNR_PRT(+ 4 * sizeof(canary_t)) DBG(+ 3 * sizeof(char*)));
}

uint64_t FindBufferHash(Stack_t* stk){
    uint64_t newBufferHash = djb2hashFunc((char*)stk->data CNR_PRT(- 1 * sizeof(canary_t)), stk->capacity * sizeof(StackElem_t) CNR_PRT(+ 2 * sizeof(canary_t)));

    return newBufferHash;
}

uint64_t FindStackHash(Stack_t* stk){
    uint64_t oldStackHash = stk->stackHash;
                            stk->stackHash = 0;
    uint64_t newStackHash  = djb2hashFunc(stk, 5 * sizeof(char*) CNR_PRT(+ 4 * sizeof(canary_t)) DBG(+ 3 * sizeof(char*)));

    stk->stackHash = oldStackHash;

    return newStackHash;
}

stackExits StackRelocate(Stack_t* stk, int param){ //TODO: REMOVE COPYPASTE
    if (param == 1){
        *(stk->data + stk->capacity) = 0;
        uint64_t oldCapacity = stk->capacity;
        stk->capacity *= 2;

        printf(BLU "old data pointer:%p\n", stk->data);
        StackElem_t* newDataPointer = (StackElem_t*)realloc((char*)stk->data CNR_PRT(- 1 * sizeof(canary_t)), ((stk->capacity CNR_PRT(+ 2)) * sizeof(StackElem_t))) CNR_PRT(+ 1);
        memset((char*)newDataPointer + oldCapacity * sizeof(StackElem_t), 0, oldCapacity * sizeof(StackElem_t));
        printf(    "new data pointer:%p\n" RESET, newDataPointer);

        //StackDump(stk, __FILE__, __LINE__);
        if (!newDataPointer){
            return REALLOC_ERR;
        }

        stk->data = newDataPointer;
        *(stk->data + stk->capacity) = 0x900deda;
    }

    else if (param == -1){
        *(stk->data + stk->capacity) = 0;
        uint64_t oldCapacity = stk->capacity;
        stk->capacity /= 2;

        printf(BLU "old data pointer:%p\n", stk->data);
        StackElem_t* newDataPointer = (StackElem_t*)realloc((char*)stk->data CNR_PRT(- 1 * sizeof(canary_t)), ((stk->capacity CNR_PRT(+ 2)) * sizeof(StackElem_t))) CNR_PRT(+ 1);
        printf(    "new data pointer:%p\n" RESET, newDataPointer);

        if (!newDataPointer){
            return REALLOC_ERR;
        }

        stk->data = newDataPointer;
        *(stk->data + stk->capacity) = 0x900deda;

        StackDump(stk, __FILE__, __LINE__);
    }

    else{
        return ERR;
    }

    return OK;
}

stackExits StackCtor(Stack_t* stk DBG(, const char* fileName, int line)){
    if (!stk) return STK_NULL;

    if (!stk->capacity) stk->capacity = 4;
    stk->data = (StackElem_t*)calloc(stk->capacity CNR_PRT(+ 2), sizeof(StackElem_t)) CNR_PRT(+ 1);
    if (!stk->data) return MEM_FULL;

    CNR_PRT(
    *(stk->data - 1)             = 0xbadeda;
    *(stk->data + stk->capacity) = 0x900deda;

    stk->chicken_first  = 0xBADC0DE ;
    stk->chicken_second = 0x900DC0DE;
    )

    HASH_PRT(PutHash(stk);)

    printf(BLU "stack created\n" RESET);

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

    printf(GRN "item (%lld) pushed\n" RESET, item);

    if (stk->size >= stk->capacity){
        if (!StackRelocate(stk, 1)){
            printf(GRN "new memory allocated\n" RESET);
        }
        else{
            printf(RED "reallocation error\n"   RESET);
            return REALLOC_ERR;
        }
    }

    HASH_PRT(PutHash(stk);)
    STK_CHECK(stk, fileName, line)
    return OK;
}

stackExits StackPop(Stack_t* stk, StackElem_t* item DBG(, const char* fileName, int line)){
    STK_CHECK(stk, fileName, line)
    *item = *(stk->data + stk->size - 1);
            *(stk->data + stk->size - 1) = 0;
    stk->size -= 1;

    if (stk->capacity > 4 && stk->size <= stk->capacity / 4){
        if(!StackRelocate(stk, -1)){
            printf(GRN "new memory freed\n" RESET);
        }
        else{
            printf(RED "freeing error\n"   RESET);
            return REALLOC_ERR;
        }
    }

    printf(GRN "item (%lld) popped\n" RESET, *item);

    HASH_PRT(PutHash(stk);)
    STK_CHECK(stk, fileName, line)
    return OK;
}

stackExits StackDump(Stack_t* stk, const char* filename, int line){
    if (!stk){
        printf(RED "stack does not exist\n" RESET);
        return ERR;
    }
    printf(MAG "Stack_t[%p] born at " DBGPrintLine("%s:%lld, name \"%s\"") "\n" YEL, stk DBG(, stk->filename, stk->line, stk->name));
    printf("dumb dump called from: %s:%d\n", filename, line);
    printf("{\n");

    CNR_PRT(
    printf("first chick: \t%llx", stk->chicken_first);
    if (stk->chicken_first == 0xBADC0DE) printf(GRN " <OK>\n" YEL);
        else printf(RED " <NOT OK>\n" YEL);
    printf("expected:   \tbadc0de\n");
    )

    printf("size:%lu\n",     stk->size);
    printf("capacity:%lu\n", stk->capacity);

    if (stk->data){
        CNR_PRT(
        printf("first hen:\t%llx", *(stk->data - 1));
        if (*(stk->data - 1) == 0xbadeda) printf(GRN " <OK>\n" YEL);
            else printf(RED " <NOT OK>\n" YEL);
        printf("expected:   \tbadeda\n");
        )

        printf("data:%p\n",      stk->data);
        for (int i = 0; i < stk->capacity; i++){
            printf("<%d>:%lld\n", i, *(StackElem_t*)((char*)(stk->data) + i * sizeof(StackElem_t)));
        }

        CNR_PRT(
        printf("second hen: \t%llx", *(stk->data + stk->capacity));
        if (*(stk->data + stk->capacity) == 0x900deda) printf(GRN " <OK>\n" YEL);
            else printf(RED " <NOT OK>\n" YEL);
        printf("expected:   \t900deda\n");
        )
    }
    else{
        printf("data is empty\n");
    }

    CNR_PRT(

    printf("second chick:  \t%llx", stk->chicken_second);
    if (stk->chicken_second == 0x900DC0DE) printf(GRN " <OK>\n" YEL);
        else printf(RED " <NOT OK>\n" YEL);
    printf("expected:   \t900dc0de\n");
    )

    HASH_PRT(

    uint64_t newBufferHash = FindBufferHash(stk);
    printf("buffer hash:\t%llx", stk->bufferHash);
    if (newBufferHash != stk->bufferHash) printf(RED " <NOT OK>\n" YEL);
        else printf(GRN " <OK>\n" YEL);
    printf("expected:   \t%llx\n", newBufferHash);

    uint64_t newStackHash = FindStackHash(stk);
    printf("stack hash: \t%llx", stk->stackHash);
    if (newStackHash != stk->stackHash) printf(RED " <NOT OK>\n" YEL);
        else printf(GRN " <OK>\n" YEL);
    printf("expected:   \t%llx\n", newStackHash);

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

    HASH_PRT(
    char sameStackHash  = (FindStackHash(stk)  == stk->stackHash );
    char sameBufferHash = (FindBufferHash(stk) == stk->bufferHash);
    if (!sameStackHash){
        return HASH_STK_ERR;
    } else if (!sameBufferHash){
        return HASH_BUF_ERR;
    }
    )

    return OK;
}


