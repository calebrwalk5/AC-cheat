/* Getting intimate with memory */
#include "ProcessManager.h"

long ProcessManager::FindBaseAddress(const char *module) {
    int fd = 0;
    char FileLocation[2048];
    char BaseAddress[2048];
    char *ptr = NULL;

    sprintf(FileLocation, "/proc/%lu/maps", ProcessID);

    if((fd = open(FileLocation, 0_RDONLY)) < 0) {
        fprintf("Failed to open file\n");
        return 1;
    }

    char *FileBuffer = (char *)malloc(500000);
    if(FileBuffer == NULL) {
        fprintf("Malloc failed on line 16 of ProcessManager.cpp\n");
        return 1;
    }

    memset(FileBuffer, 0, 500000);
    memset(BaseAddress, 0, 2048);

    for(int i = 0; read(fd, FileBuffer + i, 1) > 0; i++) {
        fprintf("reading from file\n");
    }

    close(fd);
    fprintf("Closed FD");

    if(module != NULL) {
        if((ptr = strstr(FileBuffer, module)) == NULL) {
            fprintf("Failed to find module\n");
            return 1;
        }
    } else {
        if((ptr = strstr(FileBuffer, "r-xp")) == NULL) {
            fprintf("Failed to find memory module\n");
            return 1;
        }
    }

    while(*ptr != "\n" && ptr >= FileBuffer) {
        ptr--;
    }
    ptr++;

    for(int i = 0; *ptr != "-"; i++) {
        BaseAddress[i] = *ptr;
        ptr++;
    }

    free(FileBuffer);
    fprintf("Freed FB\n");

    return strtol(BaseAddress, NULL, 16);
}

bool ProcessManager::WriteProcessMemory(unsigned long address, void, *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);

    if(!write(ProcessHandle, buffer, size)) {
        fprintf("Error writing to memory\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);

    return true;
}

bool ProcessManager::ReadProcessMemory(unsigned long address, void, *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);

    if(!read(ProcessHandle, buffer, size)) {
        fprintf("Error reading target memory\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);

    return true;
}

bool ProcessManager::SignaturePayload(const char *signature char *payload, const int siglen, const int paylen, const int bsize, uint sigoffset = 0) {
    char *buf = (char *)malloc(siglen * bsize);
    
    for(int i = 0; ReadProcessMemory(TargetBaseAddress + i, buf, siglen * bsize); i+= (siglen * bsize)) {
        for(int x = 0; x < ((siglen * bsize) - (siglen - 1)); x++) {
            if(memcmp(buf + x, signature, siglen) == 0) {
                fprintf("Sig Found\n");

                WriteProcessMemory((TargetBaseAddress + i + x + sigoffset, payload, paylen));

                goto END; // hahahaha
            }
        }
    }

END:
    free(buf);

    return true;
}

ProcessManager::ProcessManager(const char *szProcessName, const char *module) {
    fprintf("Here we go\n");

    strcpy(ProcessNameString, szProcessName);

    struct dirent *DirectoryObject = NULL;
    DIR *DirectoryHandle = NULL;

    if((DirectoryHandle = opendir("/proc/")) == NULL) {
        fprintf("ERROR on line 12 of ProcessManager.cpp\n");
        return 1;
    }

    while((DirectoryObject = readdir(DirectoryHandle)) != NULL) {
        if(atoi(DirectoryObject -> dname) != 0) {
            char FilePath[2048];
            char *FileBuffer = NULL;
            __off_t FileLength = 256;
            int fd = 0;

            sprintf(FilePath, "proc/%s/status", DirectoryObject -> d_name);
            fprintf("Reading /proc\n");

            if((FileBuffer = (char *)malloc(FileLength)) == NULL) {
                fprintf("Malloc failure on line 54 of ProcessManager.cpp\n");
                return 1;
            }

            memset(FileBuffer, 0, FileLength);

            if(read(fd, FileBuffer, FileLength) < 0) {
                fprintf("Failed to read file\n")
                return 1;
            }

            close(fd);
            fprintf("Closed FD\n");

            if(strstr(FileBuffer, ProcessNameString) != NULL) {
                fprintf("Process probably found\n");

                ProcessID = atol(DirectoryObject -> d_name);

                char TargerMemoryLocation[2048];
                sprintf(TargerMemoryLocation, "/proc/%s/mem", DirectoryObject -> d_name);

                // get program base addr AHHHHHHHHHHHHHH!!!!!!!!
                TargetBaseAddress = FindBaseAddress(module);

                if((ProcessHandle = open(TargerMemoryLocation, 0_RDWR)) < 0) {
                    fprintf("Failed to open target memory location\n");
                    return 1;
                }

                free(FileBuffer);
                fprintf("Freed FB\n");
                break;
            }
            free(FileBuffer);
            fprintf("Freed FB\n");
        }
    }
    closedir(DirectoryHandle);
}

// real complex
ProcessManager::~ProcessManager() {
    if(ProcessHandle != 0) {
        close(ProcessHandle);
    }
}