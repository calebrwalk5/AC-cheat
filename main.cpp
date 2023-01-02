#include "ProcessManager.h"

int main() {
    // https://stackoverflow.com/a/43796871
    char line[128];
    FILE *cmd = popen("pidof -s ac_client", "r");
    unsigned long processId = 0;

    fgets(line, 128, cmd);
    processId = strtoul(line, NULL, 128);

    printf("%ld\n", processId);

    pclose(cmd);

    const char szSignature[] = "/x48/x8b/x45/x28/x83/x01/x48"; // https://guidedhacking.com/

    char szOpCode[] = "/x90/x90/x90"; // https://guidedhacking.com/

    ProcessManager *procManager = new ProcessManager(processId, "ac_client");

    procManager->SignaturePayload(szSignature, szOpCode, strlen(szSignature), strlen(szOpCode), 64, 4);

    delete procManager;

    exit(EXIT_SUCCESS);
}
