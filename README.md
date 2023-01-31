
# Project Tornado

This project aims to create a full-product solution for control of 4-pin adressable rgb fans. Led effects, speed monitoring and speed control.

This project is a 3 part project; the PC GUI, the firmware running on the PCB and the PCB itself.

At the time of writing this 02/01/2023 the PCB has not been build yet so no real-hardware testing has been performed. All the software testing is included as well as documentation for the project.


## Authors

- [@stan-one](https://github.com/stan-one/)


## FAQ

#### Is this a ready to build and use project

No, I havent build the PCB yeat and I havent tested it on my own PC. If you happen to order, assamble and test the desing before I do, you will be a true pioneer :)

#### What OS does the PC software support

Currently the project is build only for linux(created on ubuntu 22.04). All the external libraries are avalible for Windows too so hopefully soon I will have ported it for windows stay tuned. 


## Documentation

Doxygen files included in the /test/ folders.

## Deployment

To deploy this project run:
For the GUI, to the project folder and run the folliwing commands
```bash
mkdir build
cd build
cmake ..
make

```

This will build the GUI and create a executable.

For the firmware, you need to install STM32CubeIDE and load the project.


At some point in time I will add the build binaries ready to run.

