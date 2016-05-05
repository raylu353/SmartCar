#include <DHT.h>
#include <Wire.h>
#include "stdlib.h"


#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

String data = "";
String stat;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

    dht.begin();
    pinMode(11,OUTPUT);//H-Swich
    digitalWrite(11,LOW);
    pinMode(12,OUTPUT);//H-Power
    digitalWrite(12,LOW);
    
    Wire.begin(8);//address = 8;
    Wire.onRequest(requestEvent);
    stat = "OFF";
}

void loop() {
  String sensor = "OK";
  data = "";
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    sensor = "ERROR";
  }else {
    sensor = "OK";
  }

  

  if (sensor.equals("OK")) {
    if((t > 22 && stat.equals("OFF"))) {
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      delay(200);
      digitalWrite(11,LOW);
      stat = "ON ";
    }
    else if( t <=22 && stat.equals("ON ") ){
      digitalWrite(12,LOW);
      stat = "OFF";
  
    }
    data.concat(stat);
    data.concat(",");
    data.concat(t);
    data.concat(",");
    data.concat(h);
    data.concat("\n");
  }
  else {
    data.concat("ERR,ERROR,ERROR");
  }
  Serial.println(data);
  delay(1000);
}
  
void requestEvent() {
  char x[17];
  data.toCharArray(x,17);
  Wire.write(x);
}


