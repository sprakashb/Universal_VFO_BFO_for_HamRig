float R_1= 10000.0, R_2=1000.0; // values of the resistors used in voltage divider for keypad , All keys are connected through 1K resisors (R_2) to 10K (R_1) to 5V

void keyCal()
{
  // Calibrate keypad / find the actual analog values for each key and store in eeprom
  // also may be called at power up if one of the keys (>) is pressed
  lcd.clear();
  lcd.setCursor(0, 0);
  int key_val[number_of_keys];
  int read_value = 0, k_count = 0;  // actual value read , how many times

  for (int x = 0; x <= number_of_keys - 1; x++)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press ") ;
    lcd.print(keymap[x]);
    //    lcd.println(" button.");
    while (analogRead(0) > 1000)   // wait for keypress
    {
      lcd.setCursor(0, 1);
      lcd.print("waiting..");
    }
    lcd.setCursor(0, 1);
    lcd.print("         ");

    while (analogRead(0) < 1000) ;   // till pressed
    {
      read_value = read_value + analogRead(0);
      delay(20);
      k_count++;

      if (k_count >= 100)
      {
        lcd.setCursor(0, 0);
        lcd.print("Release the key ") ;
        delay(500);
      }
    }
    read_value = read_value / k_count;  // take average
    keyPad[x] = read_value - 5;

    delay(500);
    count = 0;
  }
  save_keypad_values(); // store on eeprom

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("keypad calibrated");
  delay(1000);
  lcd.clear();
}

void fill_keypad()
{
  // fill up keypad array with calculated values as initial guess, done when magic number changes ie eeprom init
  // Keypad has 10K (R1) and 7K to 1K (R2) values
  // Vout = Vin (5V) [ R2 / (R1 + R2)]
  float adcResol, temp;

  adcResol = 5000.0 / 1024.0;
  for ( int i = 7; i >= 1; i--)
  {
    temp = (( 5000.0 * (  (i * R_2) / (R_1 + i * R_2) ) ) / adcResol ); // calc divider value and find its equival adc o/p
    keyPad[7 - i] = temp-10;  // reduce by 10 as margin 
    // test
 /*   Serial.print(temp);
    Serial.print(" - ");
    Serial.print(keymap[7 - i]);
    Serial.print(" = ");
    Serial.println(keyPad[7 - i]);
*/
    save_keypad_values(); // store on eeprom
  }
}
