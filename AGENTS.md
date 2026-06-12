# AGENTS.md — KNX UP-Buzzer

Compact guidance for AI sessions working in this repo.

## Repo layout

- `firmware/` — PlatformIO C++ application for RP2040 (Raspberry Pi Pico). This is where almost all code changes happen.
- `hardware/` — Altium Designer PCB project and production package (Gerbers).
- `housing/` — 3D-printable STL files and CAD models.
- `software/` — KNX ETS product database source (`up-buzzer.ae-manu`) and generated `knxprod.h`.

There are no unit tests, no CI, and no pre-commit hooks.

## Firmware build & flash

- Build system: **PlatformIO** (`firmware/platformio.ini`).
- Default environment is `release`.
- **Build**: `cd firmware && pio run` (or use VS Code + PlatformIO extension).
- **Flash artifact**: `.pio/build/release/firmware.uf2`. Flash by holding `Bootsel`, pressing `Reset`, then copying the UF2 to the USB mass-storage device that appears.
- `platformio.ini` uses a **custom platform fork** and a **framework zip from GitHub releases**, not the PlatformIO registry:
  - platform: `https://github.com/maxgerhardt/platform-raspberrypi.git#60d6ae8`
  - framework: `https://github.com/earlephilhower/arduino-pico/releases/download/3.9.3/rp2040-3.9.3.zip`

### Important `platformio.ini` quirks

- `src_dir = .` — the project root is `firmware/`, **not** `firmware/src/`. Source files are referenced with paths like `src/application/...` because the compiler sees `firmware/` as the root.
- **Logging is disabled in release** (`-DKNX_UP_BUZZER_DISABLE_LOGGING`) because `ArduinoLog` has a bug where it does not call `va_end()`.
- To enable serial logging, uncomment the `logger->init(...)` lines in `firmware/src/main.cpp` and rebuild. Serial monitor is 115200/8/N/1.
- `cppcheck` is configured; it checks `src/application/`, `src/drivers/`, `src/utils/`, and `src/main.cpp`.

## Generated code: `knxprod.h`

- `firmware/src/drivers/knx/data/knxprod.h` is **generated** by Kaenx-Creator from `software/up-buzzer.ae-manu`.
- Do **not** hand-edit `knxprod.h`; changes will be lost when the product database is regenerated.
- To regenerate: open `software/up-buzzer.ae-manu` in Kaenx-Creator (recommended version **v1.8.4**), edit, bump the version, and re-deploy. Copy the resulting `knxprod.h` into `firmware/src/drivers/knx/data/`.

## Adding a new melody mode / behaviour

The firmware supports three modes today: `Trigger`, `Switch`, and `VentingMonitor`.

To add a new mode, you must create **two** pieces and wire them in:

1. **Mode** — a class in `firmware/src/drivers/knx/data/modes/` that captures the ETS-configured parameters (see `TriggerMode`, `SwitchMode`, `VentingMonitorMode`).
2. **Behaviour** — a class in `firmware/src/application/behaviour/` that implements the runtime logic (see `TriggerBehaviour`, `SwitchBehaviour`, `VentingMonitorBehaviour`).
3. Register both in:
   - `BehaviourFactory::getBehaviour()` (creates the correct behaviour for the mode)
   - `KnxConfig::getMelodyConfigs()` (reads ETS parameters and constructs the mode)

## Architecture notes

- `main.cpp` wires everything together via `ArduinoDriverFactory` (drivers) and `MelodyController` (application).
- `MelodyController` is a resource granter: behaviours request the buzzer, and the current `IPriority` strategy decides who gets it. Today the strategy is `NoPriority`.
- A simple scheduler (`utils/Scheduler`) is used for deferred work; the main `loop()` just feeds the watchdog and processes the scheduler.
- Core 1 (`setup1` / `loop1`) is empty.

## Important constraints

- Hardware features beyond the buzzer (proximity sensor, binary inputs) are **present in the schematic but untested**.
- No automated test suite exists; verify by building and manual flashing.
- The KNX stack is `thelsing/knx` with `Bau07B0` (TP1 twisted-pair) and `RP2040ArduinoPlatform`.
