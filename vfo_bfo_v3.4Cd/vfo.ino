void set_vfo_freq()  // called when encoder rotated, from loop
{
  vfo += enc_cnt * stepz[stepzIndex];

  enc_cnt = 0;
  
  if (bindex != 3)  // not BC then limit bands
  {
    if (vfo < BandBases[bindex])
    {
      vfo = BandBases[bindex];
    }
    else if (vfo > BandTops[bindex])
    {
      vfo = BandTops[bindex];
    }
  }
  changed_f = true;
}

//---------------

void adj_vfo_freq()    // called from loop when changed_f is true
// we can play with two different logics for generating vfo, either add vfo & bfo or subtract smaller from bigger
{
  /* if (vfo >= BandBases[bindex]  || vfo <= BandTops[bindex])
     vfo = VFO_Freq[activeVFO] ;
    else if (vfo <= BandBases[bindex])
     vfo = BandBases[bindex];
    else if (vfo >= BandTops[bindex])
     vfo = BandTops[bindex];
  */
  // Adding vfo  bfo
  //  opfreq = vfo + bfo + BandOffset[bindex];

  // subtraction logic
  opfreq = labs(vfo - bfo) + BandOffset[bindex] ;

  si5351.set_freq(opfreq * 100UL, SI5351_CLK0);
  VFO_Freq[activeVFO] = vfo;
  display_vfo_bfo();

  changed_f = false;
}

void select_vfo()   // select VFOA/VFOB/MEM in sequence on key press
{
  VFO_Freq[activeVFO] = vfo; // save current vfo
  activeVFO += 1;
  if (activeVFO >= 3)
    activeVFO = 0;
  if (activeVFO == 2) //Mem VFO
  {
    memChMode = 1;    // ch no changes by < > keys
    read_ch(activeCh);
    display_ch();
  }
  else
  {
    memChMode = 0;    // VFO A/B mode
  }
  vfo = VFO_Freq[activeVFO]; // load
  check_band(vfo);
  set_mode();
  changed_f = 1;
  changed_b = 1;  // spb 4/2023
  display_vfo_name();
  display_vfo_bfo();
  set_filter();
  // display_band();
}

void set_offset()   // band offsets
{
  BandOffset[bindex] += enc_cnt * stepz[stepzIndex];
  enc_cnt = 0;
  changed_o = true;
}


void select_sideband()    // SB = Mode
{
  mode += 1;
  if (mode >= maxModes)
    mode = 0;   // LSB/USB for bitx and 0 = cw, 1 = USB, 2 = LSB, BC for others
  display_mode();
  set_bfo_freq();
}
