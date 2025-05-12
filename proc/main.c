#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;
    HANDLE hMutex;

    // Создаём мьютекс для синхронизации
    hMutex = CreateMutex(NULL, FALSE, TEXT("MyMutex"));
    if (hMutex == NULL) {
        printf("CreateMutex failed (%d)\n", GetLastError());
        return 1;
    }

    // Создаём объект разделяемой памяти
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // использование paging file
        NULL,                    // защита по умолчанию
        PAGE_READWRITE,          // чтение/запись
        0,                       // максимальный размер (high-order DWORD)
        256,                     // максимальный размер (low-order DWORD)
        TEXT("MySharedMemory")); // имя объекта

    if (hMapFile == NULL) {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);

    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Запускаем дочерний процесс (например, child.exe)
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(
        TEXT("child.exe"), // путь к дочернему процессу
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)) 
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    // Записываем данные с синхронизацией
    WaitForSingleObject(hMutex, INFINITE);
    sprintf((char*)pBuf, "Hello from parent process!");
    ReleaseMutex(hMutex);

    // Ждём завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Очистка
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hMutex);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}