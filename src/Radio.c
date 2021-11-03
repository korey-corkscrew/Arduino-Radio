#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <radio.h>
#include <Wire.h>
 

#define SEEK_DOWN   2                     // SEEK DOWN PUSHBUTTON -> PD2                  
#define SEEK_UP     3                     // SEEK UP PUSHBUTTON -> PD3

Radio radio;
LiquidCrystal_I2C lcd(0x27,16,2);         // LCD ADDRESS 0x27. 16 COLUMNS. 2 ROWS.


/* CREATE ANTENNA CHARACTER FOR DISPLAY ON LCD */

byte CHR_ANTENNA[8] = {
  B11111, B10101, B10101, B01110, B00100, B00100, B00100, B00100
};

/* CREATE SIGNAL CHARACTERS FOR DISPLAY ON LCD */

byte CHR_STRENGTH[6][8] = {
    {
        B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000
    }, {
        B00000, B00000, B00000, B00000, B00000, B00000, B00100, B10100
    }, {
        B00000, B00000, B00000, B00000, B00000, B00001, B00101, B10101
    }, {
        B00000, B00000, B00000, B00000, B10000, B10000, B10000, B10000
    }, {
        B00000, B00000, B00000, B00100, B10100, B10100, B10100, B10100
    }, {
        B00000, B00000, B00001, B00101, B10101, B10101, B10101, B10101
    }
};


void setup() {
  pinMode(SEEK_UP,INPUT_PULLUP);            // CONFIGURE INPUT PINS 
  pinMode(SEEK_DOWN,INPUT_PULLUP);

  lcd.init();                               // INITIALIZE LCD
  lcd.backlight();                          // TURN LCD BACK LIGHT ON
  lcd.clear();                              // CLEAR LCD
  
  radio.init();                             // INITIALIZE RADIO
  radio.setMute(true);                      // MUTE RADIO DURING STARTUP
  radio.tuneTo(getEepromFreq());            // TUNE TO LAST SAVED STATION                    
  delay(1000);                              // 1 SECOND DELAY FOR TUNING
  radio.setMute(false);                     // UNMUTE RADIO STARTUP
  radio.setVolume(15);                      // SET MAX VOLUME
 
}

void loop() {
  radio.updateStatus();                     // UPDATE RADIO DATA
  lcd.setCursor(0,0);                       // LCD CURSOR: COL. 0, ROW 0
  lcd.print("FM ");  
  
  if(radio.state.frequency >= 100.0){
    lcd.setCursor(6,0);
    lcd.print(radio.state.frequency);       // PRINT STATION FREQUENCY                        
  }
  else{
    lcd.setCursor(6,0);
    lcd.print(" ");
    lcd.print(radio.state.frequency);       // PRINT STATION FREQUENCY                        
  }
  
  lcd.setCursor(12,0);                      // LCD CURSOR: COL. 13, ROW 0  
  lcd.print(" MHz");

  drawStatusLine();                         // PRINT SIGNAL STRENGTH

  if(lastFreq != radio.state.frequency){    // STORE CURRENT STATION IN EEPROM
    setEepromFreq(radio.state.frequency); 
  }


  if(digitalRead(2) == LOW){                // SEEK DOWN WHEN PIN 2 IS LOW
    radio.seekDown();
    delay(100);
  }
  
  if(digitalRead(3) == LOW){                // SEEK UP WHEN PIN 3 IS LOW
    radio.seekUp();
    delay(100);
  }
}


/* LOAD LAST STORED FREQUENCY FROM EEPROM */

float getEepromFreq() {
  float frequency = 0.0f;
  frequency += EEPROM.read(6)  * 100.0f;	// 0.00
  frequency += EEPROM.read(7)  * 10.0f;		// 90.00
  frequency += EEPROM.read(8);			    // 91.00
  frequency += EEPROM.read(9)  / 10.0f;		// 91.50
  frequency += EEPROM.read(10) / 100.0f;	// 91.50
  return frequency;
}


/* STORE LAST TUNED FREQUENCY TO EEPROM */

void setEepromFreq(float frequency) {
  unsigned short freq = frequency * 100;	// 91.5 -> 9150
  EEPROM.update(6, (freq / 10000));		    // 0
  EEPROM.update(7, (freq / 1000) % 10);		// 9
  EEPROM.update(8, (freq / 100) % 10);		// 1
  EEPROM.update(9, (freq / 10) % 10);		// 5
  EEPROM.update(10, freq % 10);			    // 0
}


/* CREATE AND PRINT SIGNAL STRENGTH OF CURRENT STATION ON LCD */

void drawStatusLine() {
  switch (radio.state.signalStrength) {
    case 0 ... 13:                          // SIGNAL STRENGTH 1
      lcd.createChar(1, CHR_STRENGTH[0]);
      lcd.createChar(2, CHR_STRENGTH[0]);
      break;
    case 14 ... 23:                         // SIGNAL STRENGTH 2
      lcd.createChar(1, CHR_STRENGTH[1]);
      lcd.createChar(2, CHR_STRENGTH[0]);
      break;
    case 24 ... 33:                         // SIGNAL STRENGTH 3
      lcd.createChar(1, CHR_STRENGTH[2]);
      lcd.createChar(2, CHR_STRENGTH[0]);
      break;
    case 34 ... 43:                         // SIGNAL STRENGTH 4
      lcd.createChar(1, CHR_STRENGTH[2]);
      lcd.createChar(2, CHR_STRENGTH[3]);
      break;
    case 44 ... 53:                         // SIGNAL STRENGTH 5
      lcd.createChar(1, CHR_STRENGTH[2]);
      lcd.createChar(2, CHR_STRENGTH[4]);
      break;
    case 54 ... 63:                         // SIGNAL STRENGTH 6
      lcd.createChar(1, CHR_STRENGTH[2]);
      lcd.createChar(2, CHR_STRENGTH[5]);
      break;
  }

  lcd.setCursor(0,1);                       // MOVE CURSOR TO FIRST COLUMN, SECOND ROW
  lcd.write(byte(0));                       // PRINT ANTENNA CHARACTER ON LCD
  lcd.write(byte(1));                       // PRINT SIGNAL STRENGTH 1 - 3
  lcd.write(byte(2));                       // PRINT SIGNAL STRENGTH 4 - 6
}