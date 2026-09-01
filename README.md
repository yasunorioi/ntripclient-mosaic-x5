# ntripclient-mosaic-x5

**M5Atom Lite を使った、Septentrio mosaic-X5 rover 向けの NTRIP クライアント。**

ロジックは共通コア [ntripclient-core](https://github.com/yasunorioi/ntripclient-core)
(`NtripRover.h`)にあり、この sketch は X5 固有の設定だけを渡す薄いラッパ
(`src/main.cpp` ~15 行)。**G5-P3H 版と同一ファーム**で、違いは繋ぐ COM と board 名だけ。

NTRIP で拾った RTCM3 を Grove UART(3.3V TTL)経由で mosaic の **COM2** に流し込み
RTK 測位させ、COM2 から返る GGA+GSA+GSV を読んで **fix-LED + skyplot/C/N0 Web モニタ**を出す。

## この個体(mosaic-X5)の特徴

- **フル RTK Rover + Base 可**(P3H は rover 専用)。基準局にもできる。
- **ASCII コマンドに応答する**(COM input=auto)。ただし RTCM3 と共有の UART では
  auto 検出が RTCM にロックして Atom からのコマンドを黙殺しがち → **boot config で
  NMEA を出す**のが確実(下記)。
- 全 COM 既定 115200。

## データフロー

```
 caster (rtk.toiso.fit:2101/eniwa-bd982, anonymous)
     │  RTCM3 over WiFi (NTRIP)
     ▼
 M5Atom Lite ── Grove UART G26/G32 (3.3V TTL) ──►  mosaic-X5 COM2  (RTCM3 IN → RTK fixed)
             ◄── NMEA GGA+GSA+GSV ─────────────    mosaic-X5 COM2  (LED + Web モニタ)
```

## 配線

| Atom (Grove) | 向き | mosaic COM2 |
|---|---|---|
| G26 (Serial2 TX) | ──► | COM2 RX |
| G32 (Serial2 RX) | ◄── | COM2 TX |
| GND | ── | GND |

> fix が出ない時はまず **G26/G32 の入れ違い**を疑う。

## 受信機セットアップ(RxTools/USB で一度だけ)

COM2 に GGA+GSA+GSV を出し、boot config に保存する:

```
setNMEAOutput, Stream1, COM2, GGA+GSA+GSV, sec1
exeCopyConfigFile, Current, Boot
```

> ⚠ `exeCopyConfigFile, Current, Boot` を忘れると電源再投入で NMEA 設定が消える。
> skyplot/DOP は GSV/GSA が来ている時だけ描画される(GGA だけなら status のみ)。

## ビルド / 焼き

PlatformIO。**M5 Atom Lite の CH9102F は 115200 固定**(230400+ はほぼ失敗)。

```sh
pio run                 # build
pio run -t upload       # flash
pio device monitor      # 115200
```

Web UI は STA 接続後 `http://ntrip-rover.local/`。設定ポータルは起動時ボタン長押しで
SSID `NTRIP-Client`(スマホで繋ぐと設定ページが自動で開く)。

## ライセンス

MIT。依存する `NTRIPClient` は LGPL-3.0(未改変参照)。
