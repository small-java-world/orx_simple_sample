# 🛠️ Orx 1.15 で「画像が出ない & ログが出力されない」問題を一気に解決するガイド

---

## 0. 結論サマリ

| 症状 | 即効解決策 |
|------|------------|
| ログがまったく出ない | **Debug ビルド用ライブラリ (`orxd.lib` + `orxd.dll`) をリンク & 実行** |
| `orxObject_GetWorldBoundingBox` 等が未解決シンボル | **Debug 用ライブラリへ切替** or **`__orxSTATIC__` マクロの不一致** |
| ボタン画像が出ない | `TexturePath` / `[Resource]` の設定とログで読み込み失敗を確認 |

---

## 1. なぜログが出ないのか？

- **Orx は「Release ライブラリ (orx.lib / orx.dll)」ではデフォルトでログが _完全に無効化_ されています**  
  - `orxLOG()` 系は `#ifndef __orxDEBUG__` ガードで空関数になる
- `.ini` や `-log-console` で設定しても _コンパイル時に削除_ されるため出力されない

### ✅ 対処

1. **リンク先を Debug ビルド (`orxd.lib`, `orxd.dll`) に切り替える**  
   ```cmake
   # Debug/Release で自動切替
   target_link_libraries(MyOrxTest
       $<$<CONFIG:Debug>:orxd>
       $<$<CONFIG:Release>:orx>
   )
   ```
2. `build/Debug/` へ `orxd.dll` をコピーして実行（または Release でも orxd.dll を同フォルダに置く）

---

## 2. 未解決シンボル問題の正体

| 関数 | なぜリンクできない？ |
|------|---------------------|
| `orxObject_GetWorldBoundingBox` | Release ビルドでは `ORX_DEBUG` ブロック内だけに実装 |
| `orxAABOX_TestPoint` | 同上 |
| `orxObject_Dump` | **完全にデバッグ専用** (`__orxDEBUG__`) |

### ✅ 対処

- **Debug ライブラリ (`orxd.*`) にリンクすれば解決**  
- どうしても Release で欲しい場合は **Orx を再ビルド** し、`__orxDEBUG__` マクロを有効にして ReleaseDLL を作る

---

## 3. ボタン画像が出ないチェックリスト

| チェック | 解説 |
|----------|------|
| `[Resource] TexturePath = .` | 画像を実行フォルダ直下から読み込むなら必須 |
| ログで `Can't find texture` | まずは Debug ビルドでログ確認 |
| `[Camera]` セクション | 省略すると何も見えない場合がある |
| Z 座標 | `Position = (x, y, 0.5)` 以上推奨 |
| DLL/EXE と同ディレクトリに `button.png` | パスミスが最も多い |

---

## 4. 修正版 `MyOrxTest.ini`（確実にログ生成）

```ini
[Log]
Type      = file console
FileName  = MyOrxTest.log
Level     = debug

[Display]
ScreenWidth  = 800
ScreenHeight = 600
Title        = "Orx Button Test"

[Camera]
FrustumWidth  = 800
FrustumHeight = 600
FrustumNear   = -1
FrustumFar    = 1
Position      = (400, 300, -1)

[Resource]
TexturePath = .
Texture      = button.png

[MainObject]
Graphic   = ButtonGraphic
Position  = (400, 300, 0.5)

[ButtonGraphic]
Texture = button.png
```

---

## 5. CMakeLists 修正例（Debug/Release切替 & 定義）

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyOrxTest C)

set(ORX_ROOT "D:/Orx/orx")  # SDK ルート

include_directories(${ORX_ROOT}/include)
link_directories(${ORX_ROOT}/lib/dynamic)

add_executable(MyOrxTest src/main.c)

# Debug 用マクロを明示（必須ではないが念のため）
target_compile_definitions(MyOrxTest PRIVATE
    $<$<CONFIG:Debug>:__orxDEBUG__>
)

# Debug ⇔ Release でリンクライブラリ切替
target_link_libraries(MyOrxTest
    $<$<CONFIG:Debug>:orxd>
    $<$<CONFIG:Release>:orx>
)
```

---

## 6. 手順まとめ

1. **Debug モードでビルド (`cmake --build . --config Debug`)**  
2. `orxd.dll` を EXE と同じフォルダにコピー  
3. 上記 `MyOrxTest.ini` を EXE と同フォルダへ  
4. `.\MyOrxTest.exe` を実行し **`MyOrxTest.log` を確認**  
5. 画像ロード失敗などがログに残るので、パス・Resource を修正  

---

これで **ログが出ない / ボタンが出ない / 未解決シンボル** の三大トラブルを一括で解決できます。