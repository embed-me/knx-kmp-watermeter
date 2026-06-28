# KNX KMP Watermeter

![Pipeline](https://github.com/embed-me/knx-up-buzzer/actions/workflows/pipeline.yml/badge.svg)

KNX TP1 interface for Kamstrup water/heat meters via KMP protocol on RP2040.

## Setup

```bash
pip install platformio
```

## Build

```bash
pio run -e release
```

Flash `.pio/build/release/firmware.uf2` via UF2 bootloader.

## Unit Tests

```bash
pio test -e test
```

## Static Code Analysis

```bash
sudo apt-get install -y cppcheck
pio check -e check
```
