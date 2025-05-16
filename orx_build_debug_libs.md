# 🚧 Orx 1.15 – Debug ライブラリ (`orxd.lib / orxd.dll`) を自前ビルドしてログを出す

---

## 1. 前提

| 環境 | 値 |
|------|----|
| OS | Windows |
| コンパイラ | Visual Studio 2022 (MSVC) |
| CMake | 3.16 以上 |
| Orx ソース | `D:/Orx/orx` に clone 済み (`--recursive` 付与) |

---

## 2. Debug ライブラリをビルドする手順（CLI）

```powershell
# ❶ Orx ルートへ
cd D:/Orx/orx

# ❷ ビルド用ディレクトリを作成
mkdir build-debug ; cd build-debug

# ❸ VS2022 x64 のソリューション生成
cmake .. -G "Visual Studio 17 2022" -A x64

# ❹ Debug 構成だけビルド
cmake --build . --config Debug
```

| 出力ファイル | 出力先 |
|--------------|--------|
| `orxd.lib`   | `D:/Orx/orx/lib/dynamic/` |
| `orxd.dll`   | `D:/Orx/orx/bin/`         |

---

## 3. CMakeLists.txt の修正ポイント

```cmake
# ライブラリ検索ディレクトリ
link_directories(${ORX_ROOT}/lib/dynamic)

# Debug と Release でリンク先切替
target_link_libraries(MyOrxTest
    $<$<CONFIG:Release>:orx>
    $<$<CONFIG:Debug>:orxd>
)
```

> `ORX_ROOT` は `D:/Orx/orx` を想定。  
> **注意**: Debug 構成の実行フォルダには **`orxd.dll` を必ずコピー**！

---

## 4. 実行フォルダ構成（Debug）

```
build/Debug/
├── MyOrxTest.exe
├── MyOrxTest.ini
├── orxd.dll       ← 新しくコピー
└── button.png
```

---

## 5. ログ設定例（MyOrxTest.ini）

```ini
[Log]
Type     = file console   ; ファイル&コンソール
FileName = MyOrxTest.log
Level    = debug          ; 詳細ログ
```

---

## 6. Release でログを有効にしたい場合

```powershell
cd D:/Orx/orx/build-release
cmake .. -DCMAKE_BUILD_TYPE=Release -DORX_DEBUG=ON ^
      -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

> これで **Release 用 `orx.lib / orx.dll`** にもログ関数が含まれる。  
> サイズ増加とパフォーマンス低下に注意。

---

### まとめ

1. **Debug ビルドを作成 → `orxd.lib` / `orxd.dll` 出現**  
2. **Debug 構成では `orxd` をリンク、DLL を同フォルダへ**  
3. `.ini` の `[Log]` か `-log-console` でログ確認  
4. Release でもログが欲しければ `-DORX_DEBUG=ON` で再ビルド

これで **ログが出ない / デバッグ関数が未解決シンボル** の問題は解消し、画像表示トラブルの原因もログで追跡できます！
