
# Project Tornado

This project aims to create a holistic solution for the control of 4-pin adressable rgb fans, led effects, speed monitoring and speed control.

This project consists of 3 parts; the PC GUI, the firmware running on the PCB and the PCB itself.

At the time of writing this 02/01/2023 the PCB has not been build yet so no real-hardware testing has been performed. All the software testing is included, as well as documentation for the project.

![alt text](https://iili.io/H1Vq0bV.png)
![alt text](https://iili.io/H1VqN5v.png)

## Authors

- [@stan-one](https://github.com/stan-one/)


## FAQ

#### Is this a ready to use project

No, I havent build the PCB yet and I havent tested it on my own PC. If you happen to order, assamble and test the desing before I do, you will be a true pioneer :)

#### What OS does the PC software support

Currently the project is build only for linux(created on ubuntu 22.04). All the external libraries are avalible for Windows too so hopefully soon I will  port it for windows, stay tuned. 


## Documentation

Doxygen files included in the test/ folders.

## Deployment

For the GUI, to make the project, run the following commands in the project folder
```bash
mkdir build
cd build
cmake ..
make

```

This will build the GUI and create a executable.

For the firmware, you need to install STM32CubeIDE and load the project.

For the PCB you will need to install KiCAD6. The project includes non-standart components, so its likely that you will have missing footprints or symbols. If thats the case go to [Ultralibrarian][1] and add them.

The componets are avalible in the JLCPCB part cataloge so the pcbs can be ordered assambled from factory.

[1]: https://www.ultralibrarian.com/ Ultralibrarian

