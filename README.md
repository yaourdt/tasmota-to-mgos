# x2mg

A minimal firmware for OTA (over the air) flashing Mongoose OS from Tasmota
or compatible firmware types.

## Overview

This is a first working draft for an intermediate firmware which can be used to
wirelessly recover [Mongoose OS](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md)
on esp8266 based IoT devices initially shipped with a Mongoose OS based
firmware, if the device was converted to a different firmware using [mg2x](https://github.com/yaourdt/mgos-to-tasmota).
This recovery firmware will install a minimal version of Mongoose OS, and you can continue from
there to your favourite target release, such as e.g. the Shelly stock firmware.

## Supported Devices

Any device initially shipped with a Mongoose OS based firmware which has been
build using `build_vars: BOOT_CONFIG_ADDR: 0x1000`. Examples are Shelly1,
Shelly1PM, Shelly2, Shelly25, and ShellyEM.

**Currently not supported:** ShellyPlugS, ShellyRGBW2, ShellyDimmer1,
ShellyDimmer2, ShellyBulb, ShellyVintage, ShellyPlugUS.

## Install

**Warning:** _This application is still at an early stage. If something fails,
your device may be bricked, if you don't know how to flash a new firmware over a
wired connection. Proceed with caution!_

There are two versions of this firmware,
 - [for a flash size of 2 MB](https://github.com/yaourdt/tasmota-to-mgos/binaries/mgos512k-2MB.bin) and
 - [for a flash size of 4 Mb](https://github.com/yaourdt/tasmota-to-mgos/binaries/mgos512k-4MB.bin).

Shelly users should pick the 2 MB version.

Use your current firmware's update mechanism to apply x2mg as you would for any
other update. If the process succeeds, the device will reboot and you will see a
new WiFi network labeled _mg-????_.

As soon as you have joined the devices WiFi network, you can install the desired
version of a Mongoose OS based firmware via the command

```
curl -i -F filedata=@./fw.zip http://10.42.42.44/update
```

Replace `./fw.zip` with the path to your target firmware ZIP-file. If you own a
Shelly device and want to revert to stock firmware, you can find the matching
files [here](https://api.shelly.cloud/files/firmware).

If you cannot upload the intermediate firmware to Tasmota due to a _not
compatible_ error, see [this github issue](https://github.com/esphome/issues/issues/955).
In short: Running `Setoption78 1` in the Tasmota console disables compatibility
check and allows for the update to be applied.

## Build the firmware yourself

You can compile a binary version of this firmware using [mos tools](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool). Once installed, `git clone` this repository and
run

```
mos build --build-var TARGET_FLASH_SIZE=2MB --local
```

to copy all necessary libraries to `deps/...`. It does not matter if the initial
build fails, as long as the libraries have been installed.

Now move the makefile included in this repository to `deps/modules/mongoose-os/platforms/esp8266/Makefile.build`
and build again with

```
mos build --build-var TARGET_FLASH_SIZE=2MB --local
```

for a target flash size of 2MB (=32Mb). Alternatively, you may build using
`TARGET_FLASH_SIZE=4MB`.

The generated firmware can be found in `build/fw.zip` and flashed using the
command `mos flash --esp-flash-params 'dio,4m,80m'`. It can be combined into a
into a single binary file using [mgos-combine](https://github.com/yaourdt/mgos-combine).

## Acknowledgments
Thanks to [rojer](https://github.com/rojer) for helping me with the debugging of
the initial code.

This firmware is build using a fork of [Mongoose OS docker action](https://github.com/dea82/mongoose-os-action)
which can be found [here](https://github.com/yaourdt/mongoose-os-action).
