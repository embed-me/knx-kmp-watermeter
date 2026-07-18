# Embedded C++ Patterns (SOLID & DRY)

Guidelines for embedded firmware design applied in this repo.

## SOLID in practice

| Principle | Application in this repo |
|---|---|
| **S**ingle Responsibility | Each class does one thing: `CommandQueue` queues commands, `CRC` computes checksums, `PhysicalLayer` handles UART framing |
| **O**pen/Closed | Interfaces (`IKnxDriver`, `IGpioDriver`, `ITimer`) are open for extension via new implementations, closed for modification |
| **L**iskov Substitution | All interface implementations are substitutable — `ArduinoTimer` can be swapped with `MockTimer` in tests |
| **I**nterface Segregation | `IDriverFactory` is composed of small segregated interfaces (`IGpioDriverFactory`, `IKnxDriverFactory`, etc.) |
| **D**ependency Inversion | High-level code (`WatermeterApp`) depends on abstractions (`IApplicationLayer`, `ITimer`), not concretions |

## OOP Principles

| Principle | Application |
|---|---|
| **Encapsulation** | Each driver interface (`IGpioDriver`, `ITimer`) hides platform internals behind a clean API. State and config are private; only the interface is exposed. |
| **Polymorphism** | Interfaces enable multiple implementations — `ArduinoTimer` (real hardware) and `MockTimer` (test) both implement `ITimer`. |
| **Inheritance** | Class hierarchies follow interface contracts. `ArduinoGpioDriver : IGpioDriver`, `ArduinoLogger : ILogger`. Interfaces are pure virtual with `= 0`. |
| **Composition > Inheritance** | `WatermeterApp` *has-a* `CommandQueue`, *has-a* `ITimer`, *has-a* `IApplicationLayer`. Favor composing small interfaces over deep class hierarchies. |

## DRY — Don't Repeat Yourself

- Factor hardware platform code behind interfaces in `firmware/src/drivers/`
- Reuse `CommandQueue`, `CRC`, `ByteStuff` across commands and layers
- If you write the same logic twice, extract it into `firmware/src/utils/`

## Embedded-specific patterns

| Pattern | Where | Why |
|---|---|---|
| **Interface + Impl** | `drivers/*/I*.hpp` + `arduino/*.cpp` | Decouple from Arduino/Pico SDK; enables native testing |
| **Dependency Injection** | Constructor args (e.g. `WatermeterWakeupDriver(motion, cfg, timer)`) | Testable without hardware |
| **RAII** | `shared_ptr` ownership throughout | Automatic cleanup on resource-constrained MCU |
| **Factory** | `ArduinoDriverFactory`, `ArduinoTimerFactory` | Centralized creation of platform-specific objects |
| **Strategy** | `ISchedulerStrategy` / `SimpleScheduler` | Pluggable scheduling behavior |
| **Layered Protocol** | `PhysicalLayer` → `DataLinkLayer` → `ApplicationLayer` | OSI-inspired separation of concerns |
| **Observer** | Listener callbacks in transport layers | Decouple senders from receivers |

## Code style rules

1. Pure virtual interfaces get `I` prefix: `ITimer`, `IGpioDriver`
2. Config structs are plain `struct`s with aggregate init
3. No exceptions (RTTI only for KNX stack); errors flow through `CommandResult`
4. Avoid dynamic allocation in hot paths; pre-allocate in `setup()`
5. Logging is disabled in release (`-DKNX_KMP_WATERMETER_DISABLE_LOGGING`)
