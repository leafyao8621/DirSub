#include <windows.h>

typedef struct {
	int size;
	int MaxSize;
	wchar_t** path;
} Stack;

void InitStack(Stack* stack);
void push(Stack* stack, wchar_t* path);
wchar_t* pop(Stack* stack);