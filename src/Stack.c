#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

void InitStack(Stack* stack) {
    stack->size = 0;
    stack->MaxSize = 100;
    stack->path = malloc(sizeof(wchar_t*) * 100);
}

void push(Stack* stack, wchar_t* path) {
    if (stack->size == stack->MaxSize) {
        stack->MaxSize <<= 1;
        stack->path = (wchar_t**)realloc(stack->path, sizeof(wchar_t*) * (stack->MaxSize));
    }
    stack->path[stack->size++] = path;
}

wchar_t* pop(Stack* stack) {
    return stack->path[--stack->size];
}

void FreeStack(Stack* stack) {
    for (int i = 0; i < stack->size; i++) {
        free(stack->path[i]);
    }
    free(stack->path);
}