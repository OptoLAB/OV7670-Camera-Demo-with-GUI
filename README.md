# OV7670 Camera Demo with GUI
 Demo project for OV7670 camera using DCMI and STemWin GUI library
## About project
This is a demo project for [OmniVision OV7670] image sensor done using digital camera interface or [DCMI](https://www.st.com/resource/en/application_note/an5020-digital-camera-interface-dcmi-on-stm32-mcus-stmicroelectronics.pdf). Project was built using following ecosystem:

 - [mikromedia 7 for STM32F7](https://www.mikroe.com/mikromedia-7-stm32f7) development board by Mikroelektronika with STM32F746ZG mcu.
 - [mikroProg](https://www.mikroe.com/mikroprog-stm32) v1.5. Mikromedia 7 board has onboard mikroProg programmer and debugger.
 - [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html?sc=stm32cubemx) 5.6.0 initialization code generator by STMicroelectronics.
 - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) 1.3.0 Integrated Development Environment for STM32.
 - [STemWin](https://www.st.com/en/embedded-software/stemwin.html) GUI library (SEGGER's emWin library for STM32)

More info can be found [here](https://www.optolab.ftn.uns.ac.rs/index.php/education/project-base/283-ov7670-camera-demo)

Demo video on [youtube](https://www.youtube.com/watch?v=5b7or2_Q1HU) 

<p align="center">
<img src="https://github.com/OptoLAB/OV7670-Camera-Demo-with-GUI/blob/main/img/photo.jpg" width="600"/>
</p>

Libjpeg is used for recording and loading captured images. Below is an exapmle of image in CIF format (352x288) captured using this demo 

<p align="center">
<img src="https://github.com/OptoLAB/OV7670-Camera-Demo-with-GUI/blob/main/img/capture.JPG" width="352"/>
</p>
