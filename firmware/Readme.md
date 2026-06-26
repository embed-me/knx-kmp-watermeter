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

## Test

```bash
pio test -e test
```

## Check

```bash
sudo apt-get install -y cppcheck
pio run -e release                     # download platform & libs
cppcheck --enable=all --error-exitcode=1 \
  -i.pio -isrc/.pio \
  -DKNX_NO_AUTOMATIC_GLOBAL_INSTANCE \
  -DMASK_VERSION=0x07B0 \
  -DKNX_FLASH_SIZE=0x8000 \
  -DUSE_TP_RX_QUEUE \
  -DPIO_FRAMEWORK_ARDUINO_ENABLE_RTTI \
  --suppress=missingInclude \
  src/application/ src/drivers/ src/utils/ src/main.cpp
```
