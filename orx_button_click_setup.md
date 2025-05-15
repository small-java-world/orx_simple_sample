# 🖱️ Orx でボタンを表示してイベントを発火する手順書

このガイドは、Orx 1.15 を使用して、画面上にボタン画像を表示し、クリックされたときにイベントを検知する最小構成の手順を示します。

---

## 📁 プロジェクト構成（変更追加）

```
MyOrxTest/
├── CMakeLists.txt
├── data/
│   ├── config.ini
│   └── object.ini         # オブジェクト定義（ボタン）
├── src/
│   └── main.c
└── build/
    └── Release/
        ├── MyOrxTest.exe
        ├── MyOrxTest.ini
        ├── orx.dll
        └── button.png     # ボタン画像（200x60 推奨）
```

---

## 🛠️ ファイル内容の変更

### data/config.ini（MyOrxTest.ini）

```ini
[Display]
ScreenWidth  = 800
ScreenHeight = 600
Title        = "Orx Button Test"

[Resource]
Texture = button.png

[Input]
SetList = MainInput
MainInput = MOUSE_LEFT

[MainObject]
Graphic   = ButtonGraphic
Position  = (400, 300, 0)

[ButtonGraphic]
Texture = button.png
```

---

### data/object.ini（新規）

```ini
[MainObject@Button]
Graphic   = ButtonGraphic
Position  = (400, 300, 0)
```

---

### src/main.c（クリックイベント付き）

```c
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
        orxAABOX stBox;
        orxObject_GetWorldBoundingBox(pstButton, &stBox);
        if (orxAABOX_TestPoint(&stBox, &vMouse)) {
            orxLOG("Button clicked at (%f, %f)", vMouse.fX, vMouse.fY);
        }
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
```

---

## ⚙️ 実行準備

### 必須素材

- `data/button.png`（200×60pxのボタン画像）
- `MyOrxTest.ini` の中で `button.png` を読み込んでいることを確認

### 実行時準備

```bash
# button.png を Release ディレクトリへ
copy data/button.png build/Release/

# config.ini → MyOrxTest.ini としてコピー
copy data/config.ini build/Release/MyOrxTest.ini
```

---

## ✅ 実行確認

```bash
cd build/Release
MyOrxTest.exe
```

- 画面中央にボタン画像が表示される
- マウス左クリックを押すとログに座標が表示される

---

## 🔁 補足

- 当たり判定を追加したい場合、`orxAABOX_TestPoint()` を使用してマウス座標とオブジェクトの当たり判定を確認
- `Input` セクションで複数のマッピングを行うことが可能
