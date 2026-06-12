File: 5512-447

First edition: 2006-11-23

KMP
Kamstrup Heat Meter Protocol

MULTICAL(cid:163)601 and  MULTICAL®801

Protocol description

The Kamstrup Meter Protocol (KMP) description describes the internal protocols used in e.g.
MULTICAL® 601 and 801, Heat and Cooling meters. The KMP description contains information
necessary to connect MULTICAL® to a third-party system, by serial data communication.

While the KMP description is believed to be accurate at the time of issue, Kamstrup A/S does not
guarantee the accuracy of the document, nor will it be held responsible for damages of any kind,
direct or indirect, which may result from the use of the KMP description. Further, you understand
that the protocols described in the KMP description are subject to change without notice.

The KMP Protocol description is provided free of charge from Kamstrup A/S. It is, however, copy-
righted and must not be copied. Please feel free to order your personal copy from Kamstrup A/S.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:
sl

SL

Digitally signed by sl
DN: cn=sl, o, ou,
email=sl@kamstrup.
dk, c=DK
Date: 2009.12.08
12:10:39 +01'00'

EDB:
GHA

GHA

Digitalt signeret af GHA
DN: cn=GHA,
o=Kamstrup, ou,
email=GHA@kamstrup.
dk, c=DK
Dato: 2009.12.08
11:53:12 +01'00'

Doc. No.:
5512-447

Rev.: M1
Page:  1

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

1 Introduction

This protocol is designed to handle point-to-point communication in a master / slave bus system. The main
application for this protocol is data readout of Kamstrup heat meters, MULTICAL(cid:163) 601 and MULTICAL(cid:163) 801.

The data reliability is improved by introducing 16 bit CRC (Cyclic Redundancy Check).

Data read out is made more flexible by introducing specific register read out. Registers will be represented by
at register identification value. A register read out is associated with unit and exponent.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  2

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

2 Communication format

The basic KMP communication is based on a master/slave bus communication (see 3.2 for addressing), where
the meter (and most ”feature” modules) is the slave, and any meter communication hardware is the master.

An example with a Kamstrup radio module as the master:

Basic KMP communication is always initiated from the master by a data request frame.
The meter can then respond either by a data response frame or a single application acknowledge character
(ACK = 06h).

Example:

In addition to this, the meter can in some cases send a single character (= 00h) without being requested. This
should be ignored.

2.1

MULTICAL timing sequence

The MULTICAL® (MC601 and MC801) works with a communication timeout of 1.6 second.

The meter gets active when it receives the first byte of a request frame. The timeout in between bytes in a
request frame is 27 milliseconds.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  3

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

The time, from the last byte in the request is send to the meter, to the first byte in the response is received
from the meter, is less than 1.6 second (see [1] in the figure below).
The meter then stays active in 1.6 second from the first byte of the response is send (independently of the
response communication time) before going to sleep (see [2] in the figure below).

To optimize the communication time any following request has to be send less than 1.6 second after the first
byte in the last response (see [3] in the figure below) to keep the meter from going “to sleep”. The time
between the last byte in the last response is received to the first byte in the new request is send should be at
least 1 ‘byte time’ ((cid:124) 9 milliseconds at 1200 baud) though (see [4] in the figure below).

[3]

[1]

[4]

[2]

If the meter receives data, but the data is invalid or the meter is to busy, the meter stays active for 1.6 second
after the last byte is received (see [1] in the figure below), in which any following request is discharge.

If the meter has not started the response frame after 1.6 second (or a response is received but the CRC is
wrong – see [2] in the figure below). Then the transmission of the first byte of any following request frames
should wait at least 1.6 second (see [3] in the figure below) for the meter to be ready for a new request.

[3]

[2]

[1]

2.2

Communication via 6705 top module

The 6705 top module gives a 2nd priority access to MULTICAL(cid:163) 601.
It supports the same KMP communication and uses the same destination address as MULTICAL(cid:163) 601.
However, the supported application commands are limited to 01h, 02h and 10h.
To distinguish a connection through 6705 top module from one directly at a MULTICAL(cid:163) 601, the meter type
returned by application command “GetType” is different (see chapter 6.2.1).

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  4

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

3 Data frame format

The data frame is based on the OSI model. In this protocol, only three layers of the OSI model are used. These
are the physical layer, data link and the application layer. The figure below show how each of the layers
(shown as shaded) is related to the data frame.

Field name
Number of bytes in each field
OSI – layer

Start byte  Destination address  CID  Data
1

1

1

0-?

CRC  Stop byte

2

1

Application layer

Data link layer
Physical layer

In some communication examples only the APL is shown (CID and Data).

3.1

Physical layer

The protocol is based on half duplex serial asynchrony communication with the set-up: 8 data bit, no parity bit
and 2 stop bits. Standard data bit rate is 1200 baud for both transmitted and received data.

Data is transmitted byte wise in a binary data format. 8 data bit represent one byte of data.

The physical layer in the data frame is used to synchronize and terminate data transmission. For this purpose
the physical layer uses a unique start and stop byte.

Different start bytes values are used in frames to and from the meter. This is done to make an easy
identification of the frame start and frame direction in communication where TXD is ‘echoed’ to the RXD line –
e.g. at communication via the IR readout head.

Start byte in a data frame to the meter = 80h
Start byte in a data frame from the meter = 40h

Stop byte = 0Dh

Note that the KMP protocol also supports an ACK byte (0x06h)used some commands as a reply for
successful execution. The ACK is an APL level acknowledge, but send as a single byte – Without start,
CRC or stop bytes.

The physical layer uses ’Byte stuffing’ to compensate for byte values reserved as start, stop and acknowledge.
The method is to substitute the reserved bytes values with a pair of byte values.
In this protocol the substitution shown below is used:

80h
40h
0Dh
06h
1Bh

(cid:111)
(cid:111)
(cid:111)
(cid:111)
(cid:111)

1Bh, 7Fh
1Bh, BFh
1Bh, F2h
1Bh, F9h
1Bh, E4h

=
=
=
=
=

1Bh, NOT(80h)
1Bh, NOT(40h)
1Bh, NOT(0Dh)
1Bh, NOT(06h)
1Bh, NOT(1Bh)

Note that byte stuffing can increase the length of the physical layer by 100% compared to the data link layer.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  5

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Unstuffing data is done by removing the byte ‘1Bh’ and the following byte and make a substitution according
to the table below:

1Bh, 7Fh  (cid:111)
1Bh, BFh  (cid:111)
1Bh, F2h   (cid:111)
1Bh, F9h  (cid:111)
1Bh, E4h  (cid:111)

80h
40h
0Dh
06h
1Bh

