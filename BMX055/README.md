# Bosch Sensortec BMX055

## Introduction

This is an Arduino SPI library for the Bosch BMX055 absolute orientation sensor.

From the Bosch Sensortec website:

> "The BMX055 is a very small absolute orientation sensor in the class of low-noise 9-axis measurement units. All three sensor components of the BMX055 can be operated and addressed independently from each other. On top, the BMX055 integrates a multitude of features that facilitate its use especially in the area of motion detection applications, such as device orientation measurement, gaming, HMI or menu browser control. All sensor parameters and all settings of the respective interrupt engines can be easily programmed via the digital interfaces"

[Click here for the BMX055 datasheet.](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmx055-ds000.pdf)

This library was borne out of the need to communicate with a BMX055 IMU, specifically using SPI
communication.

The library is modeled after the source code for the BMX055 driver previously in the [Useful Packages & Modules (UPM) repository developed by Intel](https://android.googlesource.com/platform/hardware/bsp/intel/+/4de2569d39d40009e8f440de7143f39804a213bc/peripheral/libupm/src/bmx055/).

**Please note that the library is not complete and only implements accelerometer communication for now.**

Supports the following features:
* Read chip ID
* Read filtered X,Y,Z accelerometer from the FIFO

## Basic Usage

The header file in the BMX055 directory shows all public functions in the BMX055 class.

Example Arduino code:

```c++
BMX055 bmx;

void setup()
{
    bmx.init();
}

void loop()
{
    float x, y, z;
    bmx.update();
    // Should print 250
    Serial.println(bmx.getChipID());
    bmx.getAccelerometer(&x, &y, &z);
}
```
The initialization function makes the following *accelerometer* configurations:
* Configure the Low Power Mode register to LP2 in the case the user decides to 
enter low power mode
* Set the power mode to normal
* Set the range to 2G
* Set the bandwidth to 7.81 Hz
* Enable output filtering
* Enable shadow registering
* Configure the FIFO to be in BYPASS mode and store all X,Y,Z accelerometer data
* Enable the FIFO

## Contributing

Contributions are very welcome. Please make a pull request or raise an issue.