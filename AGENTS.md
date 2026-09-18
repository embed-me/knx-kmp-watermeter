# AGENTS.md — KNX KMP Watermeter

Compact guidance for AI sessions working in this repo.

## Available skills

`.opencode/skills/embedded-tdd/skill.md` — Red-Green-Refactor TDD cycle for embedded C++ with GoogleTest, mocks, and stubs.
`.opencode/skills/embedded-cpp-patterns/skill.md` — SOLID, DRY, and embedded design patterns used throughout this codebase.

## Repo layout

- `firmware/` — PlatformIO C++ application for RP2040 (Raspberry Pi Pico). This is where almost all code changes happen.
- `hardware/` — Altium Designer PCB project and production package (Gerbers).
- `housing/` — 3D-printable STL files and CAD models.
- `software/` — KNX ETS product database source (`kmp-watermeter.ae-manu`) and generated `knxprod.h`.

Build, flash, and test instructions live in [firmware/Readme.md](firmware/Readme.md).

### `platformio.ini` quirks (agent reference)

- `src_dir = .` — the project root is `firmware/`, **not** `firmware/src/`. Source files are referenced with paths like `src/application/...` because the compiler sees `firmware/` as the root.
- **Logging is disabled in release** (`-DKNX_KMP_WATERMETER_DISABLE_LOGGING`) — `ArduinoLog` has a bug where it does not call `va_end()`. To enable, uncomment `logger->init(...)` in `firmware/src/main.cpp`. Serial: 115200/8/N/1.
- `cppcheck` checks `src/application/`, `src/drivers/`, `src/utils/`, `src/main.cpp`.

## Generated code: `knxprod.h`

- `firmware/src/drivers/knx/data/knxprod.h` is **generated** by Kaenx-Creator from `software/kmp-watermeter.ae-manu`. **Do not hand-edit.**
- To regenerate: open `software/kmp-watermeter.ae-manu` in Kaenx-Creator (v1.8.4), edit, bump version, re-deploy. Copy resulting `knxprod.h` to `firmware/src/drivers/knx/data/`.

## Important constraints

- The KNX stack is `thelsing/knx` with `Bau07B0` (TP1 twisted-pair) and `RP2040ArduinoPlatform`.
- Hardware features beyond the buzzer (proximity sensor, binary inputs) are in the schematic but untested.
