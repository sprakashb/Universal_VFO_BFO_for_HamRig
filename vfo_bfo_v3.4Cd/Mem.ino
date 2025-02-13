void init_eprom()      // write some info on EEPROM when initially loaded or when magic number changes
{
  uint16_t i;
  uint32_t temp;
  int address;

  EEPROM.write(magic_no_addr, magic_no);  // check byte may be same as ver no at 0 address
  //  display_mem_msg("InitEPrm");
  // write various parameters in the memory beginning
  lcd.setCursor(14, 0);
  lcd.print("IE"); // Initializing Eprom Indicator
  // VFO's
  EEPROM_writeAnything(VFO_A_addr, 7050000UL);
  EEPROM_writeAnything(VFO_B_addr, 14200000UL);

  EEPROM_writeAnything(IF_addr, if_freq);
  EEPROM_writeAnything(CWOffset_addr, mode_offset[0] );
  EEPROM_writeAnything(USBOffset_addr, mode_offset[1]);
  EEPROM_writeAnything(LSBOffset_addr, mode_offset[2]);
  EEPROM_writeAnything(BCOffset_addr, mode_offset[3]);

  EEPROM_writeAnything(BandOffset0_addr, BandOffset[0]);
  EEPROM_writeAnything(BandOffset1_addr, BandOffset[1]);
  EEPROM_writeAnything(BandOffset2_addr, BandOffset[2]);
  EEPROM_writeAnything(BandOffset3_addr, BandOffset[3]);

  // Now store next 20 channels with same vfo value - starts with 1 not 0
  for (int i = 1; i <= 06; i++)
  {
    address = VFO_M_base_addr + sizeof(vfo) * i;
    EEPROM_writeAnything(address, 7100000UL);
  }

  for ( i = 06; i <= 11; i++)
  {
    address = VFO_M_base_addr + sizeof(vfo) * i;
    EEPROM_writeAnything(address, 14200000UL);
  }

  for ( i = 11; i <= 16; i++)
  {
    address = VFO_M_base_addr + sizeof(vfo) * i;
    EEPROM_writeAnything(address, 7020000UL);
  }

  for ( i = 16; i <= 21; i++)
  {
    address = VFO_M_base_addr + sizeof(vfo) * i;
    EEPROM_writeAnything(address, 14250000UL);
  }

  fill_keypad();  // calculate keypad values as initial guess, if keypad not working properly need to calibrate it
  // calibration is invoked by keeping the ">" (rightmost) key pressedwhile applying power

  lcd.setCursor(14, 0);
  lcd.print("  "); // Initializing Eprom Indicator blanked
}

void read_eprom()     // should be called at powerup to retrieve stored values for vfos A and B
{ int address;
  uint32_t temp;
  // read various parameters in the memory beginning
  EEPROM_readAnything(VFO_A_addr, VFO_Freq[0]);
  EEPROM_readAnything(VFO_B_addr, VFO_Freq[1]);
  EEPROM_readAnything(IF_addr, if_freq);
  EEPROM_readAnything(VFO_M_base_addr, VFO_Freq[2]);
  EEPROM_readAnything(CWOffset_addr, mode_offset[0] );
  EEPROM_readAnything(USBOffset_addr, mode_offset[1]);
  EEPROM_readAnything(LSBOffset_addr, mode_offset[2]);
  EEPROM_readAnything(BCOffset_addr, mode_offset[3]);
  EEPROM_readAnything(BandOffset0_addr, BandOffset[0]);
  EEPROM_readAnything(BandOffset1_addr, BandOffset[1]);
  EEPROM_readAnything(BandOffset2_addr, BandOffset[2]);
  EEPROM_readAnything(BandOffset3_addr, BandOffset[3]);

  vfo = VFO_Freq[activeVFO];
  check_band(VFO_Freq[activeVFO]);
  set_mode();
  changed_f = 1;
  read_keypad_values() ;
}