=  NOT(7Fh)
=  NOT(BFh)
=  NOT(F2h)
=  NOT(F9h)
=  NOT(E4h)

Example:

Data:

04  0D  00  06

Stuffed data:

04  1B  F2  00  1B  F9

Unstuffed data:

04  0D  00  06

3.2

Data link layer

The destination address is included in order to prepare a future enhanced version of the protocol.
For heat meters the destination address is 3Fh. The logger top module use 7Fh and the logger base module
use BFh.

Included in the data link layer is a CRC with reference to the CCITT-standard using the polynomial 1021h.
Only deviation from the standard is the initial value, which is 0000h instead of FFFFh.

The CRC result is calculated for destination address, CID and data. CRC is transmitted with MSByte first and
LSByte last.

3.3

Application layer

The first byte of the application layer is the Command ID (CID). This also applies for a response data frame
from the meter. The response CID will then equal the request CID.
The number of bytes in the data field (if any) depends on the CID. The interpretation of the bytes in the data
field also depends on the CID.

See chapter 6 for more details on CIDs.

Most data in the meter is handled in a KMP register format (see chapter 4.4).

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  6

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

4 Data formats

4.1

Basis formats

[] Specifies one byte data.

() Specifies a set of bytes – e.g. an integer.

The value specified by () is of the length 2, 3, 4 … bytes.
The order of most and least significant bytes is independent of the number of bytes. Most significant
byte is transmitted first.

Examples:

(data) = [data 1][data 2][data 3]

[data 1] is the most significant byte, [data 3] is the least significant byte.

4.2

Floating point data format

This data format applies to transmission of data registers.

One value consists of the following bytes:

[number of bytes][sign+exponent] (integer)

(integer) is the register data value. The length of the integer value is specified by [number of bytes].

[sign+exponent] is an 8-bit value that specifies the sign of the data value and sign and value of the exponent.
The meaning of the individual bits in the [sign+exponent] byte is shown below:

Sign + exponent
Bit 7
SI

6
SE

5
E5

4
E4

3
E3

2
E2

1
E1

0
E0

Floating

poi

nt

value

(cid:16)(cid:32)

1

SI

(cid:152)

(
i

nteger

SE

110)
(cid:16)
(cid:152)

e
(cid:152)

xponent

Examples:

-123.45 = 04h, C2h, 0h, 0h, 30h, 39h

87654321*103  = 04h, 03h , 05h, 39h, 7Fh, B1h

255*103  = 01h, 03h , FFh

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  7

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

4.3

Logger timestamp data format

(Time) is a 64bits/8bytes integer. It contains a timestamp in the following format:

(Time) = ([YY][MM][DD][hh][mm][ss][WK][Info])

With the following definitions:

Byte  Parameter  Range(dec)  Definition

0
1
2
3
4
5
6
7

[YY]
[MM]
[DD]
[hh]
[mm]
[ss]
[WK]
[Info]

0-99
1-12

Year –Year 2000: [YY] = 0d,  Year 2099: [YY] = 99d
Month

1-(28)31  Day

0-23
0-59
0-59
0
0

Hours
Minutes
Seconds
Weekday – not used
Info byte – not used

Examples:

2007-05-17 10:30:00  (cid:156)
2091-12-31 23:59:59  (cid:156)

07|05|11|0A|1E|00|00|00
5B|0C|1F|17|3B|3B|00|00

4.4

KMP register data format

Most of the data in the meter is implemented as registers. A register description consists of three parts shown
below and explained in the following subchapters.

Register ID  Register format
4
2

0

1

3
xxxxxx

xxxx

Register value

5

6
7
xxxxxxxx

8

4.4.1

Register ID (RID)

Meter registers are associated with unique register identification numbers (RID). The RID is a 16-bit value.
It is used in commands for reading actual and historical data.

Examples:

Register ID
0001h  =
0048h  =
03E9h  =  1001d

1d

Register
Energy A14

72d  Mass1

Serial number

For a list of RIDs supported in MC601 see chapter 5.2.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  8

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

4.4.2

Register format

The register format consists of three bytes; Unit, NumberOfBytes(NoB) and SignExp(SiEx):

Unit  NoB  SiEx
1
xx

0
xx

2
xx

These parameters are defined as the following:

Parameter
Unit

NoB

SiEx

Description
8bit – Register unit
Specifies the unit of the register data value.
Example:

3d  =  MWh

40d  =  m3
47d  =  clock (hh:mm:ss)
See chapter 5.1 for more units.
8bit – Number Of Bytes (n)
Specifies the byte size of the register data value.
This parameter is always 4 in MC601 and MC801.
8bit – Sign and Exponent
See ‘sign+exponent’ in chapter 4.2.

4.4.3

Register value

The register value is defined as a 4 byte integer as descript in chapter 4.1.

4.4.4

Examples

-123.45 kCal =>

Unit  NoB  SiEx
1
04

0
0A

2
C2

87654321*103 l =>
Unit  NoB  SiEx
1
04

2
03

0
27

3

3

Value
5
4

00003039

Value
5
4

05397FB1

6

6

255*103 m3 =>

Unit  NoB  SiEx  Value

0
28

1
01

2
03

3
FF

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  9

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

5 Units and ID’s

5.1

RegisterUnit

Byte, which describes the measuring units:

Unit ID

Unit name

hex
01
02
03
08
0C
16
17
25
26
27
28
29
2A
2B
2C
2D
2E
2F

dec
1  Wh
2
kWh
3  MWh
Gj
8
Gcal
12
22
kW
23  MW
37
38
39
40  m3
41
l/h
42  m3/h
43  m3xC
44
45
46
47

ton
ton/h
h
clock

C
K
l

30

48

date1

Description

hh:mm:ss (dec)
Examples:

10:30:00  (cid:156)  00|01|92|58
23:59:59  (cid:156)  00|03|99|B7

yy:mm:dd (dec)
Examples:

06:08:24  (cid:156)  00|00|ED|98
99:12:31  (cid:156)  00|0F|1F|FF

32

50

date3

mm:dd (dec)
Examples:

08:24
12:31

(cid:156)  00|00|03|38
(cid:156)  00|00|04|CF

33
34

51
52

number
bar

5.2

MULTICAL(cid:163) 601 Register ID’s

Register ID

Register name

dec
1003  DATE

hex
03EB
003C
005E
003F
003D
003E
005F
0060
0061
006E
0040
0041
0044

60
94
63
61
62
95
96
97
110
64
65
68

E1
E2
E3
E4
E5
E6
E7
E8
E9
TA2
TA3
V1

Description

