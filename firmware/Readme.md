# 🛠️ UP Buzzer - Firmware

![Pipeline](https://github.com/embed-me/knx-up-buzzer/actions/workflows/pipeline.yml/badge.svg)

To ensure the application functions correctly, the product database must be imported into **ETS**, the physical address must be set, and the application must be written to the device.



## Building

The firmware is built using **PlatformIO**, which provides an easy setup for embedded development. While it is recommended to use **Visual Studio Code (VS Code)**, other IDEs can also be used.

### Steps to Build with VS Code

1. Open this directory in VS Code.
2. Trust the project when prompted.
3. VS Code will automatically install the required extensions for PlatformIO.
4. Build the firmware using the PlatformIO interface.



## Flashing

The **RP2040 UF2 ROM bootloader** is the recommended method for flashing the firmware due to its simplicity.

### Flashing Steps

1. Connect the board to your computer using the debugger port.
2. Press and hold the `Bootsel` button while pressing and releasing the `Reset` button.
3. A new USB drive will appear on your computer.
4. Drag and drop the file `.pio/build/release/firmware.uf2` onto the USB drive.



## Debugging

The easiest way to debug the firmware without using a dedicated debugger is through **printf debugging** via a serial console. 

### Enabling Serial Debugging

By default, logging output is disabled. To enable it:
1. Modify the logger initialization in `main.cpp` by setting the desired log level and ensuring logging starts when the serial connection is established:
   ```cpp
   logger->init(drivers::logger::LOGLEVEL::LOGLEVEL_TRACE, true);
   ```
2. Connect the debugger board to your computer.
3. Open PlatformIO's Serial Monitor.
4. Set the serial settings to 115200 baud rate, 1 stop bit, 8 data bits, no parity.
5. Observe the logging output for debugging information, including early boot messages.

## Extending functionality

The software configurations are organized into **modes**, which can be found in the [modes](src/drivers/knx/data/modes/) directory. During initialization, these modes are populated by reading the memory configuration from flash and are referenced within `MelodyConfig.hpp` via the `KnxConfig.hpp` file.

Once the configuration is loaded, it is applied to the corresponding **behavior**—defined in the [behaviours](src/application/behaviour/) directory—which determines the specific behavior for each mode. The correct behavior is automatically selected by the controller based on the user's configuration in **ETS**.

To implement a new behavior, it must be added to the existing set of behaviors. It's recommended to start by copying and modifying an existing mode to create your new behavior.


## Testing

Unit tests use GoogleTest and run on the native (host) platform via PlatformIO.

```
pio test -e test
```

Test suites cover the Kamstrup watermeter protocol stack:

| Suite | Tests | Covers |
|---|---|---|
| `test_units` | 11 | `unitToString()` unit code mapping |
| `test_crc` | 5 | `crc16_ccitt()` payload integrity |
| `test_bytestuff` | 16 | `stuff_bytes()` / `unstuff_bytes()` framing |
| `test_commands` | 25 | `GetRegisterCommand`, `GetSerialNumberCommand`, `GetClockCommand` |
| `test_transport` | 23 | `PhysicalLayer`, `DataLinkLayer`, `ApplicationLayer` |
| `test_scheduler` | 4 | `Scheduler` wrapper |
| `test_queue` | 1 | Placeholder — `CommandQueue` untestable on native due to Arduino timer coupling |

Mock headers for protocol interfaces live under `test/mocks/`.

Each test directory provides its own `test_main.cpp` because PlatformIO's native `googletest` integration does not auto-link `gtest_main`.
