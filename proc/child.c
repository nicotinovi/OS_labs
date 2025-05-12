#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;
    HANDLE hMutex;

    // Открываем мьютекс
    hMutex = OpenMutex(SYNCHRONIZE, FALSE, TEXT("MyMutex"));
    if (hMutex == NULL) {
        printf("OpenMutex failed (%d)\n", GetLastError());
        return 1;
    }

    // Открываем объект разделяемой памяти
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("MySharedMemory"));
    if (hMapFile == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
        return 1;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Читаем данные с синхронизацией
    WaitForSingleObject(hMutex, INFINITE);
    printf("Child process read: %s\n", pBuf);
    ReleaseMutex(hMutex);

    // Очистка
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hMutex);

    return 0;
}