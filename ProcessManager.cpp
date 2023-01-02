#include "ProcessManager.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

ProcessManager::ProcessManager(unsigned long ProcessID, const char *module) {
    printf("Here we go\n");

    char FilePath[2048];

    struct dirent *DirectoryObject = NULL;
    DIR *DirectoryHandle = NULL;

    if((DirectoryHandle = opendir("/proc/")) == NULL) {
        printf("ERROR on line 12 of ProcessManager.cpp\n");
        return;
    }

    while((DirectoryObject = readdir(DirectoryHandle)) != NULL) {
        if(atoi(DirectoryObject->d_name) != 0) {
            char FilePath[2048];
            sprintf(FilePath, "/proc/%s/comm", DirectoryObject->d_name);

            char ProcessName[2048];
            int fd = 0;

            if((fd = open(FilePath, O_RDONLY)) < 0) {
                printf("Failed to open file\n");
                return;
            }

            read(fd, ProcessName, 2048);
            close(fd);

            if(strcmp(ProcessName, module) == 0) {
                ProcessID = atoi(DirectoryObject->d_name);
                break;
            }
        }
    }

    closedir(DirectoryHandle);

    sprintf(FilePath, "/proc/%lu/mem", ProcessID);

    if((ProcessHandle = open(FilePath, O_RDWR)) < 0) {
        printf("Failed to open process\n");
        return;
    }

    TargetBaseAddress = FindBaseAddress(module);
}

long ProcessManager::FindBaseAddress(const char *module) {
    int fd = 0;
    char FileLocation[2048];
    char BaseAddress[2048];
    char *ptr = NULL;

    sprintf(FileLocation, "/proc/%lu/maps", ProcessID);

    if((fd = open(FileLocation, O_RDONLY)) < 0) {
        printf("Failed to open file\n");
        return 0;
    }

    char *FileBuffer = (char *)malloc(500000);
    if(FileBuffer == NULL) {
        printf("Malloc failed on line 16 of ProcessManager.cpp\n");
        return 0;
    }

    memset(FileBuffer, 0, 500000);
    memset(BaseAddress, 0, 2048);

    for(int i = 0; read(fd, FileBuffer + i, 1) > 0; i++) {
        printf("reading from file\n");
    }

    close(fd);
    printf("Closed FD");

    if(module != NULL) {
        if((ptr = strstr(FileBuffer, module)) == NULL) {
            printf("Failed to find module\n");
            return 0;
    }

    while(*ptr != '\n' && ptr >= FileBuffer) {
        ptr--;
    }
    ptr++;

    for(int i = 0; strcmp(ptr, "-") != 0; i++) {
        BaseAddress[i] = *ptr;
        ptr++;
    }

    free(FileBuffer);
    printf("Freed FB\n");

    return strtol(BaseAddress, NULL, 16);
}

bool ProcessManager::WriteProcessMemory(unsigned long address, void *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);

    if(write(ProcessHandle, buffer, size) != size) {
        printf("Error writing to memory\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);

    return true;
}

bool ProcessManager::ReadProcessMemory(unsigned long address, void *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);

    if(read(ProcessHandle, buffer, size) != size) {
        printf("Error reading target memory\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);

    return true;
}

bool ProcessManager::SignaturePayload(const char *signature, char *payload, const int siglen, const int paylen, const int bsize, uint sigoffset) {    
    char *buf = (char *)malloc(siglen * bsize);
    
    for(int i = 0; ReadProcessMemory(TargetBaseAddress + i, buf, siglen * bsize); i += (siglen * bsize)) {
        for(int x = 0; x < ((siglen * bsize) - (siglen - 1)); x++) {
            printf("Sig Found\n");

            WriteProcessMemory((TargetBaseAddress + i + x + sigoffset), payload, paylen);

            break;
        }
    }

    free(buf);

    return true;
}
}