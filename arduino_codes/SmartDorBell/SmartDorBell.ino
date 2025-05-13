/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  Small bug-fix by Michael cochez

  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Button.h>
#include <Buzzer.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(10, 11);
Button buttonBell(13); // Connect your button between pin 2 and GND
Buzzer buzzer(12);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
uint8_t id;
int userChoice = 0;


LiquidCrystal_I2C lcd(0x27,16,2);
void setup()
{
  Serial.begin(9600);
  buttonBell.begin();

 
  delay(100);
  Serial.println("\n\nFingerprint sensor enrollment");

 
  
  lcd.init();// initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("valid finger");
  
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop() // run over and over again
{
  if (digitalRead(13) == HIGH) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Bell pressed");

    buzzer.begin(100);

    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_G7, 80);
    buzzer.sound(0, 240);
    buzzer.sound(NOTE_G6, 80);
    buzzer.sound(0, 240);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_G6, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_E6, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_A6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_B6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_AS6, 80);
    buzzer.sound(NOTE_A6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_G6, 100);
    buzzer.sound(NOTE_E7, 100);
    buzzer.sound(NOTE_G7, 100);
    buzzer.sound(NOTE_A7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_F7, 80);
    buzzer.sound(NOTE_G7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(NOTE_D7, 80);
    buzzer.sound(NOTE_B6, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_G6, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_E6, 80);
    buzzer.sound(0, 160);
    buzzer.sound(NOTE_A6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_B6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_AS6, 80);
    buzzer.sound(NOTE_A6, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_G6, 100);
    buzzer.sound(NOTE_E7, 100);
    buzzer.sound(NOTE_G7, 100);
    buzzer.sound(NOTE_A7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_F7, 80);
    buzzer.sound(NOTE_G7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_E7, 80);
    buzzer.sound(0, 80);
    buzzer.sound(NOTE_C7, 80);
    buzzer.sound(NOTE_D7, 80);
    buzzer.sound(NOTE_B6, 80);
    buzzer.sound(0, 160);

    buzzer.end(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("valid finger");
  }	
  uint8_t result = getFingerprintID();
  if (result == finger.fingerID) {
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print("access allowd");
    lcd.print("Access allowed");
    delay(2000);
  }
  else if (result == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("valid finger");
  }
  

  /*if (userChoice == 2) {
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();
    if (id == 0) {// ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);

    while (! getFingerprintEnroll() );
  }
  */
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

bool checkCode(const String& code) {
  if (code == "123456") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Code OK");
    buzzer.begin(10);
    buzzer.sound(NOTE_C5, 100);
    buzzer.sound(NOTE_E5, 100);
    buzzer.sound(NOTE_G5, 200);
    buzzer.sound(0, 100);       
    buzzer.sound(NOTE_G5, 300);     
    buzzer.end(1000);
    return true;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong Code!");
    buzzer.begin(10);
    buzzer.sound(NOTE_G4, 200);
    buzzer.sound(NOTE_E4, 200);
    buzzer.sound(NOTE_C4, 300);
    buzzer.sound(0, 100);           
    buzzer.end(1000);
    delay(2000);
    return false;
  }
}