Current date (YYMMDD)
Energy register 1: Heat energy
Energy register 2: Control energy
Energy register 3: Cooling energy
Energy register 4: Flow energy
Energy register 5: Return flow energy
Energy register 6: Tap water energy
Energy register 7: Heat energy Y
Energy register 8: (cid:62)m3 (cid:120) T1(cid:64)
Energy register 9: (cid:62)m3 (cid:120) T2(cid:64)
Tariff register 2
Tariff register 3
Volume register V1

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  10

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Register ID

Register name

Description

hex
0045
0054
0055
0048
0049
03EC
0071
03EA
0063
0056
0057
0058
007A
0059
005B
005C
004A
004B
0050
007B
007C
007D
007E
007F
0080
0081
0082
008A
008B
008C
008D
008E
008F
0090
0091
0092
0093
0095
0096
0042
0043
0062
0098
0099
00A8
03E9
0070
03F2
0072
0068
03ED
009A
009B
009D
009E
00AF

dec
69
84
85
72
73

V2
VA
VB
M1
M2
1004  HR
113
1002  CLOCK

INFOEVENT

Volume register V2
Input register VA
Input register VB
Mass register V1
Mass register V2
Operational hour counter
Info-event counter
Current time (hhmmss)
Infocode register, current
Current flow temperature
Current return flow temperature
Current temperature T3
Current temperature T4
Current temperature difference
Pressure in flow
Pressure in return flow
Current flow in flow
Current flow in return flow
Current power calculated on the basis of V1-T1-T2
Date for max. this year
Max. value this year
Date for min. this year
Min. value this year
Date for max. this year
Max. value this year
Date for min. this  year
Min. value this year
Date for max. this year
Max. value this year
Date for min. this month
Min. value this month

INFO
T1
T2
T3
T4
T1-T2
P1
P2
FLOW1
FLOW2
EFFEKT1

99
86
87
88
122
89
91
92
74
75
80
123  MAX FLOW1DATE/ÅR
124  MAX FLOW1/ÅR
125  MIN FLOW1DATE/ÅR
126  MIN FLOW1/ÅR
127  MAX EFFEKT1DATE/ÅR
128  MAX EFFEKT1/ÅR
129  MIN EFFEKT1DATE/ÅR
130  MIN EFFEKT1/ÅR
138  MAX FLOW1DATE/MÅNED
139  MAX FLOW1/MÅNED
140  MIN FLOW1DATE/MÅNED
141  MIN FLOW1/MÅNED
142  MAX EFFEKT1DATE/MÅNED  Date for max. this month
143  MAX EFFEKT1/MÅNED
144  MIN EFFEKT1DATE/MÅNED
145  MIN EFFEKT1/MÅNED
146
147
149
150
66
67
98
152
153
168
1001  SERIE NO
112  METER NO 2
1010  METER NO 1
114  METER NO VA
104  METER NO VB
1005  METER TYPE
154
155
157
158
175

AVR T1/ÅR
AVR T2/ÅR
AVR T1/MÅNED
AVR T2/MÅNED
TL2
TL3
XDAY
PROG NO
CONFIG NO 1
CONFIG NO 2

CHECK SUM 1
HIGH RES
TOPMODUL ID
BOTMODUL ID
ERRORHOURCOUNTER

Max. value this month
Date for min. this month
Min. value this month
Year-to-date average for T1
Year-to-date average for T2
Month-to-date average for T1
Month-to-date average for T2
Tariff limit 2
Tariff limit 3
Target date (reading date)
Program no. ABCCCCCC
Config no. DDDEE
Config no. FFGGMN
Serial no. (unique number for each meter)
Customer number (8 most important digits)
Customer number (8 less important digits)
Meter no. for VA
Meter no. for VB
Software edition
Software check sum
High-resolution energy register for testing purposes
ID number for top module  ( only mc 601 )
ID number for base module
Error hour counter

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  11

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Register ID

hex
00EA
00EB

dec
234
235

Register name

Description

INA LITERIMP
INB LITERIMP

Liter/imp value for input A
Liter/imp value for input B

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  12

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

5.3

MULTICAL(cid:163) 801 Register ID’s

Additional to the register Id’s listed above,  MULTICAL(cid:163) 801 include the following registers.

hex

BOTMODULE ID2
EXTMODULE ID

dec
171
172
184  MbusPriAdrMod1
185  MbusSekAdrMod1
218  MbusPriAdrMod2
219  MbusSekAdrMod2
220  MbusPriAdrExtMod
221  MbusSekAdrExtMod
FEACPU ID
157

ID number for base module 2
ID number for external base module
Primary Mbus adress for module 1
Sekundary Mbus adress for module 1
Primary Mbus adress for module 2
Sekundary Mbus adress for module 2
Primary Mbus adress for external module
Sekundary Mbus adress for external module
Feature cpu ID ( dummy topmodule id )

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  13

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6 General application commands

6.1

Command overview

The following general commands are specified for the MULTICAL:

CID
01h
02h
09h *
10h

Command
GetType
GetSerialNo
SetClock
GetRegister

* Not accessible through 67-05 top module.

The above and the following commands are specified for the MC601 logger modules (67-0B top module and
67-00-22 base module) and for MC801:

CID
A0h
A1h
A2h
A3h
9Bh
9Ch
11h

Command
GetLogTimePresent
GetLogLastPresent
GetLogIDPresent
GetLogTimePast
GetEventStatus
ClearEventStatus
PutRegister

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  14

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.2

Description of general application commands

6.2.1

GetType   -

CID = 01h

Function:

This command returns identification of the type of meter and software revision.

The document ‘KPM Product Identification’ lists meter types and the corresponding identification
values.

Request:

[80h] [destination address] [01h] (CRC) [0Dh]

Response:

[40h] [destination address] [01h] (MT) (SW revision) (CRC) [0Dh]

The meter type (MT) is a 16-bit value.

Meter type:

0001h = Multical 601 – pt500
0004h = Multical 601 – pt100
0005h = Multical 601 – pt500, 4-wire
1501h = Multical 601 – pt500, through 67-05 top module
1504h = Multical 601 – pt100, through 67-05 top module
1505h = Multical 601 – pt500, 4-wire, through 67-05 top module
09xx = A general top module (use dest addr 0x7F)
16xx = A general base module (use dest addr 0xBF)

The software revision is a 16 bit value:

 (SW revision) = [data1] [data 2]

[data 1] identify revision letter:   01h = A
02h = B
03h = C
04h = D
05h = E
…

[data 2] identify revision number: 00h = 0
01h = 1
02h = 2
03h = 3
04h = 4
…

Example

Request:

Application Layer:

Data Link Layer:

01

3F  01

CID

  Destination address added

3F  01  05  8A

CRC added (No byte stuffing needed)

Physical Layer:

80  3F  01  05  8A  0D

Start and stop byte added

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  15

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Response:

