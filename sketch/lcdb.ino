/*************************************************************************
* Demo sketch for MultiLCD library
* Distributed under GPL v2.0
* (C)2013-2014 developed by Stanley Huang <stanleyhuangyc@gmail.com>
* For more information, please visit http://arduinodev.com
*************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <MultiLCD.h>
#include <Wire.h>

LCD_ILI9341 lcd; /* for 2.2" SPI TFT LCD module */

void setup()
{
  Wire.begin();
  Serial.begin(9600);
	lcd.begin();
	lcd.clear();
  
	lcd.setColor(255, 255, 0);
	lcd.setCursor(0, 0);
	lcd.setFontSize(FONT_SIZE_XLARGE);
  lcd.print("Welcome to Active Smart Car System!");
  //Test Speedometer
  Wire.beginTransmission(4);
  byte error = Wire.endTransmission();

  lcd.setCursor(0,3);
  if(error != 0) {
    lcd.print("Speedometer: OFF LINE!");
  } else {
    lcd.print("Speedometer: ON LINE!");
  }
  //Test Humidifier
  Wire.beginTransmission(8);
  error = Wire.endTransmission();

  lcd.setCursor(0,6);
  if(error != 0) {
    lcd.print("Humidifier: OFF LINE!");
  } else {
    lcd.print("Humidifier: ON LINE!");
  }

  delay(3000);
  lcd.clear();
}

void loop()
{
  String data = "";
  String carPower = "";
  //Check if Speedo online
  Wire.beginTransmission(4);
  byte error = Wire.endTransmission();
  if(error == 0) {
    /*Wire.requestFrom(4,3,true);
    while (Wire.available()) {
      char x = Wire.read();
      speed.concat(x);
    }*/
    carPower = "ON "; 
  }
  else {
    carPower = "OFF";
  }
  String output = carPower;
  //Check if Humidifier online
  Wire.beginTransmission(8);
  error = Wire.endTransmission();

  if(error == 0) {
    Wire.requestFrom(8,17);
    while (Wire.available()) {
      char c = Wire.read();
      data.concat(c);
    }
    lcd.setFontSize(FONT_SIZE_XLARGE);
    lcd.setCursor(0, 0);
    
    String temperature = "";
    temperature.concat(data[4]);
    temperature.concat(data[5]);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    
    String humidity = "";
    humidity.concat(data[10]);
    humidity.concat(data[11]);
    lcd.setCursor(0, 3);
    lcd.print("Humidity: ");
    lcd.print(humidity);
  
    String humidifier ="";
    humidifier.concat(data[0]);
    humidifier.concat(data[1]);
    humidifier.concat(data[2]);
    lcd.setCursor(0, 6);
    lcd.print("Humidifier: ");
    lcd.print(humidifier);
  
    lcd.setCursor(0,9);
    lcd.print("carPower: ");
    lcd.print(carPower);

    output.concat(",");
    output.concat(temperature);
    output.concat(",");
    output.concat(humidity);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Error!              ");
    lcd.setCursor(0,3);
    lcd.print("Humidifier:         ");
    lcd.setCursor(0,6);
    lcd.print("OFF LINE            ");
    lcd.setCursor(0,9);
    lcd.print("carPower: ");
    lcd.print(carPower);
    output.concat(",-,-");
  }

  lcd.setCursor(0,15);
  String serverMsg = "";
  serverMsg = Serial.readString();
  if(serverMsg.equals("")) {
    lcd.print("Server is down");
  } else {
    lcd.print(serverMsg);
  }
  
  Serial.println(output);
  
  
  delay(2000);
}
