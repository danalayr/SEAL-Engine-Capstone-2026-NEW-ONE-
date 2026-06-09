# SEAL-Engine-Capstone-2026-NEW-ONE-
Iono Sense dual enclosure thermal and humidity stability system — > SEAL Engine 2026
# IonoSense Dual Enclosure System
**SEAL Engine 2026 — UW Electrical Engineering Capstone**

Team: Pedley Jiaxin Huang, Danalay Ramos, Ben Barzyk, Jenny Ko  
Mentors: Zheng Liu, Alexander Mamishev

## Overview
Dual-enclosure thermal and humidity management system for ionospheric 
signal detection instrumentation (ELF/VLF/ULF band).

## Repository Structure
- `analog/` — Arduino code for the analog enclosure (1 fan)
- `digital/` — Arduino code for the digital enclosure (2 fans)
- `test_sketches/` — Ground truth and component validation sketches

## Hardware
- Arduino UNO R3
- MLX90614 IR temperature sensor
- SHT31 humidity + ambient temp sensor
- Noctua NF-F12 PWM fans (one for the analog and two fans for the digital enclosure)
- IRLZ44N N-Channel MOSFETs
- 12V 12W polyimide adhesive heater pads (one digital pin controls 5 mosfets per enclosure)