Physical Layer:

Data Link Layer:

40  3F  01  00  04  1B

F9  01  26  99  0D

3F  01  00  04  1B

F9  01  26  99

Start and stop byte removed

3F  01  00  04  06  01  26  99

  Destuffing

3F  01  00  04  06  01  26  99  00  00

CRC calculated

Application Layer:

3F  01  00  04  06  01

  New and old CRC removed

01  00  04  06  01

0601h = SW revision F1

0004h = Multical 601 – pt100

01h = GetType

  Destination address removed

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  16

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.2.2

GetSerialNo   -  CID = 02h

Function:

This command returns the serial number of the meter.
The serial number is a 32-bit number. Every heat meter has a unique serial number and these
meters will not have identical serial numbers.
This number is used as an unambiguous identification of the meter.

Request:

[80h] [destination address] [02h] (CRC) [0Dh]

Response:

[40h] [destination address] [02h] (SN) (CRC) [0Dh]

The serial number (S/N) is a 32-bit value.

Example

Request:

Application Layer:

Data Link Layer:

02

3F  02

CID

  Destination address added

Physical Layer:

Response:

Physical Layer:

Data Link Layer:

3F  02  35  E9

CRC added (No byte stuffing needed)

80  3F  02  35  E9  0D

Start and stop byte added

40  3F  02  01  23  45  67  E9  56  0D

3F  02  01  23  45  67  E9  56

Start and stop byte removed

3F  02  01  23  45  67  E9  56  00  00

CRC calculated (No destuffing needed)

3F  02  01  23  45  67

  New and old CRC removed

Application Layer:

02  01  23  45  67

01234567h = S/N: 19088743d

01h = GetSerialNo

  Destination address removed

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  17

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.2.3

SetClock  -

CID = 09h

Function:

This command sets the meter clock.
(Not accessible through 67-05 top module.)

Request:

[80h] [destination address] [09h] (date) (time) (CRC) [0Dh]

The (date) is a 32 bit binary value formatted as YY-MM-DD.

Example: The date 25th august 2004 results in  (date) = 40825d

The (time) is a 32 bit binary value formatted as HH:MM:SS.

Example: The time 23:59:59 results in  (time) = 235959d
Example: The time 00:00:00 results in  (time) = 0d

Validation demands:

YY:
MM:
DD:

HH:
MM:
SS:

0 – 99d
1 – 12d
1 – X

0 – 23d
0 – 59d
0 – 59d

X depends on month and leap year

Response:

An application acknowledge is returned if (date) and (time) are valid.

Example

Request:

Application Layer:

Time: 16:15:57 = 0x027715h

  Date: 2007-07-30 =  0x01144Ah

09  00  01  14  4A  00  02  77  15

  CID and data

3F  09  00  01  14  4A  00  02  77  15

  Destination address added

3F  09  00  01  14  4A  00  02  77  15  C3  4A

80  3F  09  00  01  14  4A  00  02  77  15  C3  4A  0D

  CRC added (No byte
  stuffing needed)
  Start and stop byte added

Data Link Layer:

Physical Layer:

Response:

Physical Layer:

06

Acknowledge (No start/stop, CRC, destination address or CID)

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  18

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.2.4

GetRegister  -  CID = 10h

Function:

This command returns a variable set of registers.

Request:

[80h] [destination address] …
[10h] [number of registers] (register 1 ID) (register 2 ID) ...(register n ID) …
(CRC) [0Dh]

[number of registers] specifies the number of registers to be returned in the response and thus the
number of following register codes.

A maximal number of 8 registers can be read with one request.
Thus [number of registers] = 1 to 8.

(register 1 ID) … (register n ID) are 16 bit register identification codes and specifies registers to be
read out.

Response:

[40h] [destination address] …
[10h] …
(register 1 ID) [register 1 unit] (register 1 format) (register 1 value) …
(register 2 ID) [register 2 unit] (register 2 format) (register 2 value) …
(register n ID) [register n unit] (register n format) (register n value) …
(CRC) [0Dh]

[register unit] is a unit identification code and specifies units.

(register format) described in detail in section 4.2, page 7, is the floating-point data format
consisting of:

    [number of bytes] [sign+exponent]

An occurrence of an unsupported register in the request is handled by omitting the register in the
response. This means that a request for ex. 5 registers may result in a response with less than 5
registers – in principle no registers at all.

Example

Request:

Application Layer:

Register ID 1 = 128

  Number of registers

10  01  00  80

CID and data

Data Link Layer:

Physical Layer:

3F  10  01  00  80

  Destination address added

3F  10  01  00  80  D4  08

CRC added

3F  10  01  00  1B  7F  D4  08

Byte stuffing

80  3F  10  01  00  1B  7F  D4  08  0D

Start and stop byte added

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  19

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Response:

Physical Layer:

40  3F  10  00  1B  7F  16  04  11  01  2A  F0  24  F3  8A  0D

Data Link Layer:

3F  10  00  1B  7F  16  04  11  01  2A  F0  24  F3  8A

  Start and stop byte removed

3F  10  00  80  16  04  11  01  2A  F0  24  F3  8A

  Destuffing

3F  10  00  80  16  04  11  01  2A  F0  24  63  03  00  00

  CRC calculated

3F  10  00  80  16  04  11  01  2A  F0  24

  New and old CRC removed

Application Layer:

10  00  80  16  04  11  01  2A  F0  24

  Destination address removed

012AF024h => int = 19.591.204

11h => SI = 0 ; SE = 0 ; exp = 17 => -10(cid:152)int(cid:152)10-1^0 (cid:152) 17

04h = 4 bytes

16h = kW
0080h = MAX EFFEKT1/ÅR

10h = GetRegister

MAX EFFEKT1/ÅR (Register ID 128) =
-10(cid:152)19.591.204(cid:152)10-1^0 (cid:152) 17 kW = 19.591.204 (cid:152)1017 kW

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  20

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Description of logger application commands

6.3
The description in this section covers the loggermodules 670B000000 and 6700220000 for MULTICAL®601
and the internal logger in MULTICAL®801.

Remember to use the right destination address when communicating with the logger (See Data link
layer, section 3.2, page 6).
670B is a MC601 top module and replies on 0x7F.
670022 is a MC601 base module and replies on 0xBF.
MC801 is a meter and therfore uses 0x3F hovever the logger commands in this section is accessed
using 0x7F (Logger functionality in MC801 is seen as a virtual top module).

The following terms and expressions are used for documentation of a logger (module or meter):

Term
Device
Log
Record

Explanation
A device is the module/meter in the field.
A specific log contains data logged in records.
A specific log record contains a copy of each of the registers covered by the log.

