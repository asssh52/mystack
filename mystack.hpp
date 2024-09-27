#pragma once

#define DBG(...)      __VA_ARGS__
#define CNR_PRT(...)  __VA_ARGS__
#define HASH_PRT(...) __VA_ARGS__

#ifdef DBG
    #define STK_CHECK(stk, file, line)\
    if (StackVerify(stk)){\
        StackDump(stk, file, line);\
        return ERR;\
    }
#else
    #define STK_CHECK(...)
#endif

enum stackExits{
    OK = 0,
    ERR = 1,
    DATA_EMPTY = 2,
    SIZE_OVERFLOW = 3,
    SIZE_UNDERFLOW = 4,
    STK_NULL = 5,
    MEM_FULL = 6,
    CNR_STK_ERR = 7,
    CNR_BUF_ERR = 8

};

typedef long long StackElem_t;

typedef struct Stack_t{
    CNR_PRT         (size_t chicken_first;)
    DBG             (const char* name;)
    DBG             (const char* filename;)
    DBG             (size_t line;)
    StackElem_t*            data;
    size_t                  size;
    size_t                  capacity;
    HASH_PRT        (size_t bufferHash;)
    HASH_PRT        (size_t stackHash;)
    CNR_PRT         (size_t chicken_second;)
} Stack_t;

stackExits StackCtor   (Stack_t* stk                     DBG(, const char* fileName, int line));
stackExits StackDtor   (Stack_t* stk                     DBG(, const char* fileName, int line));
stackExits StackPush   (Stack_t* stk, StackElem_t  item  DBG(, const char* fileName, int line));
stackExits StackPop    (Stack_t* stk, StackElem_t* item  DBG(, const char* fileName, int line));
stackExits StackDump   (Stack_t* stk, const char* filename, int line);
stackExits StackVerify (Stack_t* stk);
