//---------Displays---------
void update_display()
{
  display_TxRx();
  display_vfo_bfo() ;  // vfo/bfo/offset
  display_stepz();

  // 2nd line
  display_vfo_name();
  display_mode();
}


void display_band()   // not used now
{
  lcd.noCursor();
  lcd.setCursor(12, 0);
  lcd.print(BandNames[bindex]);
}

//----------  1st Line LCD Start
void display_TxRx()
{
  lcd.noCursor();
  lcd.setCursor(0, 0);
  if (inTx)
    lcd.print("Tx");
  else
    lcd.print("Rx");
}

void display_vfo_bfo()    // vfo/bfo/offset
{
  lcd.setCursor(vfoPosn, 0);
  if (setbfo)
  {
    if (bfo < 10000000)
      lcd.print("0");
    lcd.print(bfo / 1000.0, 3);
  }
  else if (setOffset)
  {
    lcd.setCursor(vfoPosn, 0);
    lcd.print("          ");
    lcd.setCursor(vfoPosn + 5, 0);
    lcd.print(BandOffset[bindex]);
  }
  else
  {
    if (vfo < 10000000)
      lcd.print("0");
    lcd.print((vfo ) / 1000.0, 3);
  }
  display_vfo_name();
}

void display_stepz()
{
  curPos = 7 - stepzIndex; // underline cursor pos
  if (curPos >= 5) // avoid cursor under decimal point
    curPos ++;
  lcd.setCursor(vfoPosn + curPos, 0); // underline cursor
  lcd.cursor();
}
//  Display Line 1 End -------------


//---Display Line 2 LCD
void display_vfo_name()
{
  lcd.noCursor();
  lcd.setCursor(0, 1);  //0
  if (setbfo)
    lcd.print("BFO ");
  else
  {
    lcd.print(VFO_Names[activeVFO]);  // A/B/M
    if (activeVFO != 2 )
    {
      // lcd.setCursor(5, 1); //cleanup ch no left by mem ch no //1
      // lcd.print("  ");
    }
    else
      display_ch();
  }
}

void display_ch() // display ch no as well as its stored freq
{
  lcd.noCursor();
  lcd.setCursor(1, 1);
  // Serial.println(activeCh);
  if (activeCh < 10)
    lcd.print(" ");
  lcd.print(activeCh);
  if (memChMode == 1)
  {
    read_ch(activeCh);
    memChMode = 2;    // alredy in mem mode
  }
}

void display_mode()
{
  lcd.noCursor();
  lcd.setCursor(5, 1); //4
  lcd.print(mode_name[mode]);
}

#if defined lagoon
void display_s_meter()
{
  smeter.drawValue( analogRead(smIn), 1023);
}
#endif
//--End Line 2 displ