Following log example is referenced in the four readout commands in subchapter 6.3.1 to 6.3.4:

Log ID: 01 (interval logger)

Log
record ID
...

Log record timestamp

…

Logged register values

003C
…

0044
…

10F2h  2007-05-24 10:00:00  1D8B83A0  9DBAD708
10F3h  2007-05-24 11:00:00  FF068662  E993DC05
10F4h  2007-05-24 12:00:00  B50A5C5E  93E05047
10F5h  2007-05-24 13:00:00  CF065396  F2584C96
10F6h  2007-05-24 14:00:00  A38FB479  2E44778E
10F7h  2007-05-24 15:00:00  2F4CED5F  931D90A5
10F8h  2007-05-24 16:00:00  04AC8C55  B5700C78
10F9h  2007-05-24 17:00:00  6067E035  7CE685E5
10FAh  2007-05-24 18:00:00  7BA2B68E  F3947871
10FBh  2007-05-24 19:00:00  717F72A0  8C7D6E62
10FCh  2007-05-24 20:00:00  99F948C3  EA02904C
10FDh  2007-05-24 21:00:00  331CD991  5D43BB58
10FEh  2007-05-24 22:00:00  A835284A  F4B07BD8
10FFh  2007-05-24 23:00:00  96F3547C  B3A8F8B2
1100h  2007-05-25 00:00:00  DA24B37C  12DD6D6B
1101h  2007-05-25 01:00:00  0EF8D8A0  4388026C
1102h  2007-05-25 02:00:00  0CD83E6A  54F6D02E

…

…

…

…

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  21

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Each parameter in the examples is referenced with a certain abbreviation.
The abbreviations of the most common command parameters in this document are described in the following
table:

Parameter abbreviation  Parameter description
LID

8bit – Log ID.

The ID of a specific log.
A logger device contains one or more logs. Each log in a device has a unique
consecutively number; the log ID.

 The log ID is not unique between devices!

NRgs

8bit – Number of registers

Reg ID 1-n

Number of registers requested – also denoted ‘n’.
n is limited to the range 1-8 to keep the response frame in a manageable
size.
16bit – Register ID 1-n

Register ID for each of the n registers requested (see paragraph 4.4.1).
For a list of registers supported see paragraph 5.2.

NRcs

8bit – Number of records

Number of records requested – also denoted ‘m’.
If the requested response is larger than MaxL or communication limits in the
device, then the number of records is reduced in the response (see response
example in paragraph 6.3.1).

MaxL

8bit – Max APL Length

Time

NRtR

The maximum APL length in the response.
This is used to limit the response size if for instance the receiver of the
response has a limited communication buffer.
If the requested response exceeds this limit the NRcs is reduced before
creating the response (see response example in paragraph 6.3.1).

64 bit – Timestamp

The timestamp used as search criteria in time based readout commands.
For a description of the KMP timestamp format see paragraph 4.3.

8bit – Number of returned registers

Number of registers included in the response – also denoted ‘N’.
If all registers requested exist in the log this number equals NRgs. Any
register ID not included in the entity list will be left out of the response.

RecID 1-M

16bit – Record ID 1-M

Record ID for each of the records in the response.
M denotes the amount of records in the response. If all records requested
exist in the log, and the response size is within the limits (see the
description of NRcs), this number equals NRcs.
The records is read out and placed in the response frame in a consecutive
order (either descending or ascending depending on the command) with the
record ID found in the search placed first.

Reg ID 1-N format

24bit – Reg ID 1-N format

The register format of each register included in the frame.
For a description of the KMP register format see paragraph 4.4.2.

Reg ID 1-N value

8bit – Reg ID n value

The register value of each register included in the frame (see paragraph
4.4.3).

LRORecID

16bit – LastReadOutRecord ID

The ID of the record included in the response which was readout last/most
recent from the log.
It will always correspond to the ID of the last record in a specific response,
because the last record in the response is the last record readout from the
log.
Depending on the command (whether it read out towards present or towards
past) this will correspond to either the newest/highest or the oldest/lowest
record ID from the log included in the response.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  22

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Parameter abbreviation  Parameter description
NewRec ID

16bit – NewestRecord ID

INFO

8bit – Info

The ID of the newest record contained in the log.
This is (only) changed when the log creates a new record – and then set to
the ID of this new record.

An info byte indicating errors concerning log readout.
The byte is interpreted as a collection of 8 single bit flags.
0 = No error ; 1 = Error occurred
Bit  Error
0
1
2

Log is empty.
Not used
Requested record ID or timestamp is out of the current range of records
in the log.

3  MaxL was reached. One or more records were excluded in the response.
4

The end of the log was reached. One or more records were excluded in
the response.
When reading towards present this will occur when the newest record
in the log is reached. When reading towards past this will occur when
the oldest record in the log is reached.
Not used
Not used
Not used

5
6
7

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  23

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.1 A0h  GetLogTimePresent

(GetLogFromTimeTowardsPresent)

Function:

Log readout from specified ‘timestamp’ towards ‘now’.

The log is searched for the oldest record with a timestamp equal to or newer than the
requested timestamp.
This record and the following records (with higher/ascending record ID’s) is read out.

 If the clock in the device is changed, an overlap of timestamps can occur.

 If the response contains a log error (if any bit in the INFO byte is set), the response

package length and application content can vary. The CID and INFO byte is always included
and placed as respectively the first and the last byte in the application layer.

Request:

[A0h] [Log ID] [Number of registers] (Reg ID 1) (Reg ID 2) …
… (Reg ID n) [Number of records] [Max APL length] (Time)

Parameter
LID

Description
8bit – Log ID*

NRgs

8bit – Number of registers*

The ID of a specific log.

Reg ID 1-n

16bit – Register ID 1-n*

Number of registers requested – also denoted ‘n’.

NRcs

MaxL

Time

Register ID for each of the n registers requested.

8bit – Number of records*

Number of records requested – also denoted ‘m’.

8bit – Max APL Length*

The maximum APL length in the response.

64 bit – Timestamp*

The timestamp used as search criteria in time based readout
commands.
* See abbreviation list at paragraph 6.3

Example:

Two registers (E1 and V1) from 2007-05-24 10:30:00 and 3 records forward

(cid:160)

CID
0
A0

LID  NRgs  Reg ID 1
3
1
4
003C
01

2
02

Reg ID 2
5
6
0044

NRcs  MaxL

7
03

8
80

9

10

Time

12

11
14
xxxx001E0A180507

13

15

16

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  24

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Response:

[A0h] [Log type ID] [Number of returned registers]
(RecordID 1)
value)

