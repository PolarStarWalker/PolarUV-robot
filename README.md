# PolarUV-robot
This software is part of PolarUV project and runs on a raspberry pi 4 single board computer or similar.


# Dependencies
Hardware dependencies:
- i2c
- spi

Software dependencies:
 - gcc - 10.2
 - cmake >= 3.16
 - linux-drivers for i2c bus
 - linux-drivers for spi bus
 - conan 1.48
 - all needed libraries automatically installing by conan


This project uses conan as packet manager and if you want pre-built binaries do this command in terminal

    conan remote add starwalker https://polarrov.jfrog.io/artifactory/api/conan/windows-clang-conan


