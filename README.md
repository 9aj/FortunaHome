# FortunaHome

FortunaHome is a simple application for the LaFortuna that controls basic applications connected to the device through its inbuilt USART

## Installation

Requires GNU Make to install
Connect the LaFortuna and enable S6 to flash.
'''bash
make
'''

## Usage
Connect the serial port TX/RX connections to a USB.
Load the Java application which handles serial communications and its device.
On the device, use the rotary encoder to scroll through the options, and select with the center button to send a request through serial communications to your computer.

## License
[MIT](https://choosealicense.com/licenses/mit/)

