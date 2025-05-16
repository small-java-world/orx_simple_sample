# 🗒️ Orx でログを確実に出力する方法

このドキュメントは、Orx 1.15 プロジェクトで**ログファイルを常に生成し、コンソールにも出力させる**最小手順をまとめたものです。

---

## 1. `.ini` に `[Log]` セクションを追加

**MyOrxTest.ini**（または任意の設定ファイル）に以下を追記します：

```ini
[Log]
Type        = file console   ; ← ファイル + コンソール両方に出力
FileName    = MyOrxTest.log  ; ← 出力するログファイル名
```

- `Type` には `file`, `console`, `debug` をスペース区切りで組み合わせ可能  
- `FileName` を省略すると `orx.log` という名前になります

---

## 2. `orx_Log_Setup` をコードで呼び出す（任意）

`main.c` の `Init()` で動的に設定することも可能です：

```c
orxSTATUS orxFASTCALL Init() {
    /* ログセットアップ：verbosity, タイプ, ファイル名 */
    orxLOG_Setup(orxLOGGER_LEVEL_DEBUG, orxFALSE, "file console", "MyOrxTest.log");

    orxObject_CreateFromConfig("MainObject");
    return orxSTATUS_SUCCESS;
}
```

> INI で `[Log]` を書いている場合は **不要**。コード優先で上書き。

---

## 3. コマンドライン引数で指定（デバッグ時に便利）

```powershell
.\MyOrxTest.exe -log-file Custom.log -log-console
```

| オプション | 内容 |
|------------|------|
| `-log-file <name>` | 指定したファイル名でログを出力 |
| `-log-console`     | コンソール表示を強制有効 |
| `-log-verbosity <level>` | `debug/info/warning/error` を指定 |

---

## 4. 出力結果

実行ディレクトリに **MyOrxTest.log** が生成され、  
PowerShell には同内容がリアルタイムで表示されます。

```
[RESOURCE] Adding texture : button.png
[OBJECT] Creating object : MainObject
[LOG] Button clicked at (412.000000, 298.000000) - Bounding box check skipped
```

---

## 5. トラブルシューティング

| 症状 | 原因と対処 |
|------|-----------|
| `.log` が生成されない | `[Log]` セクションを書き忘れ or `FileName` パスが存在しない |
| コンソールに出ない | `Type = file` しか設定していない → `file console` にする |
| 各行の先頭が `[?]` になる | `log-verbosity` が低すぎる → `-log-verbosity debug` |

---

## 6. すぐ試せる最小 `MyOrxTest.ini`

```ini
[Display]
ScreenWidth  = 800
ScreenHeight = 600
Title        = "Orx Button Test"

[Log]
Type        = file console
FileName    = MyOrxTest.log

[Resource]
Texture = button.png

[Camera]
FrustumWidth  = 800
FrustumHeight = 600
FrustumNear   = -1
FrustumFar    = 1
Position      = (400, 300, -1)

[MainObject]
Graphic   = ButtonGraphic
Position  = (400, 300, 0.5)

[ButtonGraphic]
Texture = button.png
```

これで次回実行時に必ず**`MyOrxTest.log`** が生成され、クリックログもファイル＆コンソールに記録されます。
