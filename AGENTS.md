# AGENTS.md — KNX KMP Watermeter

Compact guidance for AI sessions working in this repo.

## Available skills

`.opencode/skills/embedded-tdd/skill.md` — Red-Green-Refactor TDD cycle for embedded C++ with GoogleTest, mocks, and stubs.
`.opencode/skills/embedded-cpp-patterns/skill.md` — SOLID, DRY, and embedded design patterns used throughout this codebase.

## Repo layout

- `firmware/` — PlatformIO C++ application for RP2040 (Raspberry Pi Pico). This is where almost all code changes happen.
- `hardware/` — Altium Designer PCB project and production package (Gerbers).
- `housing/` — 3D-printable STL files and CAD models.
- `software/` — KNX ETS product database source (`up-buzzer.ae-manu`) and generated `knxprod.h`.

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
- **Logging is disabled in release** (`-DKNX_KMP_WATERMETER_DISABLE_LOGGING`) because `ArduinoLog` has a bug where it does not call `va_end()`.
- To enable serial logging, uncomment the `logger->init(...)` lines in `firmware/src/main.cpp` and rebuild. Serial monitor is 115200/8/N/1.
- `cppcheck` is configured; it checks `src/application/`, `src/drivers/`, `src/utils/`, and `src/main.cpp`.

## Unit tests

- **Framework**: GoogleTest, runs natively (not on Pico).
- **Run**: `cd firmware && pio test -e test`
- **Test suites** in `firmware/test/test_*/`
- **Mocks** in `firmware/test/mocks/` — hand-rolled classes implementing driver interfaces
- **Stubs** in `firmware/test/stubs/` — platform headers for host compilation

## Generated code: `knxprod.h`

- `firmware/src/drivers/knx/data/knxprod.h` is **generated** by Kaenx-Creator from `software/up-buzzer.ae-manu`.
- Do **not** hand-edit `knxprod.h`; changes will be lost when the product database is regenerated.
- To regenerate: open `software/up-buzzer.ae-manu` in Kaenx-Creator (recommended version **v1.8.4**), edit, bump the version, and re-deploy. Copy the resulting `knxprod.h` into `firmware/src/drivers/knx/data/`.

## Architecture notes

- `main.cpp` wires everything together via `ArduinoDriverFactory` (drivers) and `WatermeterApp` (application).
- `WatermeterApp` is the application orchestrator: it sets up the KMP transport stack (`PhysicalLayer` → `DataLinkLayer` → `ApplicationLayer`), manages `CommandQueue` for sequential KMP commands, and uses timers for keep-alive and data polling.
- `WatermeterWakeupDriver` actuates a servo to physically wake the Kamstrup meter before communication.
- A simple scheduler (`utils/Scheduler`) is used for deferred work; the main `loop()` feeds the watchdog, processes the scheduler, and runs the KNX stack.
- Core 1 (`setup1` / `loop1`) is empty.

## Important constraints

- Hardware features beyond the buzzer (proximity sensor, binary inputs) are **present in the schematic but untested**.
- The KNX stack is `thelsing/knx` with `Bau07B0` (TP1 twisted-pair) and `RP2040ArduinoPlatform`.
