Firmware for STM32H723VGT6 used in the custom vehicle control unit (VCU) for TR27 handling data acquisition from sensors and logic

Prerequisites:
  -
- VSCode
- A STM32H7 dev board (can be any H7 but if it is not the same as the one listed make your own project and add in your code)
- GCC


Getting started:
  -

1. Download STM32CubeMX
2. Click help on the top

    a. Click Manage Embedded Software Packages
 
    b. Scroll down and find STM32H7 and install the package

4. Click on File at the top
   
   a. Load Project
   
   b. Open "STM32-config.ioc"

5. Go to Project Manager and check for these things
   
     - Toolchain/IDE is CMake
     - Default Compiler/Linker is GCC
     - MCU Reference is STM32H723VGTx
6. Click GENERATE CODE on the top right
7. Open VSCode and go to extensions
8. Install "STM32CubeIDE for Visual Studio Code" bundle
9. Open the repository once the extension has been installed
10. Select Debug for the build
11. Build the project by going to the bottom left clicking on the gear that says Build to ensure that everything is working

Flashing:
  - 

  The 2 main ways we will be flashing firmware will be with a ST-Link through SWD or J-Link through JTAG or SWD. Each way has their own software ST-Link STM32CubeProgrammer and J-Flash for J-Link

  **ST-Link**
  
  Connections 
  
  ST-Link -> STM32
  
  SWCLK -> SWCLK (or clk)
  
  SWDIO -> SWDIO (or DIO)
  
  3.3V/VCC -> 3.3V
  
  GND -> GND

  Open ST32CubeProgrammer 
  1. Click Connect
  2. Click Open File and find the .elf file
  3. Click Start Programming
  **J-Link**

  Connections (if using SWD you do not have to include TDI, TDO, and RESET)
  
  J-Link -> STM32
  
  VTref -> VCC
  
  GND -> GND
  
  TMS -> JTMS/SWCLK
  
  TDI -> JTDI
  
  TDO -> JTDO
  
  TCK -> JTCK/SWCLK
  
  RESET -> NRST
  
  Open J-Flash (any version)
  1. Create New project if first time opening
  2. Click find target device and find STM32H723VG
  3. Select protocol that you are using
  4. Click File and then Open Data File and select the .elf
  5. Click Target and Click Connect
  6. Flash
   