(Reg ID 1) (Reg ID 1 format: [Unit] [Number of bytes] [SignExp] ) (Reg ID 1

(Reg ID 2) (Reg ID 2 format: [Unit] [Number of bytes] [SignExp] ) (Reg ID 2

value)

…
(Reg ID n) (Reg ID n format: [Unit] [Number of bytes] [SignExp] ) (Reg ID n

value)
(RecordID 2)
…
(RecordID M)
(LastReadOutRecord ID) (NewestRecord ID) [Info]

(Reg ID 1 value) (Reg ID 2 value) … (Reg ID n value)

(Reg ID 1 value) (Reg ID 2 value) … (Reg ID n value)

Parameter
NRtR

Description
8bit – Number of returned registers*

Number of registers included in the response – also denoted ‘N’.

RecID 1-M

16bit – Record ID 1-M*

Record ID for each of the records in the response.

Reg ID 1-N format  24bit – Reg ID 1-N format*

The register format of each register included in the response.
See section 4.2, page 7, for details on the floating-point data
format.

Reg ID 1-N value  xxbit1) – Reg ID n value*

LRORecID

The register value of each register included in the response for
each log record.
16bit – LastReadOutRecord ID*

The ID of the record included in the response which was readout
last/most recent from the log.

NewRec ID

16bit – NewestRecord ID*

The ID of the newest record contained in the log.

INFO

8bit – Info*

 An info byte indicating errors concerning log readout.

* See abbreviation list at paragraph 6.3

Example (see log record examples at paragraph 6.3):

CID  LTID  NRtR  RecID 1
4
2
0
3
10F3
02
A0

1
01

Reg ID 1
6
5
003C

Reg ID 1 format
9
8
7
080400

Reg ID 1 value

10

12
11
FF068662

13

Reg ID 2
15
14

0044

Reg ID 2 format
18
17
16
270400

Reg ID 2 value

19

21
20
E993DC05

22

RecID 2
24
23

10F4

RecID 3
34
33

10F5

Reg ID 1 value

Reg ID 2 value

25

27
26
B50A5C5E

28

29

31
30
93E05047

32

Reg ID 1 value

Reg ID 2 value

35

37
36
CF065396

38

39

41
40
F2584C96

42

LRORecID  NewRec ID  INFO
47
45
43
00

1102

10F5

46

44

(cid:160)

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  25

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

10F3

Record ID
10F4

10F5

Register ID

003C  FF068662  B50A5C5E  CF065396
0044  E993DC05  93E05047  F2584C96

Expected size of the response, with NRcs and NRgs in consideration (SOF, DestAddr, CRC and
EOF incl. – byte stuffing not incl.):

APL length

=  ( CID (cid:108) NRtR ) + (RecID 1-M ) + ( Reg ID 1-N + Reg ID 1-N format ) +

( M * Reg ID 1-N value ) + ( LRORecID (cid:108) INFO )

(cid:159)  ( 3 ) + ( 2 * m ) + ( 5 * n ) + ( 4 * m * n ) + ( 5 )

Response length  =  ( SOF & DestAddr ) + APL length + ( CRC & EOF )

(cid:159)  ( 2 ) + ( 3 ) + ( 2 * m ) + ( 5 * n ) + ( 4 * m * n ) + ( 5 ) + ( 3 )

This result in the following table:

m (Number of records - NRcs)

0

1

2

3

4

5

6

7

8

9  10  11  12  13  14  15  16  17  18  19

)
s
g
R
N

-
d
r
o
c
e
r

.
r
p
s
r
e
t
s
i
g
e
r

f
o
r
e
b
m
u
N

