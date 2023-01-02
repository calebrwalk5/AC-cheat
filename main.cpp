/* Thank you GPT-3 for annotating my code <3 */
#include "ProcessManager.h"

int main() {
    const char szSignature[] = "/x48/x8b/x45/x28/x83/x01/x48"; // https://guidedhacking.com/
    char szOpCode[] = "/x90/x90/x90"; // https://guidedhacking.com/

    ProcessManager procManager("ac_client");

    procManager.SignaturePayload(szSignature, szOpCode, strlen(szSignature), strlen(szOpCode), 64, 4);

    exit(EXIT_SUCCESS);
}