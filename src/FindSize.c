#include "FindSize.h"
#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

void FindSize(wchar_t* DirName, DWORD64* acc, DWORD64* cnt) {
    Stack stack;
    *acc = 0;
    *cnt = 0;
    InitStack(&stack);
    wchar_t root[MAX_PATH];
    swprintf(root, MAX_PATH, L"%s", DirName);
    push(&stack, root);
    while (stack.size) {
        wchar_t* cur = pop(&stack);
        wchar_t query[MAX_PATH];
        swprintf(query, MAX_PATH, L"%s\\*", cur);
        WIN32_FIND_DATAW found;
        HANDLE handle = FindFirstFileW(query, &found);
        FindNextFileW(handle, &found);
        while (FindNextFileW(handle, &found)) {
            wchar_t* buf = malloc(MAX_PATH * sizeof(wchar_t));
            switch (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            case FILE_ATTRIBUTE_DIRECTORY:
                swprintf(buf, MAX_PATH, L"%s\\%s", cur, found.cFileName);
                push(&stack, buf);
                break;
            default:
                *acc += ((DWORD64)found.nFileSizeHigh * (MAXDWORD + 1)) + found.nFileSizeLow;
                (*cnt)++;
            }
        }
    }
    FreeStack(&stack);
}