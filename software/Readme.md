# Software — KNX ETS Product Database

ETS product database for the KNX KMP Watermeter, tested with **ETS 6.6**.

## Quick Start

If you do not need to modify the product database, simply load the [existing product database](productdatabase/) into ETS and use it as any other vendor product.

## Group Objects

| No. | Name | Function | DPT |
|-----|------|----------|-----|
| 1 | Verbunden (Linked) | Keep-alive connection state | DPT_State |
| 2 | Durchfluss (Flow) | Current water flow | DPT_Value_Volume_Flow |
| 3 | Volumen (Volume) | Total volume | DPT_Value_Volume |
| 4 | Temperatur (Temperature) | Water temperature | DPT_Value_Temp |
| 5 | Verbleibende Batterielaufzeit (Battery Life) | Remaining battery life in days | DPT_Value_2_Ucount |

## ETS Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| Keep-Alive Interval | 16-bit | Interval in seconds for keep-alive communication (default: 10) |
| Polling Interval | 16-bit | Interval in seconds between data register reads (default: 20) |
| Volume | 1-bit | Enable/disable volume register read |
| Flow | 1-bit | Enable/disable flow register read |
| Temperature | 1-bit | Enable/disable temperature register read |
| Battery Life | 1-bit | Enable/disable battery life register read |

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

> **Note**: During re-deployment the program may become temporarily unresponsive. Allow it time to complete.
