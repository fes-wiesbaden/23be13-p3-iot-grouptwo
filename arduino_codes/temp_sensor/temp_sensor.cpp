#include <LiquidCrystal.h>      
#include <dht.h>
#include <LiquidCrystal_I2C.h>

#define outPin 8

LiquidCrystal_I2C lcd(0x27,16,2);  
dht DHT;                

void setup() {
	lcd.init();
  lcd.clear();         
  lcd.backlight();
	lcd.begin(16,2); 	
}

void loop() {
	int readData = DHT.read11(outPin);
	
	float t = DHT.temperature;
	float h = DHT.humidity;
	
	lcd.setCursor(0,0);
	lcd.print("Temp.: ");
	lcd.print(t);
	lcd.print((char)223);	
	lcd.print("C");

	lcd.setCursor(0,1);
	lcd.print("Humi.: ");
	lcd.print(h);
	lcd.print("%");
	
	delay(2000);
}