# Convert to Mongoose OS

A minimal firmware for OTA (over the air) flashing Mongoose OS from Tasmota
or compatible firmware types.

## Overview

This is a first working draft for an intermediate firmware, which can be used to
wirelessly restore [Mongoose OS](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md)
on esp8266 based IoT devices initially shipped with a Mongoose OS based
firmware. This firmware will install a minimal version of Mongoose OS, and you
can continue from there to your favourite target release, such as e.g. the
Shelly stock firmware.

## Supported Devices

Any esp8266 device with a physical flash size of 2 or 4 MB, which was initially
shipped with a Mongoose OS based firmware.

## Install

**Warning:** _This application is still at an early stage. If something fails,
your device may be bricked, if you don't know how to flash a new firmware over a
wired connection. Proceed with caution!_

There are two versions with different bootloader configuration locations. The
default location in Mongoose OS is `0x7000`, but some devices use `0x1000`
instead. Be careful to pick the appropriate version, or the device will be
bricked during the first boot of the target firmware!

For Shelly 1, Shelly 1PM, Shelly 2, Shelly 2.5, and Shelly EM use __0x1000__,
for Shelly Plug S, Shelly RGBW2, Shelly Dimmer 1, Shelly Dimmer 2, Shelly Bulb,
Shelly Vintage, Shelly Plug US, Shelly H&T, Shelly Duo use __0x7000__.

Available versions:
 * [0x1000](https://github.com/yaourdt/tasmota-to-mgos/blob/master/binaries/mgos512k-0x1.bin)
 * [0x7000](https://github.com/yaourdt/tasmota-to-mgos/blob/master/binaries/mgos512k-0x7.bin)

Use your current firmware's update mechanism to apply x2mg as you would for any
other update. If the process succeeds, the device will reboot and you will see a
new WiFi network labeled _mg-????_. Be patient, this process can take up to two
minutes.

As soon as you have joined the devices WiFi network, you can install the desired
version of a Mongoose OS based firmware via the command

```
curl -i -F filedata=@./fw.zip http://10.42.42.44/update
```

Replace `./fw.zip` with the path to your target firmware ZIP-file. If you own a
Shelly device and want to revert to stock firmware, you can find the matching
files [here](https://api.shelly.cloud/files/firmware).

## Trouble shooting

### Tasmota: Upload Failed - Not compatible
If you cannot upload the intermediate firmware to Tasmota due to a _not
compatible_ error, run `Setoption78 1` in the Tasmota console to disable
the compatibility check and allow for the update to be applied. See
[this github issue](https://github.com/esphome/issues/issues/955).

### Tasmota: Upload Failed - Upload buffer miscompare
There is not enough space for Tasmota to save the update file. Switch to
**tasmota-minimal.bin** before trying again.

## Build the firmware yourself

You can compile a binary version of this firmware using [mos tools](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool). Once installed, `git clone` this repository and
run

```
mos build --build-var BOOT_CONFIG_ADDR=0x7000 --local
```

to copy all necessary libraries to `deps/...`. It does not matter if the initial
build fails, as long as the libraries have been installed. Now patch some files
with those included in this repository:

```
cp patch/Makefile.build deps/modules/mongoose-os/platforms/esp8266/Makefile.build
cp patch/rboot/rboot.c deps/modules/mongoose-os/platforms/esp8266/rboot/rboot/rboot.c
cp patch/rboot/rboot.h deps/modules/mongoose-os/platforms/esp8266/rboot/rboot/rboot.h
cp patch/core/mos.yml deps/core/mos.yml
```

and build again with

```
mos build --build-var BOOT_CONFIG_ADDR=0x7000 --local
```

for the Mongoose OS bootloader config default location. Alternatively, you may
build using `BOOT_CONFIG_ADDR=0x1000`.

The generated firmware can be found in `build/fw.zip` and flashed using the
command `mos flash --esp-erase-chip --esp-flash-params 'dio,4m,80m'`.

If you want to read it back without it changing anything, pull pin 14 low during
boot.

## Acknowledgments
Thanks to [rojer](https://github.com/rojer) for helping me with the debugging of
the initial code.

This firmware is build using a fork of [Mongoose OS docker action](https://github.com/dea82/mongoose-os-action)
which can be found [here](https://github.com/yaourdt/mongoose-os-action).

## License
Copyright (C) 2020, Mark Dornbach

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see https://www.gnu.org/licenses/.
