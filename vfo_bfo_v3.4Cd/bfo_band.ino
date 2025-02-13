void set_bfo_freq()    // called if encoder roated and  setbfo is true, from loop
{
  mode_offset[mode] += enc_cnt * stepz[stepzIndex]; // change BFO
  enc_cnt = 0;
  changed_b = true;
}
//-----------

void adj_bfo_freq()   // called if changed_b true, from loop
{
  bfo = if_freq  + mode_offset[mode];
  //  display_vfo_bfo();
  si5351.set_freq( bfo * 100UL, SI5351_CLK2);
  display_vfo_bfo();
  changed_b = false;
}
//-----------

void set_mode()       // BFO mode : when freq is retrieved from mem sel LSB or USB
{
#ifdef bitx
  if (bindex >= 1) // 10 MHz and above
    mode = 1; // USB
  else
    mode = 0;  // LSB
#else
  if (bindex >= 1) // 10 MHz and above
    mode = 1; // USB
  else
    mode = 2;  // LSB
  if (bindex == maxBands - 1) // BC band
    mode = 3;
#endif
  //  adj_bfo_freq();
  display_mode();
}

//------------------BAND Related ---------------
void check_band(uint32_t freq)   // find out band from freq
{
  for (int i = 0; i <= maxBands; i++)
  {
    if (freq >= BandBases[i] && freq <= BandTops[i])
    {
      bindex = i;
      break;
    }
  }
}

void select_band()      // choose one of the bands
{
  VFO_Freq[activeVFO] = vfo ;  //store current freq try

  bindex += 1;
  if (bindex >= maxBands)
    bindex = 0;
  vfo = BandWkgFreqs[bindex]; // or from eprom??
  //    vfo = VFO_Freq[activeVFO];  //try
  changed_f = 1;
  display_vfo_bfo();
  set_mode();
  display_mode();
  set_bfo_freq();
  set_filter();
}