(
n

0  13  15  17  19  21  23  25  27  29  31  33  35  37  39  41  43  45  47  49  51

1  18  24  30  36  42  48  54  60  66  72  78  84  90  96  102 108 114 120 126 132

2  23  33  43  53  63  73  83  93  103 113 123 133 143 153 163 173 183 193 203 213

3  28  42  56  70  84  98  112 126 140 154 168 182 196 210 224 238 252 266 280 294

4  33  51  69  87  105 123 141 159 177 195 213 231 249 267 285 303 321 339 357 375

5  38  60  82  104 126 148 170 192 214 236 258 280 302 324 346 368 390 412 434 456

6  43  69  95  121 147 173 199 225 251 277 303 329 355 381 407 433 459 485 511 537

7  48  78  108 138 168 198 228 258 288 318 348 378 408 438 468 498 528 558 588 618

8  53  87  121 155 189 223 257 291 325 359 393 427 461 495 529 563 597 631 665 699

MaxL will overrule this and reduce the NRcs if the size is larger than MaxL after excluding non
existing registers. The broken line indicates a limitation of 128 bytes in the response.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  26

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.2 A1h  GetLogLastPresent

(GetLogFromLastReadTowardsPresent)

Function:

Log readout from ‘LRORecID’ towards ‘now’.

This command returns the records from the newest read record (‘LRORecID’) and forward –
‘LRORecID’ not included (see 6.3, 6.3.5 and 6.3.6 for examples).

On request, the record and the following/newer records, after the newest record already read
out, are returned in the response.
If the amount of new records exceeds the limitations of the device response size, the oldest
records in the interval is returned.
If the log contains no new records, a response without any records is returned.

 This command is not intended for use with any other log readout commands.

Other log readout commands can change the registration of which record, of the already
readout records, is the newest. This can cause records to be missed by this command.
The situation is handled product specific.

 If the response contains a log error (if any bit in the INFO byte is set), the response

package length and application content can vary. The CID and INFO bytes are always included
and placed as respectively the first and the last byte in the application layer.

Request:

[A1h] [Log type ID] [Number of registers] (Reg ID 1) (Reg ID 2) …
… (Reg ID n) [Number of records] [Max APL length]

Parameter
LID

Description
8bit – Log ID*

NRgs

8bit – Number of registers*

The ID of a specific log.

Reg ID 1-n

16bit – Register ID 1-n*

Number of registers requested – also denoted ‘n’.

NRcs

MaxL

Register ID for each of the n registers requested.

8bit – Number of records*

Number of records requested – also denoted ‘m’.

8bit – Max APL Length*

The maximum APL length in the response.

* See abbreviation list at paragraph 6.3

Example:

Two registers (E1 and V1) from LRORecID and 3 records forward

(cid:160)

CID
0
A1

LID  NRgs  Reg ID 1
3
1
4
003C
01

2
02

Reg ID 2  NRcs  MaxL
5
6
0044

7
03

8
80

Response:

The response frame is identical with the response frame of GetLogTimePresent (see 6.3.1 –
response) when LRORecID is 10F2h – except for the CID, which is A1h.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  27

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.3 A2h  GetLogIDPresent

(GetLogFromIDTowardsPresent)

Function:

Log readout from specified ‘Record ID Requested’ towards ‘now’.

The log is searched for a record with an ID equal to the requested record ID.
This record and the following records (with higher/ascending record ID’s) is read out.

 If the requested ID doesn’t exist, the readout starts at the following ID. Whether an info bit

is set in this situation is product specific.

 If the response contains a log error (if any bit in the INFO byte is set), the response

package length and application content can vary. The CID and INFO bytes are always included
and placed as respectively the first and the last byte in the application layer.

Request:

[A2h] [Log type ID] [Number of registers] (Reg ID 1) (Reg ID 2) …
… (Reg ID n) [Number of records] [Max APL length] (Record ID Requested)

Parameter
LID

Description
8bit – Log ID*

NRgs

8bit – Number of registers*

The ID of a specific log.

Reg ID 1-n

16bit – Register ID 1-n*

Number of registers requested – also denoted ‘n’.

NRcs

MaxL

Register ID for each of the n registers requested.

8bit – Number of records*

Number of records requested – also denoted ‘m’.

8bit – Max APL Length*

The maximum APL length in the response.

RecIDReq

16bit – Record ID Requested

The record ID requested as the first and oldest record included in
the response.
* See abbreviation list at paragraph 6.3

Example:

Two registers (E1 and V1) from record 10F3h and 3 records forward

(cid:160)

CID
0
A2

LID  NRgs  Reg ID 1
3
1
4
003C
01

2
02

Reg ID 2  NRcs  MaxL  RecIDReg
5
10
6
0044

9
10F3

8
80

7
03

Response:

The response frame is identical with the response frame of GetLogTimePresent (see 6.3.1 –
response) – except for the CID, which is A2h.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  28

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.4 A3h  GetLogTimePast

(GetLogFromTimeTowardsPast)

Function:

Log readout from specified ‘timestamp’ towards the ‘past’.

The log is searched for the oldest record with a timestamp equal to or older than the
requested timestamp.
The starting point in the log for the response is then the record ID found in the search minus
the value of ‘Record ID Offset’. This record and the following records (with lower/descending
record ID’s) are read out.

 If the clock in the device is changed, an overlap of timestamps can occur.

 If the response contains a log error (if any bit in the INFO byte is set), the response

package length and application content can vary. The CID and INFO bytes are always included
and placed as respectively the first and the last byte in the application layer.

Request:

[A3h] [Log type ID] [Number of registers] (Reg ID 1) (Reg ID 2) …
… (Reg ID n) [Number of records] [Max APL length] (Time) (Record ID Offset)

Parameter
LID

Description
8bit – Log ID*

NRgs

8bit – Number of registers*

The ID of a specific log.

Reg ID 1-n

16bit – Register ID 1-n*

Number of registers requested – also denoted ‘n’.

NRcs

MaxL

Time

Register ID for each of the n registers requested.

8bit – Number of records*

Number of records requested – also denoted ‘m’.

8bit – Max APL Length*

The maximum APL length in the response.

64 bit – Timestamp*

RecIDOff

16bit – Record ID Offset

The timestamp used as search criteria in time based readout
commands.

A record ID offset in the log, in relation to the record ID found in
the search, defining the readout starting point in the log for the
response – also denoted ‘k’.
If for instance a request for 20 records from 12 o’clock a.m. is
send, but only 10 records is contained in the answer because of
APL overflow (see MaxL). Then another request for 10 records at
the same timestamp, but with ‘Record ID Offset’ set to 10, would
return the missing 10 records.

* See abbreviation list at paragraph 6.3

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  29

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Example (see log record examples at paragraph 6.3):

Two registers (E1 and V1) from 2007-05-24 19:45:00 and 3 records backwards with an offset
of 6 records.

(cid:160)

CID
0
A3

LID  NRgs  Reg ID 1
3
1
4
003C
01

2
02

Reg ID 2
5
6
0044

NRcs  MaxL

7
03

8
80

9

10

Time

12

11
14
xxxx002D13180507

13

15

16

RecIDOff
16
16

0006

Response:

[A0h] [Log type ID] [Number of returned registers]
(RecordID M)
value)

(Reg ID 1) (Reg ID 1 format: [Unit] [Number of bytes] [SignExp] ) (Reg ID 1

(Reg ID 2) (Reg ID 2 format: [Unit] [Number of bytes] [SignExp] ) (Reg ID 2

value)

…
(Reg ID n) (Reg ID n format: [Unit] [Number of bytes] [SignExp] ) (Reg ID n

value)
(RecordID M-1)
…
(RecordID 1)
(LastReadOutRecord ID) (NewestRecord ID) [Info]

(Reg ID 1 value) (Reg ID 2 value) … (Reg ID n value)

(Reg ID 1 value) (Reg ID 2 value) … (Reg ID n value)

Parameter
NRtR

Description
8bit – Number of returned registers*

Number of registers included in the response – also denoted ‘N’.

RecID M-1

16bit – Record ID M-1*

Record ID for each of the records in the response.

Reg ID 1-N format  24bit – Reg ID 1-N format*

The register format of each register included in the response.
See section 4.2, page 7, for details on the floating-point data
format.

Reg ID 1-N value  xxbit1) – Reg ID n value*

LRORecID

The register value of each register included in the response for
each log record.
16bit – LastReadOutRecord ID*

The ID of the record included in the response which was readout
last/most recent from the log.

NewRec ID

16bit – NewestRecord ID*

The ID of the newest record contained in the log.

INFO

8bit – Info*

 An info byte indicating errors concerning log readout.

* See abbreviation list at paragraph 6.3

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  30

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Example:

CID  LTID  NRtR  RecID 1
3
4
2
0
10F5
02
A0

1
01

Reg ID 1
5
6
003C

Reg ID 1 format
9
8
7
080400

Reg ID 1 value

10

11
12
CF065396

13

Reg ID 2
15
14

0044

Reg ID 2 format
18
17
16
270400

Reg ID 2 value

19

20
21
F2584C96

22

RecID 2
24
23

10F4

RecID 3
34
33

10F3

Reg ID 1 value

Reg ID 2 value

25

27
26
B50A5C5E

28

29

31
30
93E05047

32

Reg ID 1 value

Reg ID 2 value

35

37
36
FF068662

38

39

41
40
E993DC05

42

LRORecID  NewRec ID  INFO
47
45
43
00

1102

10F3

46

44

(cid:160)

10F5

Record ID
10F4

10F3

Register ID

003C  CF065396  B50A5C5E  FF068662
0044  F2584C96  93E05047  E993DC05

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  31

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.5 9Bh  GetEventStatus

Function:

Returns the four event status bytes.

Request:

[9Bh]

Example:

CID
0
9B

Response:

