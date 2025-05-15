# 🧩 Orx 画像（ボタン）が表示されないときの原因調査ガイド

このドキュメントは、Orx 1.15 を用いてボタン画像を画面に表示しようとした際に**画像が出ない／表示されない**場合のチェック項目と対処法をまとめたものです。

---

## ✅ 1. 実行ログの確認（最重要）

PowerShell で `.\MyOrxTest.exe` を実行した直後、**ログに以下のような出力があるか**を確認してください：

### ✔️ 出ていてほしいメッセージ例

```log
[RESOURCE] Adding texture : button.png
[OBJECT] Creating object : MainObject
```

### ❌ 出ていると失敗の兆候

```log
[GRAPHIC] Can't find texture named button.png
[CONFIG] Failed to create object : MainObject
```

**ログが出ない場合 → PowerShell で実行し直す or `orxLOG` でファイル出力を追加**

---

## 📋 2. チェックリスト：よくある原因と対処

| 問題 | 原因 | 対処 |
|------|------|------|
| 画像が表示されない | `MainObject` を作っていない | `orxObject_CreateFromConfig("MainObject")` を確認 |
| テクスチャが読み込まれない | `[Resource]` セクションがない | `Texture = button.png` を `[Resource]` に追加 |
| 透明になっている | Alpha=0 or Scale=0 | `Scale = (1,1,1)` 明示 or `Alpha` 記述なしを確認 |
| Z座標がカメラ外 | `Z = 0` で見えない | `Z = 0.5` や `Z = 1` に変更 |
| カメラが定義されてない | デフォルトカメラが未定義 | `[Camera]` セクションを `config.ini` に追加 |
| パスが違う | `button.png` が無い or 場所が違う | `MyOrxTest.exe` と同じ `Release` に置く |

---

## 🧪 最小で確実に表示される `MyOrxTest.ini` 例（完全版）

```ini
[Display]
ScreenWidth  = 800
ScreenHeight = 600
Title        = "Orx Test Window"

[Camera]
FrustumWidth  = 800
FrustumHeight = 600
FrustumNear   = -1
FrustumFar    = 1
Position      = (400, 300, -1)

[Resource]
Texture = button.png

[MainObject]
Graphic   = ButtonGraphic
Position  = (400, 300, 0.5)

[ButtonGraphic]
Texture = button.png
```

---

## 📂 ファイル配置の正解（最重要）

```
MyOrxTest/
└── build/
    └── Release/
        ├── MyOrxTest.exe
        ├── MyOrxTest.ini      ← この中で button.png を指定
        ├── orx.dll
        └── button.png         ← 絶対にここに存在！
```

---

## 🧠 応用Tips

- `orxMouse_GetPosition()` + `orxAABOX_TestPoint()` でクリック判定が可能
- `orxObject_Dump()` を使うと内部の構成をデバッグ可能
- `.ini` を `orxConfig_Save()` で保存し、設定の確認にも使える

---

## 🔚 お願い

上記確認でも問題が解決しない場合は、以下を添えて問い合わせください：

- PowerShell での実行ログ全文（or スクリーンショット）
- 実際の `MyOrxTest.ini` 内容
- ボタン画像のファイル名・サイズ・配置パス

一緒に確実に原因を特定して、表示できるようにしましょう！
