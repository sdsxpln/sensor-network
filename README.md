# Local wired sensor network

![sensor_networ](./doc/sensor_network.png)

## Background and motivation

### Problems

I have developed a lot of IoT prototypes so far, then I have observed that most of my IoT projects require a communication protocol for wired sensor networking, satisfiying the requirements below:

- low power consumption and lower voltage (5V or 3.3V)
- bus topology rather than hub and spoke
- two-wire
- cheap (~$1 per node) and open
- small software footprint

There are a lot of such technologies for in-vehicle network (CAN/LIN), buidling management (BACnet) or factory automation (PROFINET), but none of them satisfies the requirements above.

This is a project to develop a networking protocol and building blocks for local wired sensor network.

## Network architecture

The network is composed of multiple nodes(blocks) and one scheduler.

```
     -----+---------------------+---------------------------+-------- I2C bus
          |                     |                           |
   +------|-----+        +------|-----+              +------|-----+
   |   [node]   |        |   [node]   |              |   [node]   |
   |      |     |        |      |     |              |      |     |                  (         )
   |  [sensor]  |        |  [sensor]  |              |      +------------[IoT GW]---(   Cloud   )
   +------------+        +------------+    . . .     +------------+                  (         )
       block                 block                     scheduler
      (slave)               (slave)                    (master)
```

Note: I am going to support CAN bus as well.

### Interfaces among blocks

All the blocks developed in this project support [Plug&Play protocol](./doc/PROTOCOL.md) that runs on UART.

```
                    USB hub
                     +---+
[block A]--UART/USB--|   |
[block B]--UART/USB--|   |--USB--[IoT GW]
[block C]--UART/USB--|   |
                     +---+

               hub&spoke topology
```

It also runs on I2C.

```
      <- - - - - I2C backplane - - - - ->
[block A]---[block B]---[block C]---[Scheduler]--UART/USB--[IoT GW]

            bus topology (daisy-chain)
```

### 8bit MCU as core of node

I use [PIC16F1829](http://ww1.microchip.com/downloads/en/DeviceDoc/41440A.pdf) that satisfies the requirements of this project.

Clock speed:
- 4MHz typical
- 32MHz (8MHz w/ PLL) for high sampling rate

### Node prototype

The base board below is a common hardware part of node:

![prototype3](./doc/prototype3.png)

![pico2](https://docs.google.com/drawings/d/1_WCC4vuPbIT2im9c337ibk5xEq9WKzrT9907IOWTCCA/pub?w=680&h=400)

## Networking examples

One I2C master and three I2C slaves are connected with each other via backplane bus on the back of base board

![compact2](./doc/compact2.png)

A similar construct to the above, but all the boards are connected with each other in a daisy-chain manner:

![daisy_chain](./doc/daisy_chain.png)

## Implementation

Note: I use [MPLAB Code Configurator (MCC)](http://www.microchip.com/mplab/mplab-code-configurator) to generate code for USART, I2C, PWM, Timer etc.

### Plug&Play protocol

- [Plug&play protocol specification](./doc/PROTOCOL.md)
- [Implementation](./blocks/lib/protocol.X)

Including it as a library:
- [Step1: include the protocol library directory](./doc/mcc_eusart4.png)
- [Step2: include the protocol library in your project](./doc/mcc_eusart3.png)
- [Step3: exclude mcc generated eusart libraries from your project](./doc/mcc_eusart2.png)
- [Step4: enable eusart interrupts](./doc/mcc_eusart.png)

### Blocks

- [5V: Character LCD actuator block (AQM1602XA-RN-GBW)](./blocks/i2c_slave_lcd.X), [pin assignment](./doc/lcd_pin.png)
- [5V: Acceleration sensor block （KXR94-2050)](./blocks/i2c_slave_accel.X), [pin assignment](./doc/acceleration_pin.png)
- [5V: Speed sensor block (A1324LUA-T)](./blocks/i2c_slave_speed.X), [pin assignment](./doc/rotation_pin.png)
- [5V: Temperature and humidity sensor block (HDC1000)](./blocks/i2c_slave_temp.X)

### CLI example

```
#WHO
$:WHO:BACKPLANE-MASTER
#SCN
#MAP
$:MAP:16,17,19
$:RSC:0,0,0,0|0,0,0,0|0,0,0,0|0,0,0,0|0,0,0,0|17,0,0,0|0,0,0,0
#POS:12
#WSC:19
#RSC
$:RSC:0,0,0,0|0,0,0,0|0,0,0,0|19,0,0,0|0,0,0,0|17,0,0,0|0,0,0,0
#STA
%19:FLOAT:0.60,1.21,-0.09
%19:FLOAT:0.61,1.22,-0.08
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.21,-0.09
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.20,-0.08
%19:FLOAT:0.60,1.21,-0.08
%17:UINT16_T:0
%19:FLOAT:0.61,1.21,-0.09
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.20,-0.09
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.21,-0.08
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.21,-0.08
%19:FLOAT:0.59,1.21,-0.08
%19:FLOAT:0.61,1.21,-0.08
%19:FLOAT:0.60,1.21,-0.08
%17:UINT16_T:0
%19:FLOAT:0.61,1.21,-0.08
%19:FLOAT:0.59,1.21,-0.08
%19:FLOAT:0.61,1.20,-0.08
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.60,1.20,-0.09
%19:FLOAT:0.60,1.21,-0.08
%19:FLOAT:0.61,1.20,-0.08
%19:FLOAT:0.61,1.20,-0.08
%19:FLOAT:0.61,1.21,-0.08
%19:FLOAT:0.59,1.21,-0.07
%17:UINT16_T:0
%19:FLOAT:0.61,1.20,-0.08
            :
#I2C:16
$:WHO:16
#CLR
#LED:ON
#LED:OFF
#STR:Hello World!
#NWL
#STR:Guten Tag!
#I2C:1
            :
#STP
*:STP:ACK
```

### Physical wiring

In some projects, I used telephone line (6P4C) with RJ11 moduler plug/jack, as I2C bus. I used this tool to make wires among nodes: [Crimper for RJ11](http://www.mco.co.jp/products/tel/telephonetool/ta-642t.html). Telephone line makes physical wirling very easy.

6P4C telephone line is suitable for I2C with power line: SDA, SCL, 5V, GND.

