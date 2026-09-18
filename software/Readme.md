# Software — KNX ETS Product Database

ETS product database for the KNX KMP Watermeter, tested with **ETS 6.6**.

## Quick Start

If you do not need to modify the product database, simply load the [existing product database](productdatabase/) into ETS and use it as any other vendor product.

## Group Objects

| No. | Name | Function | DPT |
|-----|------|----------|-----|
| 1 | Linked | Connection state to the meter, updated on every poll | DPT_State |
| 2 | Flow | Current water flow | DPT_Value_Volume_Flow |
| 3 | Volume | Total volume | DPT_Value_Volume |
| 4 | Temperature | Water temperature | DPT_Value_Temp |
| 5 | Battery Life | Remaining battery life in days | DPT_Value_2_Ucount |
| 6 | Time | Device RTC time sync input | DPT_Time (10.001) |
| 7 | Date | Device RTC date sync input | DPT_Date (11.001) |

## ETS Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| Polling Interval | Text (16 chars) | Crontab-style schedule for register polling (see below) |
| Volume | 1-bit | Enable/disable volume register read |
| Flow | 1-bit | Enable/disable flow register read |
| Temperature | 1-bit | Enable/disable temperature register read |
| Battery Life | 1-bit | Enable/disable battery life register read |

### Polling Interval Format

Standard crontab syntax with five fields: minute (0–59), hour (0–23), day of month (1–31), month (1–12), day of week (1–7, Sunday = 7). Each field supports `*`, step values (`*/15`), ranges (`9-17`), and comma-separated lists (`5,10,15`).

Examples:
- `*/15 * * * *` — every 15 minutes
- `0 9-17 * * 1-5` — hourly between 9:00 and 17:00 on weekdays

The schedule is evaluated against the device RTC, which is synchronized via the Time (6) and Date (7) group objects.

## KMP Registers Polled

| Register ID | Name | Unit |
|-------------|------|------|
| `0x0044` | Volume | m³/l |
| `0x004A` | Flow | l/h |
| `0x0124` | Temperature | °C |
| `0x0246` | Battery Life | days |

## Tools

The product database is created with **[Kaenx-Creator](https://github.com/OpenKNX/Kaenx-Creator)**.

### Recommended Version

Kaenx-Creator is under active development. Use **[v1.8.4](https://github.com/OpenKNX/Kaenx-Creator/tree/v1.8.4)** for compatibility.

- **Binary Download**: [Kaenx Creator v1.8.4](https://github.com/OpenKNX/Kaenx-Creator/releases/download/v1.8.4/Kaenx.Creator.Binaries.zip)
- **MD5 Hash**: `9aeb971f7844eeafb54341031f3e4ef8`

## Regeneration

To modify or recreate the product database:

1. Open `kmp-watermeter.ae-manu` in Kaenx-Creator.
2. Make changes and increase the version number.
3. Re-deploy the database.

Generated outputs include:

- **`knxprod.h`** — Move this file to `firmware/src/drivers/knx/data/`.
