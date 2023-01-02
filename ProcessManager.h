/* Thank you GPT-3 for annotating my code <3 */
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

typedef unsigned int uint;

class ProcessManager {
    private: 
    // ProcessNameString is a char array of size 2048
    char ProcessNameString[2048];
    // ProcessID is a long
    long ProcessID;
    // ProcessHandle is an int
    int ProcessHandle;

    // FindBaseAddress is a private member function that takes an optional char pointer as an argument and returns a long
    long FindBaseAddress(const char *module = NULL);

    public:
    // TargetBaseAddress is an unsigned long
    unsigned long TargetBaseAddress;

    // ProcessManager is a constructor that takes a char pointer and an optional char pointer as arguments
    ProcessManager(const char *szProcessName, const char *module = NULL);
    // ProcessManager is a second constructor that takes an unsigned long and a char pointer as arguments
    ProcessManager(unsigned long ProcessID, const char *module);
    // ProcessManager is a destructor
    ~ProcessManager();

    // SignaturePayload is a member function that takes a char pointer, a char pointer, two ints, an int, and an optional uint as arguments and returns a bool
    bool SignaturePayload(const char *signature, char *payload, const int siglen, const int paylen, const int bsize, uint sigoffset);
    
    // WriteProcessMemory is a member function that takes an unsigned long, a void pointer, and a uint as arguments and returns a bool
    bool WriteProcessMemory(unsigned long address, void *buffer, uint size);
    // ReadProcessMemory is a member function that takes an unsigned long, a void pointer, and a uint as arguments and returns a bool
    bool ReadProcessMemory(unsigned long address, void *buffer, uint size);
};
