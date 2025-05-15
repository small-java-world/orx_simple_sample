#include "orx.h"

orxOBJECT *pstButton = orxNULL;

orxSTATUS orxFASTCALL Init() {
    pstButton = orxObject_CreateFromConfig("MainObject");
    return orxSTATUS_SUCCESS;
}

orxSTATUS orxFASTCALL Run() {
    if (orxInput_HasBeenActivated("MOUSE_LEFT")) {
        orxVECTOR vMouse;
        orxMouse_GetPosition(&vMouse);
        // orxAABOX stBox; // コメントアウト
        // orxObject_GetWorldBoundingBox(pstButton, &stBox); // コメントアウト
        // if (orxAABOX_TestPoint(&stBox, &vMouse)) { // コメントアウト
            orxLOG("Button clicked at (%f, %f) - Bounding box check skipped", vMouse.fX, vMouse.fY); // ログメッセージ変更
        // } // コメントアウト
    }
    return orxSTATUS_SUCCESS;
}

void orxFASTCALL Exit() {
    orxObject_Delete(pstButton);
}

void orxFASTCALL Update(const orxCLOCK_INFO *_pstClockInfo, void *_pContext) {}

int main(int argc, char **argv) {
    orx_Execute(argc, argv, Init, Run, Exit);
    return EXIT_SUCCESS;
}
