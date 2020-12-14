# N64/DualShock

Author: Adrian Padin (padin.adrian@gmail.com)
Date: December 13, 2020

This is the results of my work attempting to use a PlayStation 2 DualShock 2 controller on a Nintendo 64.

I used an [Adafruit Trinket Mini](https://learn.adafruit.com/introducing-trinket) to read from the DualShock controller, remap the buttons, and send the data to the N64.

Some of the tasks I accomplished along the way:

* Implementing the beginnings of a SPI library for the Trinket using the Atmel USI (Universal Serial Interface).

* Using bit-banging to read and write from the N64 using the N64's unique single-wire interface.

* Reprogramming the Trinket to use a different bootloader which allows it to boot much faster.

You can read more about the N64, SPI, and other topics by checking out the resources below.

## Prototype

This is a picture of the prototype device:

![](pics/prototype.jpg "Prototype")

## Resources:

### embedded_tools

* Github repository:
    [https://github.com/padinadrian/embedded_tools](https://github.com/padinadrian/embedded_tools)

### ATTiny85 Bootloader

* Repairing bootloader:
   [https://learn.adafruit.com/introducing-trinket/repairing-bootloader](https://learn.adafruit.com/introducing-trinket/repairing-bootloader)

* New bootloader image:
    [https://forums.adafruit.com/viewtopic.php?f=52&t=54218&p=376898&hilit=trinket+bootloader+skip#p376898](https://forums.adafruit.com/viewtopic.php?f=52&t=54218&p=376898&hilit=trinket+bootloader+skip#p376898)


## TODO

* Allow an attachable PS2 controller (instead of break-out demo)
  * https://www.amazon.com/PS2-Controller-Extension-Cable-Playstation-2/dp/B00005MDZ1

* Put together a more sturdy setup (PCB? or solder board?)
