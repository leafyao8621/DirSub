#include "FindSize.h"
#include <stdio.h>
#include <string.h>

typedef struct DirSizeCnt {
    wchar_t* dir;
    DWORD64 size;
    DWORD64 cnt;
} DirSizeCnt;

DWORD Handler(void* data) {
    DirSizeCnt* dsc = (DirSizeCnt*)data;
    FindSize(dsc->dir, &(dsc->size), &(dsc->cnt));
    return 0;
}

void InsertGroup(wchar_t* in, wchar_t* out) {
    int oind = wcslen(in) + (wcslen(in) % 3 ? wcslen(in) / 3 : wcslen(in) / 3 - 1);
    int ocnt = 0;
    for (int i = wcslen(in), cnt = 0; i >= 0; i--, cnt++) {
        out[oind--] = in[i];
        if (cnt && !(cnt % 3)) {
            out[oind--] = L',';
            ocnt++;
        }
    }
}
int main() {
    WIN32_FIND_DATAW found;
    wchar_t cur[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, cur);
    DirSizeCnt dsc;
    dsc.dir = cur;
    HANDLE h = CreateThread(NULL, 0, Handler, &dsc, 0, 0);
    WaitForSingleObject(h, INFINITE);
    wchar_t in[50];
    wchar_t out[50];
    swprintf(in, 50, L"%lld", dsc.size);
    InsertGroup(in, out);
    wprintf(L"\nSize of %s containing %lld files is %s Bytes\n\n", cur, dsc.cnt, out);
    fflush(stdout);
    wchar_t query[MAX_PATH];
    swprintf(query, MAX_PATH, L"%s\\*", cur);
    HANDLE handle = FindFirstFileW(query, &found);
    FindNextFileW(handle, &found);
    wprintf(L"Sub directories of %s\n\n", cur);
    while (FindNextFileW(handle, &found)) {
        wchar_t query[MAX_PATH];
        switch (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        case FILE_ATTRIBUTE_DIRECTORY:
            swprintf(query, MAX_PATH, L"%s\\%s", cur, found.cFileName);
            dsc.dir = query;
            h = CreateThread(NULL, 0, Handler, &dsc, 0, 0);
            WaitForSingleObject(h, INFINITE);
            swprintf(in, 50, L"%lld", dsc.size);
            InsertGroup(in, out);
            wprintf(L"%-25s %-10lld Files %-20s Bytes\n", found.cFileName, dsc.cnt, out);
        }
    }
    system("pause");
}