[9Bh][EventStatusByte0] [EventStatusByte1] [EventStatusByte2] [EventStatusByte3]

Parameter
ES0

ES1

ES2

Description
8bit – EventStatusByte0

Event status byte 0.
Not used.
8bit – EventStatusByte1

Event status byte 1.
Not used.
8bit – EventStatusByte2

Event status byte 2.
Each bit in this byte is used as flags to indicate if any of the logs in
a device containes new unread log records.
0 = No unread data in log ; 1 = New unread data in log
Bit  Log
0
1
2-7  Not used

Interval log contain new unread log record(s)
RTC log contain new unread log record(s)

ES3

8bit – EventStatusByte3

Event status byte 3.
Not used.

* See abbreviation list at paragraph 6.3

Example:

CID  ES0  ES1  ES2  ES3
3
0
xx
9B

4
xx

5
xx

2
xx

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  32

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.6 9Ch

ClearEventStatus

Function:

Clear the event status bytes.

The function is prepared for future products, and for coexistance with the electrical meters.
Only “conditional clear” (CC) is supported, in the MC601 logger modules and in MC801.

An example of a log and some read out scenarios are descript here:

A

B

C

D

E

F

G

H

Rec ID
…
0FFEh
0FFFh
1000h
1001h
1002h
1003h
1004h
1005h
1006h
1007h
1008h
1009h
100Ah
100Bh
…

  Unused records

  Unread records

  Records treated as already read out

  Specific readout

Internally controlled barrier (LRORecID – ‘LastReadOutRecordID’) between read and unread records

A) The log contains data up to record ID 1008. The last record ID read (followed by a

‘ClearEventStatus’) previous to this state is record ID 1001h. The ‘EventStatusByte’ flag
will be 1 for the shown log because of the unread records in the log.

B) Record 1002h and 1003h are read out. The last record ID read is still record ID 1001h,

C)

while LRORecID is only updated when ‘ClearEventStatus’ is called at the log in question.
‘ClearEventStatus’ is called. The last record ID read is changed to record ID 1003h. The
‘EventStatusByte’ flag is still 1 because of the unread records in the log.

D) Record 0FFEh to 1000h are read out. A read out will not affect the last record ID read

when the record ID(s) read out is older than the newest record ever read.
‘ClearEventStatus’ is called. The last record ID read is unchanged (see D).

E)
F) Record 1006h to the newest record in the log (record ID 1008h) are read out. As descript

G)

at B and D the read out action it self doesn’t change the LRORecID.
‘ClearEventStatus’ is called. The last record ID read is now changed to record ID 1008h.
The ‘EventStatusByte’ flag will be 0 now because the newest read record equals the
newest log in the log. All data from record ID 1008h and back in time is perceive as read,
even though log record 1004h and 1005h hasn’t been read in this example.
H) A new record is created in the log. This changes the ‘EventStatusByte’ flag to 1.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  33

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Request:

[9Ch] [ClearEventStatusByte0-CC] [ClearEventStatusByte1-CC] [ClearEventStatusByte2-CC]
[ClearEventStatusByte3-CC] [ClearEventStatusByte0-UC] [ClearEventStatusByte1-UC]
[ClearEventStatusByte2-UC] [ClearEventStatusByte3-UC]

Parameter
CS0c

CS1c

CS2c

Description
8bit – ClearEventStatusByte0-CC

Not used – default 0.

8bit – ClearEventStatusByte1-CC

Not used – default 0.

8bit – ClearEventStatusByte2-CC

Clear mask for event status byte 2.
This byte is used as a mask for clearing the event status byte 2.
Each bit in ClearEventStatusByte2 control the same bit in
EventStatusByte2. The flag is only cleared if the newest record in
the corresponding log is read out.
0 = Event flag is not cleared
1 = Event flag is cleared if the condition(s) is true
Bit  Log
0
1
2-7

Conditional clear of Interval log flag
Conditional clear of RTC log flag

Not used

CS3c

CS0u

CS1u

CS2u

CS3u

8bit – ClearEventStatusByte3-CC

Not used – default 0.

8bit – ClearEventStatusByte0-UC

Not used – default 0.

8bit – ClearEventStatusByte1-UC

Not used – default 0.

8bit – ClearEventStatusByte2-UC

Not used – default 0.

8bit – ClearEventStatusByte3-UC

Not used – default 0.

* See abbreviation list at paragraph 6.3

Example:

CID  CS0c  CS1c  CS2c  CS3c  CS0u CS1u CS2u CS3u
5
0
00
9C

7
00

9
00

6
00

4
01

2
00

3
00

8
00

Response:

[9Bh][EventStatusByte0] [EventStatusByte1] [EventStatusByte2] [EventStatusByte3]

Example:

CID  ES0  ES1  ES2  ES3
3
0
xx
9B

4
xx

5
xx

2
xx

As CID 9B GetEventStatus.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  34

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

6.3.7

PutRegister  -  CID = 11h

Function:

This command is used for changing the value of a given register.

Request:

[80h] [destination address][11h] (Password) (register ID)[Unit](Format)(Value)(CRC) [0Dh]

(Password) is a 16 bit value. If the password is invalid, the register value is not changed.

(register ID) is a 16 bit register identification codes and specifies registers to be read out.

[register unit] is a unit identification code and specifies units.

(register format) is the floating-point data format consisting of:

 [number of bytes] [sign+exponent]

(Value) is the new value to use.

Response:

[06h]

ACK (0x06) if the walue was written, othervise no reply is given.

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  35

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

Index

F

Formatting

Floating point................................................................. 7
Registers ........................................................................ 9

K

KMP Commands

By CID

0x01......................................................................... 15
0x02......................................................................... 17
0x09......................................................................... 18
0x10......................................................................... 19
0x11......................................................................... 35
0x9B ........................................................................ 32
0x9C ........................................................................ 33
0xA0........................................................................ 24
0xA1........................................................................ 27
0xA2........................................................................ 28
0xA3........................................................................ 29

By Name

ClearEventStatus ......................................................33
GetEventStatus.........................................................32
GetLogIDPresent .....................................................28
GetLogLastPresent...................................................27
GetLogTimePast ......................................................29
GetLogTimePresent .................................................24
GetRegister ..............................................................19
GetSerialNo..............................................................17
GetType....................................................................15
PutRegister...............................................................35
SetClock...................................................................18

P

Protocol...............................................................................5

T

Timing

Communication ..............................................................3

Last page:  36
Opr.:  GHA

Date for last revision:   2009-12-07

Aut.:LTS/AJ/MAJ  QA:

SL

EDB:
GHA

Doc. No.:
5512-447

Rev.: M1
Page:  36

Kamstrup A/S,  Stilling,  DK-8660 Skanderborg,  Denmark

