# Orx でシンプルな画面を表示するだけの最小構成手順（コマンドのみ）

このガイドは、**Orx 1.15** を使って「シンプルなウィンドウを開くだけ」のサンプルをコマンド操作のみで実現するための手順です。
**注:** この手順は Windows および PowerShell 環境を前提としています。

---

## 📁 1. プロジェクト構成

```
MyOrxTest/
├── CMakeLists.txt
├── data/
│   └── config.ini  # 初期設定ファイル（実行時には直接使用しない可能性あり）
├── src/
│   └── main.c
└── build/          # ビルド時に作成される
    └── Release/    # 実行ファイル等が出力される
        ├── MyOrxTest.exe
        ├── MyOrxTest.ini # Orxが自動ロードする推奨設定ファイル
        └── orx.dll
```
上記はビルド後の理想的な状態を含みます。初期状態では `build` ディレクトリ内のファイルは存在しません。

---

## 🛠️ 2. ファイル内容

### src/main.c

```c
#include "orx.h"

orxSTATUS orxFASTCALL Init() { return orxSTATUS_SUCCESS; }
orxSTATUS orxFASTCALL Run()  { return orxSTATUS_SUCCESS; }
void orxFASTCALL Exit() {}
void orxFASTCALL Update(const orxCLOCK_INFO *_pstClockInfo, void *_pContext) {}

int main(int argc, char **argv) {
    orx_Execute(argc, argv, Init, Run, Exit);
    return EXIT_SUCCESS;
}
```

---

### data/config.ini
このファイルは、後で `MyOrxTest.ini` としてコピーするための元ファイルです。

```ini
[Display]
ScreenWidth  = 800
ScreenHeight = 600
Title        = "Orx Test Window"
```

---

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyOrxTest C)

# Orx SDK のルートパスを指定します。環境に合わせて変更してください。
# 例: "C:/dev/orx_sdk" や "D:/Libraries/orx" など
set(ORX_ROOT "D:/Orx/orx")  # ★ご自身のOrx SDKのパスに合わせて変更★

include_directories(${ORX_ROOT}/include)

# Orxライブラリのパスを指定します。
# 通常は ${ORX_ROOT}/lib/dynamic や ${ORX_ROOT}/lib の下にあります。
# MSVCの場合は ${ORX_ROOT}/lib/dynamic/visualc や ${ORX_ROOT}/lib/visualc なども確認してください。
link_directories(${ORX_ROOT}/lib/dynamic) # orx.lib のある場所 (環境による)

add_executable(MyOrxTest src/main.c)
target_link_libraries(MyOrxTest orx) # orx.lib (または orxd.lib for debug)
```

---

## ⚙️ 3. 実行手順（コマンドのみ）

### ① プロジェクトディレクトリ作成と移動
```bash
mkdir -p MyOrxTest/{src,data,build}
cd MyOrxTest
```

### ② ソースファイルと初期設定ファイルを作成
エディタや `echo` コマンド等で以下の内容のファイルを作成します。
- `src/main.c` (上記の内容)
- `data/config.ini` (上記の内容)
- `CMakeLists.txt` (上記の内容。`ORX_ROOT` を自身の環境に合わせて修正してください)

### ③ CMakeでビルド
```bash
cd build
# Visual Studio 2022 (x64) の場合
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd .. # プロジェクトルートに戻る (MyOrxTest ディレクトリ)
```

### ④ 実行に必要なファイルの準備
ビルドが成功すると `build/Release/MyOrxTest.exe` が生成されます。
実行には `orx.dll` と設定ファイル (`MyOrxTest.ini`) が必要です。

1.  **`orx.dll` のコピー:**
    Orx SDK の `bin` ディレクトリ (または `code/bin`、`lib/dynamic` 等、`orx.dll` が格納されている場所) から `orx.dll` を `build/Release/` ディレクトリにコピーします。
    SDKのバージョンや構成によってパスが異なるため、ご自身の環境で `orx.dll` を見つけてください。
    例:
    ```bash
    # PowerShellの場合 (SDKのパスは適宜変更)
    # copy D:/Orx/orx/code/bin/orx.dll build/Release/
    ```

2.  **設定ファイル (`MyOrxTest.ini`) の準備:**
    `data/config.ini` を `build/Release/MyOrxTest.ini` としてコピーします。
    これにより、Orxが実行時に自動で設定を読み込みます。
    ```bash
    # PowerShellの場合
    copy data/config.ini build/Release/MyOrxTest.ini
    ```

### ⑤ 実行 (PowerShellから)
`build/Release` ディレクトリから実行します。

```bash
cd build/Release
.\\MyOrxTest.exe
```
引数なしで実行することで、Orxは同じディレクトリにある `MyOrxTest.ini` を自動的に読み込みます。

---

## ✅ 結果

- 800×600 のウィンドウが開き、「Orx Test Window」というタイトルが表示されます。
- ウィンドウの中身は特に描画されません（指定された設定での画面表示のみ）。

---

## 🔁 備考

-   **設定ファイルの読み込み優先度:**
    1.  コマンドライン引数で指定されたファイル (例: `.\\MyOrxTest.exe -i another_config.ini`)
    2.  実行ファイルと同じディレクトリにある `実行ファイル名.ini` (例: `MyOrxTest.ini`)
    3.  実行ファイルと同じディレクトリにある `data/config.ini` (今回の試行ではこの自動検出は期待通りに機能しませんでした。より確実なのは上記2の方法です)
    4.  Orxのデフォルト設定。
-   **`ORX_ROOT` のパス:** `CMakeLists.txt` 内の `ORX_ROOT` は、ご自身のOrx SDKがインストール（または展開）されているルートディレクトリを正しく指している必要があります。
-   **`link_directories` のパス:** `CMakeLists.txt` 内の `link_directories` も、`orx.lib` (またはデバッグ用の `orxd.lib`) が実際に存在するパスを指定する必要があります。SDKの構成によっては `lib/dynamic/visualc` や `lib/static/visualc` など、より詳細なパスになる場合があります。
-   **dllのアーキテクチャ:** ビルド設定（x64/x86）と `orx.dll` のアーキテクチャが一致していることを確認してください。
