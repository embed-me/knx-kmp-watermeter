# KNX KMP Watermeter

![Pipeline](https://github.com/embed-me/knx-up-buzzer/actions/workflows/pipeline.yml/badge.svg)

KNX TP1 interface for Kamstrup water/heat meters via KMP protocol. Reads meter data and exposes it on the KNX bus.

## Project Structure

| Directory    | Description                                      |
|--------------|--------------------------------------------------|
| `firmware/`  | RP2040 firmware (PlatformIO, C++)                |
| `hardware/`  | PCB design (Altium)                              |
| `housing/`   | 3D-printable enclosure                           |
| `software/`  | KNX ETS product database                         |

## Quick Start

```bash
# Firmware
cd firmware
pip install platformio
pio run -e release       # build
pio test -e test         # test
```

See [firmware/Readme.md](firmware/Readme.md) for static analysis and flashing instructions.

## Hardware

Custom PCB combining RP2040 MCU with NCN5130 BCU (NanoBCU-based) for KNX TP1 bus coupling. Includes buzzer, proximity sensor, and binary input support.

[hardware/Readme.md](hardware/Readme.md)

## Housing

Flush-mount 3D-printable enclosure.

[housing/Readme.md](housing/Readme.md)

## ETS Configuration

Product database for KNX ETS configuration.

[software/Readme.md](software/Readme.md)

## License

MIT — see [LICENSE](LICENSE).
