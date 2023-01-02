#include "ProcessManager.h"
#include <iostream>

int main() {
    unsigned long processId = 0;
    printf("Process ID: ");
    std::cin >> processId;

    const char szSignature[] = "/x48/x8b/x45/x28/x83/x01/x48"; // https://guidedhacking.com/

    char szOpCode[] = "/x90/x90/x90"; // https://guidedhacking.com/

    ProcessManager *procManager = new ProcessManager("ac_client", processId, "ac_client");

    procManager->SignaturePayload(szSignature, szOpCode, strlen(szSignature), strlen(szOpCode), 64, 4);

    delete procManager;

    exit(EXIT_SUCCESS);
}
