# Universal_VFO_BFO_for_HamRig
This project is for making a VFO/BFO for ham rigs with 2 VFO's and 20Memory VFOs 

A Si5351 based VFO + BFO for any single conversion ham rig was designed and its corresponding Arduino code developed. The system provides following facilities :
0. Using Atmel ATMEGA328 processor with Si5351 oscillator and a 2X16 display. (Photo below). A dedicated PCB designed. 7 Key keypad.
1. Adjustment of Frequency using a Rotary encoder.
2. Changing the step size of the frequency (1 Hz to 10MHz) displayed as underscore cursor under the digit affected.
3. Two VFOs named VFO A and B. Third VFO is from EEPROM Memory  (20 channels), which can be adjusted and Saved.
4. Selection of one of the predefined bands. Based on the selection one of the 3 band control outputs may be activated to select Filters.
5. Choosing Sidebands (USB/LSB/CW/BC ).
6. Adjusting frequency of BFO.
7. Saving to memory.

![IMG_20180826_153741477_LL](https://github.com/user-attachments/assets/19047267-157b-4989-b68f-707d6a27cb93)

The manual.pdf file is for an older version but most of the feaures are same/similar. I will try to update it asap.