void read_ch(int chNo)    // read channel info from eeprom when ever mem ch no changed
{
  int address = VFO_M_base_addr + sizeof(vfo) * chNo;
  EEPROM_readAnything(address, VFO_Freq[activeVFO]);
  check_band(VFO_Freq[activeVFO]);
  set_mode();
  changed_f = 1;
}


void save_vfo()    // save vfos, bfos, band offsets (save key)
{
  lcd.setCursor(15, 0);
  lcd.print("-");

  if (activeVFO == 0)
    write_vfo_A();
  else if (activeVFO == 1)
    write_vfo_B();
  else if (activeVFO == 2)
    write_ch(activeCh);
  lcd.setCursor(15, 0);
  lcd.print("+");
  // delay(2000);
  write_bfos();   // if changed write to mem
  write_bandOffsets();
  lcd.setCursor(15, 0);
  lcd.print(" ");

}

void write_ch(int chNo)   // write memory channel into eeprom if different from stored value
{
  int address = VFO_M_base_addr + sizeof(vfo) * chNo;
  EEPROM_readAnything(address, x);
  if (x != VFO_Freq[activeVFO])
    EEPROM_writeAnything(address, VFO_Freq[activeVFO]);
}

void write_vfo_A()
{
  EEPROM_readAnything(VFO_A_addr, x);
  if (x != VFO_Freq[0])
    EEPROM_writeAnything(VFO_A_addr, VFO_Freq[0]);
}

void write_vfo_B()
{
  EEPROM_readAnything(VFO_B_addr, x);
  if (x != VFO_Freq[1])
    EEPROM_writeAnything(VFO_B_addr, VFO_Freq[1]);
}

void write_bfos()   // compare with stored values and write only if different
{
  lcd.setCursor(15, 0);
  lcd.print(">");     // indicate BFO update
  EEPROM_readAnything(IF_addr, x);
  if (x != if_freq)
    EEPROM_writeAnything(IF_addr, if_freq);

  EEPROM_readAnything(CWOffset_addr, x);
  if ( x  != mode_offset[0] )   // different from what was saved then store current value
    EEPROM_writeAnything(CWOffset_addr, mode_offset[0] );

  EEPROM_readAnything(USBOffset_addr, x);
  if ( x != mode_offset[1])
    EEPROM_writeAnything(USBOffset_addr, mode_offset[1]);

  EEPROM_readAnything(LSBOffset_addr, x);
  if ( x != mode_offset[2])
    EEPROM_writeAnything(LSBOffset_addr, mode_offset[2]);

  EEPROM_readAnything(BCOffset_addr, x);
  if ( x != mode_offset[3])
    EEPROM_writeAnything(BCOffset_addr, mode_offset[3]);
  lcd.setCursor(15, 0);
  delay(2000);
  lcd.print(" ");     // indicate BFO update
}

void write_bandOffsets()
{
  EEPROM_readAnything(BandOffset0_addr, x);  // compare with stored value write if different
  if (x != BandOffset[0])
    EEPROM_writeAnything(BandOffset0_addr, BandOffset[0]);

  EEPROM_readAnything(BandOffset1_addr, x);
  if (x != BandOffset[1])
    EEPROM_writeAnything(BandOffset1_addr, BandOffset[1]);

  EEPROM_readAnything(BandOffset2_addr, x);
  if (x != BandOffset[2])
    EEPROM_writeAnything(BandOffset2_addr, BandOffset[2]);

  EEPROM_readAnything(BandOffset3_addr, x);
  if (x != BandOffset[3])
    EEPROM_writeAnything(BandOffset3_addr, BandOffset[3]);
}

//-------keypad calculated & measured values saved and retrieved

void save_keypad_values()   // save to eeprom
{
  int address;

  for (int i = 0; i <= 07; i++)
  {
    address = key_pad_base_addr + sizeof(uint16_t) * i;
    EEPROM_writeAnything(address, keyPad[i]);
  }
}

void read_keypad_values()   // read from eeprom
{
  int address;

  for (int i = 0; i <= 07; i++)
  {
    address = key_pad_base_addr + sizeof(uint16_t) * i;
    EEPROM_readAnything(address, keyPad[i]);
  }
}
