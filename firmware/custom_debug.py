import os

Import("env")

platform = env.PioPlatform()

# Prefer the PlatformIO xPack package (includes ftdi support)
openocd_package = platform.get_package_dir("tool-openocd")
if openocd_package:
    openocd_path = os.path.join(openocd_package, "bin", "openocd")
else:
    print("ERROR: tool-openocd not installed. Run: pio pkg install -g --tool platformio/tool-openocd@^3.1200.0")
    env.Exit(1)

openocd_path = openocd_path.replace("\\", "/")

# Build the firmware path directly to avoid $SOURCE backslash mangling
firmware_path = env.subst("${BUILD_DIR}/firmware.bin").replace("\\", "/")

# Upload command string must quote arguments containing spaces
# so the shell passes them as single tokens to OpenOCD.
openocd_flags = (
    '-f interface/ftdi/jtag-lock-pick_tiny_2.cfg '
    '-c "set USE_CORE 0" '
    '-c "adapter speed 1000" '
    '-c "adapter srst delay 200" '
    '-f target/rp2040.cfg '
    '-c "program ' + firmware_path + ' 0x10000000 verify reset exit"'
)

env.Replace(
    UPLOADCMD='"' + openocd_path + '" ' + openocd_flags
)

print("Using OpenOCD:", openocd_path)
print("Firmware:", firmware_path)
