# Hardware — KMP Watermeter PCB

PCB combining an RP2040 MCU (Raspberry Pi Pico) with an NCN5130 BCU (NanoBCU-based) for KNX TP1 bus coupling. It uses a servo driver for water meter wake-up, driving a NEEBRC 2g Micro Servo.

The board is designed as a 2-layer PCB in Altium Designer.

The NanoBCU and the UP board are available from [muster.ing-dom.de](https://muster.ing-dom.de/).

## Project Structure

| Directory | Contents |
|-----------|----------|
| [project/](project/) | Altium Designer project: schematic (3 sheets), PCB, BOM, and custom component libraries |
| [production_package/](production_package/) | Ready-to-order Gerber files and drill data |
| [Design Rules/](Design%20Rules/) | JLCPCB design rule files (2-layer, 1 oz / 2 oz copper) |

The schematic is split into three sheets: the main sheet (MCU sockets, servo header, power supply), plus dedicated [Transmitter](project/Transmitter.SchDoc) and [Receiver](project/Receiver.SchDoc) sheets for the IR optical transceiver used for KMP communication.

## Production Package

Ready-to-order PCB files are available in the [production package](production_package/).
