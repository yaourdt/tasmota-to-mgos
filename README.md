# x2mg

A minimal firmware for OTA (over the air) flashing Mongoose OS from Tasmota,
Tuya stock, or compatible firmware types.

## Overview

This is a first working draft for an intermediate firmware that can be used to
wirelessly install [Mongoose OS](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md)
on almost any esp8266 based IoT device. It will install a minimal version of
Mongoose OS, and you can continue from there to your favourite target release,
such as e.g. the Shelly stock firmware.

## Install

**Warning:** _This application is still at an early stage. If something fails,
your device may be bricked, if you don't know how to flash a new firmware over a
wired connection. Proceed with caution!_

There are two ways for installing this intermediate firmware:

  - Either use your current firmware's update mechanism to upload the matching
    version of x2mg compiled for your device
  - or emulate a cloud service using [Tuya Convert](https://github.com/ct-Open-Source/tuya-convert/)
    and place the matching version of x2mg in Tuya Converts `files/` directory.

Either way, if the process succeeds, the device will reboot and you will see a
new WiFi network labeled _mg-????_. If you used your current firmware's update
mechanism, join the WiFi network. If you are using Tuya Convert, x2mg
will auto-connect to the _vtrust-flash_ WiFi.

As soon as you have joined the devices WiFi network (or it has connected to
_vtrust-flash_), you can install any version of a Mongoose OS based firmware via
the command

```
curl -i -F filedata=@./fw.zip http://10.42.42.44/update
```

Replace `./fw.zip` with the path to your target firmware ZIP-file. If you
own a Shelly device and want to revert to stock firmware, you can find the
matching files [here](https://api.shelly.cloud/files/firmware).

If you cannot upload the intermediate firmware to Tasmota due to a _not
compatible_ error, see [this github issue](https://github.com/esphome/issues/issues/955).
In short: Running `Setoption78 1` in the Tasmota console disables compatibility
check and allows for the update to be applied.

## Build the firmware yourself

You can compile a binary version of this firmware using [mos tools](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool). Once installed, `git clone` this repository and
run `mos build` to create a binary located in `build/fw.zip`. You can convert the
generated bin-files into a single file using [mgos-combine](https://github.com/yaourdt/mgos-combine).

## Acknowledgments
Thanks to [rojer](https://github.com/rojer) for helping me with the debugging of
the initial code.

This firmware is build using a fork of [Mongoose OS docker action](https://github.com/dea82/mongoose-os-action)
which can be found [here](https://github.com/yaourdt/mongoose-os-action).
