# KNX KMP Watermeter — Firmware

RP2040 firmware implementing the Kamstrup Meter Protocol (KMP) over UART to read data from Kamstrup Multical 601/801 water/heat meters and publish it on the KNX TP1 bus.

## Setup

```bash
pip install platformio
```

## Build

```bash
pio run -e release
```

Flash `.pio/build/release/firmware.uf2` via UF2 bootloader (hold BOOTSEL, press RESET, copy UF2 to mounted drive).

## Unit Tests

```bash
pio test -e test
```

## Static Code Analysis

```bash
sudo apt-get install -y cppcheck
pio check -e check
```

## Protocol Reference

- [Kamstrup.md](Kamstrup.md) — KMP protocol description (extracted from Kamstrup document 5512-447)

## Architecture

The firmware implements a layered KMP protocol stack:

| Layer | Responsibility |
|---|---|
| `PhysicalLayer` | UART framing, byte-stuffing, start/stop detection |
| `DataLinkLayer` | Destination addressing, CRC-16/CCITT verification |
| `ApplicationLayer` | Command dispatch (GetSerialNo, GetRegister), response routing |

Devices polled via servo-actuated wake-up mechanism. See `src/application/WatermeterApp.cpp` for the main polling loop.
