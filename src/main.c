#include "orx.h"

orxSTATUS orxFASTCALL Init() { return orxSTATUS_SUCCESS; }
orxSTATUS orxFASTCALL Run()  { return orxSTATUS_SUCCESS; }
void orxFASTCALL Exit() {}
void orxFASTCALL Update(const orxCLOCK_INFO *_pstClockInfo, void *_pContext) {}

int main(int argc, char **argv) {
    orx_Execute(argc, argv, Init, Run, Exit);
    return EXIT_SUCCESS;
}
