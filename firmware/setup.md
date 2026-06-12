# Setup USB over IP

## on raspberry

https://www.flexihub.com/usb-over-ip-raspberry-pi/

1. sudo modprobe usbip_host
2. sudo usbip list -p -l
3. sudo usbip bind -b <id>
4. sudo usbipd

## on windows (using usbip-win-0.3.6-dev)

https://github.com/cezanne/usbip-win

1. .\usbip.exe attach -r 10.0.20.28 -b 1-1
2. Forwaring to WSL using wsl USB manager
3. using pykmp to read the serial


read request:
pykmp-tool -vv -a 128 get-serial

80 # start
1B
7F
02 # get serial CID
3B # crc
DA 
0D # end



# Registers

0x0246 -> Battery live remaining in days
0x0124 -> Water temp in °C
0x0044 -> Volume in m^3
0x004A -> Flow in L/h


# Test output (deleteme)

T: PhysicalLayer: wrote 6 bytes
T: received byte 0x40

T: received byte 0x3F

T: received byte 0x02

T: received byte 0x01
T: received byte 0x9F
T: received byte 0x44
T: received byte 0x16
T: received byte 0x3F
T: received byte 0x4F

T: received byte 0x0D
I: Serial Number: 019F4416