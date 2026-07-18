# KNX KMP Watermeter

![Pipeline](https://github.com/embed-me/knx-kmp-watermeter/actions/workflows/pipeline.yml/badge.svg)

KNX TP1 interface for Kamstrup water/heat meters via the KMP (Kamstrup Meter Protocol). Reads meter data (volume, flow, temperature, battery life) and exposes it on the KNX bus.

## Project Structure

| Directory    | Description                                      |
|--------------|--------------------------------------------------|
| `firmware/`  | RP2040 firmware (PlatformIO, C++)                |
| `hardware/`  | PCB design (Altium)                              |
| `housing/`   | 3D-printable enclosure                           |
| `software/`  | KNX ETS product database source                  |

## Firmware

Custom firmware for the RP2040 (Raspberry Pi Pico) that implements the KMP protocol stack over UART to communicate with Kamstrup Multical 601/801 meters. Data is published via the KNX TP1 bus using the thelsing/knx library with an NCN5130-based BCU.

See [firmware/Readme.md](firmware/Readme.md) for build, flash, and test instructions.

## Hardware

Custom PCB combining an RP2040 MCU with NCN5130 BCU (NanoBCU-based) for KNX TP1 bus coupling. Includes a servo driver for meter wake-up and a UART transceiver for KMP communication.

[hardware/Readme.md](hardware/Readme.md)

## Housing

Flush-mount 3D-printable enclosure designed for standard installation boxes.

[housing/Readme.md](housing/Readme.md)

## ETS Configuration

KNX product database for ETS configuration. The device exposes four data points (volume, flow, temperature, battery life) with configurable polling intervals and per-register enable/disable.

[software/Readme.md](software/Readme.md)

## License

MIT — see [LICENSE](LICENSE).
